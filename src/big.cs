using System.Collections;
using System.Diagnostics;
using System.Text.RegularExpressions;
using ForPeople.Core.Actions;
using ForPeople.Core.Configuration;
using ForPeople.Core.DataConverters;
using ForPeople.Core.DocumentModel.Abstractions;
using ForPeople.Core.DocumentModel.Abstractions.Contexts;
using ForPeople.Core.DocumentModel.Contract;
using ForPeople.Core.DocumentModel.Contract.Buttons;
using ForPeople.Core.DocumentModel.Contract.FormFields;
using ForPeople.Core.DocumentModel.Contract.Interface;
using ForPeople.Core.DocumentModel.Events;
using ForPeople.Core.DocumentModel.Policies;
using ForPeople.Core.DocumentModel.Validators.Frontend;
using ForPeople.Core.Enums;
using ForPeople.Core.Exceptions;
using ForPeople.Core.Extensions;
using ForPeople.Core.FileStorage;
using ForPeople.Core.Hints;
using ForPeople.Core.Localization;
using ForPeople.Core.References;
using ForPeople.Core.Security;
using MongoDB.Bson;
using MongoDB.Bson.Serialization;
using Serilog;

namespace ForPeople.Core.DocumentModel;

public static class DocumentModelConstants
{
    public const string HiddenModelFieldName = "$$model";
    public const string ComplexCollectionUpdateCacheKey = "ComplexCollectionUpdateCache";
}

public record DocumentModelFieldModification(DocumentModelFieldModificationStatus Status, object? Source, bool Ambiguous);

/// <inheritdoc />
public partial class DocumentModel<TModel> : IDocumentModel
    where TModel : notnull
{
    // ReSharper disable once StaticMemberInGenericType
    static readonly Regex ObjectListChange = new Regex(@"(?<parentField>.+)\[(?<index>\d+)\]\.(?<field>.+)", RegexOptions.Compiled);

    readonly IDocumentModelContextFactory _contextFactory;
    readonly DocumentModelPrototype<TModel> _prototype;

    bool _isNewDocument;

    public bool IsNewDocument => _isNewDocument;
    public string? DisplayName => _prototype.DisplayName;
    public string? ConfiguratorDescription => _prototype.ConfiguratorDescription;
    public object? Context { get; set; }
    public string ModelKey => _prototype.ModelKey;
    public string MetaObjectId => _prototype.MetaObjectId;

    internal DocumentModel(
        DocumentModelPrototype<TModel> prototype,
        IDocumentModelContextFactory documentModelContextFactory
    )
    {
        _prototype = prototype;
        _contextFactory = documentModelContextFactory;
    }

    ILogger GetLog(string scope) => LogUtils.GetScoped($"DocumentModel<{GetType().GenericTypeArguments[0].Name}>:{scope}");

    bool PatchModeEnabled => _prototype.FieldPatchModification != null;

    ObjectId GetModelId(TModel model) => _prototype.Id?.Invoke(model) ?? ObjectId.Empty;

    ObjectId IDocumentModel.GetModelId(object? model)
    {
        if (model is TModel validModel)
            return GetModelId(validModel);
        return ObjectId.Empty;
    }

    /// <summary>
    /// Устанавливает флаг, указывающий на то, что модель документа создана в контексте его создания, а не редактирования.
    /// </summary>
    public void SetDocumentIsNewFlag()
    {
        _isNewDocument = true;
    }

    #region IDocumentModel

    async Task<DocumentModelUpdateRequest> IDocumentModel.ToUpdateRequest(
        object? source,
        Func<DocumentModelFieldUpdate, bool>? fieldFilter
    )
    {
        ArgumentNullMultiLangException.ThrowIfNull(source);
        return await ToUpdateRequest((TModel)source, fieldFilter ?? (_ => true), false);
    }

    IDocumentModelNamingPolicy[] IDocumentModel.NamingPolicies => _prototype.NamingPolicies.Values.ToArray();

    async Task<UpdateModelResult> IDocumentModel.Update(
        object model,
        DocumentModelUpdateRequest request,
        string? changedField,
        DocumentModelUpdateOptions? options,
        DocumentModelContext? specificContext
    )
    {
        return await UpdateModel((TModel)model, request, changedField, options, specificContext: specificContext);
    }

    async Task IDocumentModel.ValidateAndThrow(
        object model)
    {
        await ValidateAndThrow((TModel)model);
    }

    async Task<DocumentModelCompactDto> IDocumentModel.ToCompactViewModel(
        object model,
        IDocumentModelCompactFieldValuePipeline[]? pipelines
    )
    {
        return await ToCompactViewModel((TModel)model, pipelines: pipelines);
    }

    async Task<DocumentModelCompactDto> IDocumentModel.ToCompactViewModel(
        object model,
        DocumentModelContext context,
        IDocumentModelCompactFieldValuePipeline[]? pipelines
    )
    {
        return await ToCompactViewModel((TModel)model, context, pipelines);
    }

    async Task<DocumentModelViewDto> IDocumentModel.ToViewModel(object model, DocumentModelViewOptions? options)
    {
        if (model is BsonDocument document && typeof(TModel) != typeof(BsonDocument))
            model = BsonSerializer.Deserialize<TModel>(document);

        return await ToViewModel((TModel)model, options);
    }

    async Task<bool> IDocumentModel.HasEditableFields(object model)
        => await HasEditableFields((TModel)model);

    public async Task<bool> HasEditableFields(TModel model)
    {
        var context = await CreateContext(model, default);
        var editableContext = EditableDocumentModelContext.FromBaseContext(context, _isNewDocument);

        foreach (var field in (await GetUsedInModelFields(model, context))
            .Where(f => !f.IsServiceField))
        {
            if (await IsEditable(model, field, editableContext))
                return true;
        }
        return false;
    }

    async Task<DocumentModelEditDto> IDocumentModel.PrepareNewModel(object model, DocumentModelEditOptions? options)
    {
        return await PrepareNewModel((TModel)model, options);
    }

    async Task<UpdateModelResult> IDocumentModel.UpdateModel(
        object model,
        DocumentModelUpdateRequest input,
        string? changedField,
        DocumentModelUpdateOptions? options,
        DocumentModelContext? specificContext,
        DateOnly date
    )
    {
        var tModel = (TModel)(model is IConfigurationData cfg ? cfg.To<TModel>() : model);

        return await UpdateModel(tModel, input, changedField, options, specificContext: specificContext, date: date);
    }

    async Task IDocumentModel.LoadDefaults(
        object model,
        DocumentModelUpdateOptions? options,
        DocumentModelContext? dmContext
    )
    {
        await LoadDefaults((TModel)model, options, dmContext);
    }

    async ValueTask<object?> IDocumentModel.GetFieldValueInsecure(
         object document,
         string key,
         IExecuteEngineContext? executionContext,
         bool invokeFormula
     )
    {
        return await GetFieldValueInsecure((TModel)document, key, executionContext, invokeFormula);
    }

    async ValueTask<object?> IDocumentModel.GetFieldValueSecure(
        object document,
        string key,
        IExecuteEngineContext? executionContext,
        bool invokeFormula
    )
    {
        return await GetFieldValueSecure((TModel)document, key, executionContext, invokeFormula);
    }

    public async Task<string?> GetFieldTextSecure(
        TModel document,
        string key,
        IExecuteEngineContext? executionContext = null)
    {
        if (!_prototype.Fields.TryGetValue(key, out var field))
            return null;

        var value = await GetFieldValueSecure(document, key, executionContext);
        if (value is null)
            return null;

        return await GetViewFieldText(
            field, document,
            new DocumentModelViewFieldDto(field)
            {
                Value = value,
                SourceValue = value
            },
            await CreateContext(document, default));
    }

    public async Task<string?> GetFieldTextInsecure(
        TModel document,
        string key,
        IExecuteEngineContext? executionContext = null,
        bool invokeFormula = true)
    {
        if (!_prototype.Fields.TryGetValue(key, out var field))
            return null;

        var value = await GetFieldValueInsecure(document, key, executionContext, invokeFormula);
        if (value is null)
            return null;

        return await GetViewFieldText(
            field, document,
            new DocumentModelViewFieldDto(field)
            {
                Value = value,
                SourceValue = value
            },
            await CreateContext(document, default));
    }

    async Task<DocumentModelEditDto?> IDocumentModel.ToEditModel(
        object? model,
        DocumentModelEditOptions? options,
        DocumentModelContext? context,
        DateOnly date
    )
    {
        if (model == null)
            return null;

        if (model is BsonDocument document && typeof(TModel) != typeof(BsonDocument))
            model = BsonSerializer.Deserialize<TModel>(document);
        else if (model is IConfigurationData data)
            model = data.To<TModel>();

        return await ToEditModel((TModel)model, options, context, date: date);
    }

    #endregion

    public IDocumentModelField? GetField(string key)
    {
        return GetFields(all: true)
            .FirstOrDefault(x => x.Key.Equals(key, StringComparison.InvariantCultureIgnoreCase) ||
                x.KeyAliases.Any(x => key.Equals(x, StringComparison.InvariantCultureIgnoreCase)));
    }

    public IDocumentModelField? GetFieldModel(string key)
    {
        return _prototype.Fields.TryGetValue(key, out var field) ? field : null;
    }

    public IDocumentModelField[] GetFields(Func<IDocumentModelField, bool>? filter = null, bool all = false)
    {
        return _prototype.Fields
            .Values
            .Where(x => x.IsUsedInModel || all)
            .Where(x => filter?.Invoke(x) ?? true)
            .Cast<IDocumentModelField>()
            .ToArray();
    }

    public IEnumerable<DocumentModelGroup> GetGroups() => _prototype.Groups.Select(g => g);

    public async ValueTask<object?> GetFieldValueInsecure(
        TModel document,
        string key,
        IExecuteEngineContext? executionContext = null,
        bool invokeFormula = true
    )
    {
        var field = GetField(key) as DocumentModelField<TModel>;
        if (field == null)
            return null;

        var modelContext = await CreateContext(document, default);
        var actualContext = executionContext ?? modelContext.ExecutionContext;

        if (invokeFormula && field.FormulaExecutor != null)
        {
            var value = await actualContext.InvokeWithinScope(
                GetCacheKey(document, field.Key),
                () => field.FormulaExecutor(
                    document,
                    modelContext,
                    actualContext,
                    document
                )
            );

            return value;
        }

        return field.ValueBinder?.GetValue(document);
    }

    async ValueTask<object?> GetFieldValueSecure(
        TModel? document,
        string key,
        IExecuteEngineContext? executionContext = null,
        bool invokeFormula = true
    )
    {
        if (document == null)
            return null;

        var field = GetField(key) as DocumentModelField<TModel>;
        if (field == null)
            return null;

        if (!await IsAvailable(document, field, await CreateContext(document, default)))
            return null;

        return await GetFieldValueInsecure(document, key, executionContext, invokeFormula);
    }

    public async Task<DocumentModelEditDto> ToEditModel(
        TModel model,
        DocumentModelEditOptions? options = null,
        DocumentModelContext? dmContext = null,
        ObjectId? changeRequest = null,
        DateOnly? date = null
    )
    {
        var update = dmContext is not null;
        var context = dmContext ?? await CreateContext(model, changeRequest: changeRequest, date: date ?? default);
        if (update)
            await InitContext(model, context);

        using var editModelSpan = context
            .StartActivity("ToEditModel")
            .AddTag("dm.model", typeof(TModel).Name);

        options ??= DocumentModelEditOptions.Default;
        var hintMessageProvider = context.Resolve<IHintMessageProvider>();
        var localizationService = context.Resolve<ILocalizationService>();

        List<DocumentModelEditFieldDto> fields = [];
        var modelFields = (await GetUsedInModelFields(model, context, options.KeepNotAvailableFields))
            .Where(f => !f.IsServiceField);


        Func<DocumentModelField<TModel>, ValueTask<DocumentModelFieldModification>>? modification = null;
        if (context.ScopeModification != null)
        {
            // если мы в модифицированном скоупе, то все поля будут иметь запись скоупа.
            modification = _ => ValueTask.FromResult(context.ScopeModification with { Source = null });
        } else if (_prototype.FieldPatchModification != null)
        {
            var source = _prototype.FetchPatchSource != null ? await _prototype.FetchPatchSource(model, context) : null;
            modification = async field => await _prototype.FieldPatchModification(model, field, context, source);
        }

        var discoveredInnerGroups = new List<DocumentModelGroupDto>();

        foreach (var field in modelFields.OrderBy(f => f.Order))
        {
            fields.AddRange(
                await BuildDocumentFieldEditModel(
                    model,
                    field,
                    options,
                    context,
                    update,
                    modification,
                    discoveredInnerGroups
                )
            );
        }

        var modelId = GetModelId(model);
        var documentButtons = await CollectButtons(model, context, options.KeepNotAvailableFields);
        var buttons = await documentButtons
            .Select(button => BuildDocumentEditModelButton(button, model, context, modelId))
            .ToListAsync();

        var modelGroups = GetUsedInModelGroups(fields, options.ExposeAllGroups);// model, context, options.KeepNotAvailableFields);
        var groups = modelGroups.Select(x => BuildDocumentEditModelGroup(x, context)).ToList();

        // Merge discovered inner object groups (with already localized titles) if outer model didn't declare them
        var existingKeys = new HashSet<string>(groups.Select(g => g.Key));
        foreach (var g in discoveredInnerGroups)
        {
            var existing = groups.FirstOrDefault(x => x.Key == g.Key);
            if (existing == null)
            {
                groups.Add(new DocumentModelGroupDto
                {
                    Key = g.Key,
                    Title = g.Title,
                    HintMessage = g.HintMessage
                });
                existingKeys.Add(g.Key);
            }
            else if (string.IsNullOrEmpty(existing.Title) && !string.IsNullOrEmpty(g.Title))
            {
                // enrich empty title with discovered one from inner model
                existing.Title = g.Title;
                if (existing.HintMessage == null && g.HintMessage != null)
                    existing.HintMessage = g.HintMessage;
            }
        }

        var editModel = new DocumentModelEditDto
        {
            Id = modelId,
            Fields = fields,
            Buttons = buttons,
            Groups = groups,
            HintMessage = hintMessageProvider.Render(_prototype.HintMessage)
        };

        editModel.Title = await GetTitle(model, context, editModel) ?? editModel.Title;
        editModel.SubmitButtonText = _prototype.SubmitButtonText;
        editModel.CancelButtonText = _prototype.CancelButtonText;
        if (_prototype.IsFormEditable != null)
            editModel.IsEditable = await _prototype.IsFormEditable(model, context);
        if (_prototype.Alert != null)
            editModel.Alert = await _prototype.Alert(model, context);

        if (options.ForceValidate && _prototype.Validate != null)
        {
            using var validateSpan = context
                .StartActivity("Validate")
                .AddTag("dm.model", typeof(TModel).Name);

            editModel.Validation = context.Resolve<ILocalizationService>().Format(await _prototype.Validate(model, editModel, context));
        }

        ProcessHiddenFields(editModel);

        foreach (var policy in _prototype.ConfigurationSet.DtoPolicies.Values)
            policy.Visit(editModel, context);
        editModel.Tags = DocumentModelFieldTagHelper.BuildFormTags(editModel.Fields, localizationService);
        return editModel;
    }
    async Task<IMultiLangValue?> GetTitle(TModel model, DocumentModelContext context, DocumentModelEditDto editModel)
    {
        if (_prototype.Title != null)
        {
            return _prototype.Title(model, editModel, context);
        }
        if (_prototype.TitleFields.Length != 0)
        {
            var fieldsInDocument = await Task.WhenAll(
                _prototype.TitleFields
                    .Select(key => _prototype.Fields.Values
                        .FirstOrDefault(x => x.Key == key || x.KeyAliases.Contains(key))
                    )
                    .Where(x => x != null)
                    .Select(x => GetFieldText(context, x!, editModel.Fields.FirstOrDefault(f => f.Key == x!.Key)))
            );

            return MultiLanguageName.Join(" • ", fieldsInDocument.Where(t => t.IsNotBlank()));
        }
        return null;
    }

    async Task<DocumentModelButtonDto> BuildDocumentEditModelButton(
        DocumentModelButton<TModel> button,
        TModel model,
        DocumentModelContext context,
        ObjectId modelId
    )
    {
        var localizationService = context.Resolve<ILocalizationService>();
        return new DocumentModelButtonDto
        {
            Key = button.Key,
            Title = button.Title.IsBlank() ? button.Key : localizationService.Format(button.Title),
            Type = button.ButtonType,
            Action = await button.ActionBuilder(model, context, modelId),
            Attributes = button.Attributes
        };
    }

    DocumentModelGroupDto BuildDocumentEditModelGroup(
        DocumentModelGroup group,
        DocumentModelContext context
    )
    {
        var localizationService = context.Resolve<ILocalizationService>();
        var hintMessageProvider = context.Resolve<IHintMessageProvider>();

        var hintMessage = group.HintMessage ??
                          _prototype.NamingPolicies.Values
                              .Select(x => x.GetGroupHint(group))
                              .FirstOrDefault(x => (x?.Message != null) || (x?.Links?.Any() == true));

        return new DocumentModelGroupDto
        {
            Key = group.Key,
            Title = localizationService.Format(group.Title),
            HintMessage = hintMessageProvider.Render(hintMessage)
        };
    }

    void ProcessHiddenFields(DocumentModelEditDto editModel)
    {
        var hideFieldKeys = GetFieldsKeysForHideViewModel();
        foreach (var hideField in hideFieldKeys)
        {
            var field = editModel.Fields.FirstOrDefault(x => x.Key == hideField.Key);
            if (field != null)
            {
                if (hideField.KeepInModel)
                {
                    field.DataType = DocumentFieldInterfaceDataType.Hidden;
                }
                else
                {
                    editModel.Fields.Remove(field);
                }
            }
        }
    }

    async Task InitContext(TModel model, DocumentModelContext context)
    {
        if (_prototype.InitContext != null)
            await _prototype.InitContext(model, context);
    }

    public async Task<DocumentModelCompactDto> ToCompactViewModel(
        TModel model,
        DocumentModelContext? dmContext = null,
        IDocumentModelCompactFieldValuePipeline[]? pipelines = null
    )
    {
        var context = dmContext ?? await CreateContext(model, default);

        var fields = await (await GetUsedInModelFields(model, context))
            .Where(f => !f.IsServiceField)
            .OrderBy(x => x.Order)
            .Select(item => BuildDocumentViewCompactModelField(model, item, context, pipelines))
            .ToListAsync();

        var viewModel = new DocumentModelCompactDto
        {
            Id = GetModelId(model),
            Fields = fields.SelectMany(x => x).ToList(),
            ServiceFields = await GetUsedInModelServiceFields(model)
        };

        return viewModel;
    }

    public async Task<DocumentModelContext> CreateContext(
        TModel model,
        DateOnly date,
        ObjectId? changeRequest = null
    )
    {
        var context = _contextFactory.Create(this, changeRequest, date, scopeObject: model);

        await InitContext(model, context);

        return context;
    }

    async Task UpdateFieldsWithOnChange(
        TModel model,
        List<DocumentModelField<TModel>> fields,
        HashSet<string> changedFieldKeys,
        DocumentModelUpdateOptions? options,
        DocumentModelContext context)
    {
        var fieldsWithOnChange = fields
            .Where(item => item.OnChange != null)
            .ToList();

        fieldsWithOnChange = fieldsWithOnChange.Where(item => changedFieldKeys.Contains(item.Key)).ToList();

        if (fieldsWithOnChange.Count == 0)
            return;

        foreach (var field in fieldsWithOnChange)
        {
            var changedAfterChangeFields = await field.OnChange!(model, context);
            foreach (var changedAfterChangeField in changedAfterChangeFields)
                await ChangeDependentFields(model, changedAfterChangeField, options, context);
        }
    }

    async Task FieldsPostProcess(
        TModel model,
        DocumentModelUpdateRequest input,
        List<DocumentModelField<TModel>> fields,
        DocumentModelUpdateOptions? options,
        DocumentModelContext context
    )
    {
        var fieldsWithPostProcess = fields
            .Where(item => item.PostProcess != null)
            .ToList();

        foreach (var field in fieldsWithPostProcess)
        {
            var raw = input.Fields.FirstOrDefault(f => f.Key == field.Key)?.Value;
            var value = field.ValueBinder.GetValue(model);
            var (shouldUpdate, result) = await field.PostProcess!(raw, value, model, context);

            if (shouldUpdate)
            {
                await UpdateFieldValue(field, model, result, options, context, true);
            }
        }
    }

    async Task UpdateFieldsWithFunctions(
        TModel model,
        DocumentModelUpdateOptions? options,
        DocumentModelContext? context,
        HashSet<string>? changedFieldKeys = null,
        DocumentModelUpdateRequest? input = null,
        DateOnly date = default)
    {
        context ??= await CreateContext(model, date);

        var usedFields = await GetUsedInModelFields(model, context, true);
        var fields = usedFields.ToLookup(x => x.Key, y => y);
        var fieldsWithFunctions = usedFields
            .Where(
                x => x.Function != null &&
                     (x.DependedOn == null ||
                      x.DependedOn.Any(f => fields.Contains(f)) ||
                      x.DependedOn.Length == 0)
            )
            .ToArray();

        foreach (var field in fieldsWithFunctions)
        {
            if (options?.SkipCounters == true && field.ConfigurationDataType == DocumentModelFieldConfigurationType.Counter)
                continue;

            if (options != null && options.IsIntermediary && field.SkipCalculationsWhenIntermediate)
            {
                if (options.ShowCurrentIntermediateValues && input != null)
                {
                    try
                    {
                        var inputValue = input.Fields.FirstOrDefault(f => f.Key == field.Key)?.Value;
                        await UpdateFieldValueFromRequest(field, model, inputValue, options, context);
                    } catch (Exception ex)
                    {
                        throw new ValidationMultiLangException(
                            new MultiLanguageName(
                                $"Произошла ошибка при обновлении поля значением из модели: {ex.Message}."
                            )
                        );
                    }
                }

                continue;
            }

            var value = await field.Function!(model, context);
            try
            {
                await UpdateFieldValue(field, model, value, options, context, false);
                // if (changedFieldKeys != null && !changedFieldKeys.TryGetValue(field.Key, out _))
                // {
                //     changedFieldKeys.Add(field.Key);
                // }
            } catch (Exception ex)
            {
                throw new ValidationMultiLangException(
                    new MultiLanguageName(
                        $"Произошла ошибка при обновлении поля результатом функции: {ex.Message}. Результат: {value}"
                    )
                );
            }
        }
    }

    async Task<object?> CalculateNewItem(
        TModel modelCopy,
        DocumentModelField<TModel> field,
        Type itemType,
        int? objectListChangedIndex,
        string? objectListChangedField,
        IDocumentModel model,
        DocumentModelUpdateOptions options,
        DocumentModelUpdateRequest[] objectListValue,
        int i,
        DocumentModelContext? context)
    {
        object? modelItem = null;
        var fieldValue = modelCopy.GetType().GetProperty(field.Key)?.GetValue(modelCopy);
        if (fieldValue != null)
        {
            var valueList = fieldValue as IList;
            if (valueList?.Count > i)
                modelItem = valueList[i];
        }
        string? changedField = null;
        if (objectListChangedIndex.HasValue && i == objectListChangedIndex)
            changedField = objectListChangedField;

        return await CalculateNewItem(
            itemType,
            model,
            options,
            objectListValue[i],
            modelItem,
            changedField,
            modelCopy,
            context
        );
    }


    async Task<object?> CalculateNewItem(
        Type itemType,
        IDocumentModel model,
        DocumentModelUpdateOptions options,
        DocumentModelUpdateRequest request,
        object? modelItem,
        string? changedField,
        object baseModel,
        DocumentModelContext? context)
    {
        if (modelItem == null)
        {
            modelItem = Activator.CreateInstance(itemType);
            if (baseModel is IContainerCopy copyContainer)
                copyContainer.CopyContainer(modelItem);
        }

        if (modelItem != null && model != null)
        {
            if (options.ValidateFieldExists)
                ValidateRequestFields(request.Fields, model);

            using (context?.PushModel(model))
            {
                await model.Update(
                    modelItem,
                    request,
                    changedField,
                    new DocumentModelUpdateOptions(options)
                    {
                        CalculateFieldChanges = false,
                        CalculateEditModel = false
                    },
                    context
                );
            }
        }

        return modelItem;
    }

    /// <summary>
    /// Обновить значение поля из запроса ModelUpdateRequest
    /// </summary>
    /// <param name="field"></param>
    /// <param name="modelCopy"></param>
    /// <param name="inputValue"></param>
    /// <param name="options"></param>
    /// <param name="context"></param>
    /// <param name="date"></param>
    /// <param name="changedField"></param>
    async Task UpdateFieldValueFromRequest(
        DocumentModelField<TModel> field,
        TModel modelCopy,
        object? inputValue,
        DocumentModelUpdateOptions options,
        DocumentModelContext context,
        string? changedField = null
    )
    {
        var value = inputValue;
        switch (field.DataType)
        {
            // это специальное поле, его не обновляем
            case DocumentFieldInterfaceDataType.Error:
                return;
            // тут отдельная история
            case DocumentFieldInterfaceDataType.ObjectList:
                {
                    int? objectListChangedIndex = null;
                    string? objectListChangedField = null;
                    if (!string.IsNullOrEmpty(changedField))
                    {
                        var match = ObjectListChange.Match(changedField);
                        if (match.Success)
                        {
                            var objectListKey = match.Groups["parentField"].Value;
                            if (objectListKey == field.Key)
                            {
                                objectListChangedIndex = int.Parse(match.Groups["index"].Value);
                                objectListChangedField = match.Groups["field"].Value;
                            }
                        }
                    }

                    DataConverter.TryConvertTo<DocumentModelUpdateRequest[]>(inputValue, out var objectListValue);
                    var (model, itemType) = ExpandFieldModel(field, context, modelCopy);

                    if (objectListValue == null || objectListValue.Length == 0)
                    {
                        var emptyArray = Array.CreateInstance(itemType, 0);
                        field.ValueBinder?.SetValue(modelCopy, emptyArray);
                        return;
                    }

                    var objectValue = new List<object?>();

                    for (var i = 0; i < objectListValue.Length; i++)
                    {
                        objectValue.Add(
                            await CalculateNewItem(
                                modelCopy,
                                field,
                                itemType,
                                objectListChangedIndex,
                                objectListChangedField,
                                model!,
                                options,
                                objectListValue,
                                i,
                                context
                            )
                        );
                    }


                    var valueArray = Array.CreateInstance(itemType, objectValue.Count);
                    objectValue.ToArray().CopyTo(valueArray, 0);

                    value = valueArray;
                    break;
                }

            // тут еще более отдельная история
            case DocumentFieldInterfaceDataType.ComplexCollection:
                value = await UpdateComplexCollection(field, modelCopy, inputValue, options, context, changedField);
                break;

            case DocumentFieldInterfaceDataType.InnerObject:
                {
                    var (model, itemType) = ExpandFieldModel(field, context, modelCopy);

                    DataConverter.TryConvertTo<DocumentModelUpdateRequest>(inputValue, out var request);
                    if (request == null)
                        return;

                    var fieldValue = modelCopy.GetType().GetProperty(field.Key)?.GetValue(modelCopy);
                    value = await CalculateNewItem(itemType, model!, options, request, fieldValue, changedField, modelCopy, context);
                    break;
                }

            case DocumentFieldInterfaceDataType.AttachmentMulti:
            case DocumentFieldInterfaceDataType.Attachment:
                var dto = DataConverter.ConvertTo(typeof(FileAttachmentDto[]), inputValue);
                field.ValueBinder?.SetValue(modelCopy, dto);
                return;
        }
        await UpdateFieldValue(field, modelCopy, value, options, context, false);
    }

    async Task UpdateFieldValue(
        DocumentModelField<TModel> field,
        TModel model,
        object? inputValue,
        DocumentModelUpdateOptions? options,
        DocumentModelContext context,
        bool postProcess
    )
    {
        // это специальное поле, его не обновляем
        if (field.DataType == DocumentFieldInterfaceDataType.Error)
            return;

        if (field.DataType == DocumentFieldInterfaceDataType.Password)
        {
            if (inputValue is string stringValue)
            {
                if (string.Empty == stringValue)
                    return;
                var config = context.Resolve<AppConfiguration>();
                var cryptoHelper = new CryptoHelper(config.Security.EncryptPasswordKey);
                inputValue = cryptoHelper.Encrypt(stringValue);
            }
        }

        var value = context.FieldValueConverter
            .ConvertToDocument(field.DataType, inputValue, field.ValueBinder.FinalType);

        if (!postProcess)
        {
            var hookValue = context.ConvertToDocumentHook(field, value);

            if (hookValue != null)
            {
                value = hookValue;
            }
            else if (field.ConvertToDocument != null &&
                     (options is null ||
                      field.ConfigurationDataType != DocumentModelFieldConfigurationType.DecimalPercent ||
                      options.ConvertPercentValues))
            {
                value = await field.ConvertToDocument(value, model, context);
            }
        }

        field.ValueBinder.SetValue(model, value);
    }

    async Task UpdateComplexObjects(
        TModel model,
        List<DocumentModelField<TModel>> fields,
        DocumentModelUpdateRequest input,
        string? changedField,
        DocumentModelUpdateOptions options,
        DocumentModelContext context,
        HashSet<string> changedFields
    )
    {
        var innerObjectFields = fields
            .Where(x => x.DataType == DocumentFieldInterfaceDataType.InnerObject)
            .ToList();

        foreach (var field in innerObjectFields)
        {
            var (innerModel, type) = ExpandFieldModel(field, context, model);

            if (innerModel == null)
                return;

            var current = field.ValueBinder?.GetValue(model);

            var innerObject = field.InnerObjectFactory(model, context, current) ?? Activator.CreateInstance(type);

            if (innerObject != null)
            {
                var innerInput = input.CopySafe();
                var prefix = field.Key + "_";
                innerInput.Fields = innerInput.Fields
                    .Where(x => x.Key.StartsWith(prefix))
                    .Select(
                        x =>
                        {
                            x.Key = x.Key.Substring(prefix.Length);
                            return x;
                        }
                    )
                    .ToList();

                var innerChangedField = changedField?.StartsWith(prefix) == true
                    ? changedField.Substring(prefix.Length)
                    : changedField;

                var updateResult = await innerModel.Update(
                    innerObject,
                    innerInput,
                    innerChangedField,
                    new DocumentModelUpdateOptions(options)
                    {
                        CalculateFieldChanges = true,
                        CalculateEditModel = false
                    }
                );
                if (updateResult.FieldChanges.Any())
                    changedFields.Add(field.Id);
                field.ValueBinder?.SetValue(model, innerObject);
            }
        }
    }

    async Task<bool> IsEditable(
        TModel model,
        DocumentModelField<TModel> modelField,
        EditableDocumentModelContext context
    )
    {
        bool hasDisallowByModelPolicies = (await _prototype.Policies.Values
                .Select(async policy => await policy.IsFieldEditable(model, modelField, context))
                .ToListAsync())
            .Any(x => !x);

        // если нашлась запрещающая политика, то возвращаем запрет на редактирование, иначе проверяем дальше
        if (hasDisallowByModelPolicies)
            return false;

        var allowedByModel = _prototype.IsFieldEditable?.Invoke(model, context, modelField) ?? true;
        var allowedByField = modelField.IsEditable == null || await modelField.IsEditable(model, context);

        return allowedByModel && allowedByField;
    }
    async Task<bool> IsAvailable(
        TModel model,
        DocumentModelField<TModel> modelField,
        DocumentModelContext context
    )
    {
        var log = LogUtils.GetScoped(nameof(DocumentModel), nameof(IsAvailable));

        var hasDisallowByModelPolicies = (await _prototype.Policies.Values
                .Select(async policy => await policy.IsFieldAvailable(model, modelField, context))
                .ToListAsync())
            .Any(isCanView => !isCanView);

        // если нашлась запрещающая политика, то возвращаем запрет на редактирование, иначе проверяем дальше
        if (hasDisallowByModelPolicies)
        {
            log.Verbose("field {f} access restricted by at least one model policy", modelField.Key);
            return false;
        }

        if (modelField.IsAvailable != null)
            return await modelField.IsAvailable(model, context);

        return true;
    }

    async Task<bool> IsRequired(
        TModel model,
        DocumentModelField<TModel> modelField,
        DocumentModelContext context
    )
    {
        // смотрим на первую политику, в которой определена политика обязательности.
        var requiringByModelPolicies = (await _prototype.Policies.Values
                .Select(async policy => await policy.IsFieldRequired(model, modelField, context))
                .ToListAsync())
            .FirstOrDefault(r => r.HasValue);

        // если нашлась такая, то отдаём её результат, иначе - не задано и проверяем дальше.
        if (requiringByModelPolicies.HasValue)
            return requiringByModelPolicies.Value;

        if (modelField.IsRequired != null)
            return await modelField.IsRequired(model, context);

        return false;
    }

    async Task<bool> CanAccessToValue(
        TModel model,
        DocumentModelField<TModel> modelField,
        DocumentModelContext context
    )
    {
        var canAccessValueByModelPolicies = (await _prototype.Policies.Values
                .Select(async policy => await policy.CanAccessValue(model, modelField, context))
                .ToListAsync())
            .FirstOrDefault(r => r.HasValue);

        if (canAccessValueByModelPolicies.HasValue)
            return canAccessValueByModelPolicies.Value;

        if (modelField.CanAccessValue != null)
            return await modelField.CanAccessValue(model, context);

        //по умолчанию не ограничиваем
        return true;
    }

    async Task<bool> CanCreate(
        TModel model,
        DocumentModelField<TModel> modelField,
        DocumentModelContext context
    )
    {
        var canCreateByModelPolicies = (await _prototype.Policies.Values
                .Select(async policy => await policy.CanCreateValue(model, modelField, context))
                .ToListAsync())
            .FirstOrDefault(r => r.HasValue);

        // если нашлась такая, то отдаём её результат, иначе - не задано и проверяем дальше.
        if (canCreateByModelPolicies.HasValue)
            return canCreateByModelPolicies.Value;

        if (modelField.CanCreate != null)
            return await modelField.CanCreate(context);

        return false;
    }

    async Task<bool> ShowClear(
        TModel model,
        DocumentModelField<TModel> modelField,
        DocumentModelContext context
    )
    {
        if (modelField.ShowClear != null)
            return await modelField.ShowClear(model, context);

        return true;
    }

    async Task<List<DocumentEditFieldValidationDto>> GetFieldValidations(
        TModel model,
        DocumentModelField<TModel> modelField,
        DocumentModelEditFieldDto field,
        DocumentModelEditOptions options,
        DocumentModelContext context,
        ILocalizationService localizationService
    )
    {
        var validators = new List<Func<TModel, DocumentModelContext, IDocumentFieldValidator>>(modelField.Validators);
        if (field.IsRequired)
            validators.Add((_, _) => new RequiredFieldValidator());

        foreach (var validator in validators.Select(f => f(model, context)))
        {
            if (validator.ShouldAdd(field))
            {
                 var result = new DocumentEditFieldValidationDto
                 {
                     Name = validator.Name,
                     Parameters = validator.Parameters,
                     Message = localizationService.Format(validator.Message),
                };
                if (context.UpdatingFields.Contains(modelField.Key) || options.ForceValidate)
                    result.Result = await validator.Validate(field, context, localizationService);
                validationResults.Add(result);
            }
            else if (string.IsNullOrEmpty(field.Error))
            {
                var result = await validator.Validate(field, context, localizationService);
                if (!result.IsValid)
                    field.Error = localizationService.Format(result.Messages.First());
            }
        }

        return validationResults.ToList();
    }

    static async Task<IEnumerable<AvailableValueItemDto>?> GetAvailableValues(
        TModel model,
        DocumentModelContext context,
        DocumentModelField<TModel> modelField
    )
    {
        var log = LogUtils.GetScoped($"{nameof(DocumentModel)}<{typeof(TModel).Name}>.{nameof(GetAvailableValues)}:{modelField.Id}");

        if (modelField.AvailableValuesAsync == null)
            return null;

        using var span = context.StartActivity("GetAvailableValues")
            .AddTag("dm.model", typeof(TModel).Name)
            .AddTag("dm.field", modelField.Key);

        try
        {
            var sw = Stopwatch.StartNew();
            try
            {
                var values = await modelField.AvailableValuesAsync(model, context);
                if (values is not null)
                    return values.ToArray();
            } finally
            {
                sw.Stop();
                if (sw.Elapsed > TimeSpan.FromMilliseconds(100))
                    log.Warning("LONG TIME EXECUTION: {time} sec", sw.Elapsed);
            }

            return null;
        } catch (Exception e)
        {
            log.Error(e, "Ошибка вычисления GetAvailableValues");
            return
            [
                new AvailableValueItemDto(
                    null,
                    ErrorMessagesCore.Common.LoadingError
                )
                {
                    IsDisabled = true
                }
            ];
        }
    }

    async Task<IMultiLangValue?> GetFieldText(
        DocumentModelContext context,
        DocumentModelField<TModel> fieldDefinition,
        DocumentModelEditFieldDto? field
    )
    {
        if (field == null)
            return MultiLanguageName.Empty;
        if (fieldDefinition.Text != null)
            return await fieldDefinition.Text(field.RawValue, context);
        //пока так, если будут более сложные кейсы надо будет подтюнить
        if (field.Value is DocumentModelEditDto[] list && list.Length != 0)
            return string.Join(", ", list.Select(x => x.Fields.FirstOrDefault()?.Text)).AsMultiLang();
        if (field.Value is AvailableValueItemDto fieldValue)
            return fieldValue.Name;
        if (field.Value is AvailableValueItemDto fieldValueData)
            return fieldValueData.Name;
        if (field.Value is AvailableValueItemDto[] fieldValues)
            return MultiLanguageName.Join(", ", fieldValues.Where(x => x.Name != null).Select(x => x.Name!));

        if (field.Value is AvailableValueItemDto[] fieldDataValues)
            return MultiLanguageName.Join(", ", fieldDataValues.Select(x => x.Name));

        if (field.Value is IReference reference)
            return reference.Name.AsMultiLang();

        if (field.Value is IReference[] references)
            return string.Join(", ", references.Select(x => x.Name)).AsMultiLang();

        if (field.Value is IMultiLangValue multiLangValue)
            return multiLangValue;

        var valueFormatter = context.Resolve<IValueFormatter>();

        return context.FieldValueConverter.GetText(field, valueFormatter);
    }

    /// <summary>
    /// Подготовить новую модель. Будут загружены значения по умолчанию и посчитаны вычисляемые поля
    /// </summary>
    /// <param name="model">Модель</param>
    /// <param name="options">Настройки вывода модели</param>
    /// <param name="contextProvider"></param>
    /// <param name="changeRequest"></param>
    /// <returns></returns>
    public async Task<DocumentModelEditDto> PrepareNewModel(
        TModel model,
        DocumentModelEditOptions? options = null,
        Func<EditableDocumentModelContext, Task<EditableDocumentModelContext>>? contextProvider = null,
        ObjectId? changeRequest = null,
        DateOnly date = default
    )
    {
        var context = EditableDocumentModelContext.FromBaseContext(
            await CreateContext(model, date, changeRequest: changeRequest),
            true
        );

        if (contextProvider != null)
            context = await contextProvider(context);
        SetDocumentIsNewFlag();
        await LoadDefaults(model, null, context);
        var updateOptions = new DocumentModelUpdateOptions
        {
            IsIntermediary = true
        };
        await UpdateFieldsWithFunctions(model, updateOptions, context);
        return await ToEditModel(model, options, context);
    }

    /// <summary>
    /// Инициализирует незаполненные поля вычисляемыми значениями по умолчанию
    /// </summary>
    /// <param name="model">Модель</param>
    /// <param name="options"></param>
    /// <param name="dmContext">Контекст DocumentModel</param>
    /// <returns></returns>
    public async Task LoadDefaults(
        TModel model,
        DocumentModelUpdateOptions? options = null,
        DocumentModelContext? dmContext = null
    )
    {
        var context = dmContext ?? await CreateContext(model, default);

        var usedFields = await GetUsedInModelFields(model, context);
        var fields = usedFields.ToLookup(x => x.Key, y => y);

        var fieldsWithDefaults = new List<DocumentModelField<TModel>>();
        foreach (var usedField in usedFields)
        {
            if (usedField.CorrectValue == null && usedField.PredefinedValue == null)
                continue;

            var value = fields[usedField.Key]
                .Select(f => f.ValueBinder.GetValue(model))
                .FirstOrDefault();

            switch (value)
            {
                case null:
                case DateTime d when d == default:
                case string s when string.IsNullOrEmpty(s):
                case Enum:
                case IEnumerable e when !e.Any():
                    fieldsWithDefaults.Add(usedField);
                    break;

            }
        }

        var fieldsWithRecursiveDefaults = usedFields
            .Where(x => x.CustomLoadDefaults != null)
            .ToList();

        if (fieldsWithDefaults.Count + fieldsWithRecursiveDefaults.Count == 0)
            return;

        foreach (var modelField in fieldsWithDefaults)
        {
            object? value = null;
            bool corrected = false;

            if (modelField.PredefinedValue != null)
            {
                value = await modelField.PredefinedValue(model, context);
                corrected = true;
            }

            if (modelField.CorrectValue != null)
            {
                value = await modelField.CorrectValue(model, context);
                corrected = true;
            }

            if (corrected)
                await UpdateFieldValue(modelField, model, value, options, context, false);
        }

        foreach (var field in fieldsWithRecursiveDefaults)
            await field.CustomLoadDefaults!(model, context);
    }

    /// <summary>
    /// Меняет значения зависимых полей
    /// </summary>
    /// <param name="model"></param>
    /// <param name="changedField"></param>
    /// <param name="options"></param>
    /// <param name="dmContext"></param>
    /// <param name="date"></param>
    /// <returns></returns>
    async Task ChangeDependentFields(
        TModel model,
        string? changedField,
        DocumentModelUpdateOptions? options,
        DocumentModelContext? dmContext = null,
        DateOnly date = default)
    {
        if (string.IsNullOrWhiteSpace(changedField))
            return;

        var context = dmContext ?? await CreateContext(model, date);

        var dependentFields = (await GetUsedInModelFields(model, context))
            .Where(
                x => x.DependentValue != null &&
                     x.DependedOn?.Contains(changedField) == true
            )
            .ToList();

        if (dependentFields.Count == 0)
            return;

        foreach (var modelField in dependentFields)
        {
            var value = await modelField.DependentValue!(model, context);
            await UpdateFieldValue(modelField, model, value, options, context, false);
        }
    }

    public async Task ValidateAndThrow(TModel? model, DateOnly date = default)
    {
        ArgumentNullException.ThrowIfNull(model);

        var editModel = await ToEditModel(
            model,
            new DocumentModelEditOptions
            {
                ForceValidate = true
            },
            date: date
        );
        ValidateAndThrow(editModel);
    }

    public async Task BeforeWrite(TModel document, DateOnly? timePoint = null)
    {
        if (_prototype.BeforeWrite != null)
            await _prototype.BeforeWrite(document, await CreateContext(document, timePoint ?? default));
    }

    static void FillErrors(
        DocumentModelEditDto editModel,
        List<IMultiLangValue> errors,
        Dictionary<string, IReadOnlyList<IMultiLangValue?>?> fieldsWithErrors
    )
    {
        foreach (var field in
                 editModel.Fields.Where(f => f.IsAvailable && f.IsEditable)) // TODO: Нужно ли фильтровать?
        {
            if (field.Value is DocumentModelEditDto[] valueEditModelArray)
                foreach (var valueEditModel in valueEditModelArray)
                    FillErrors(valueEditModel, errors, fieldsWithErrors);
            List<IMultiLangValue> validationErrors = field.Validation
                    .Where(x => x.Result != null && x.Result.IsValid == false)
                    .SelectMany(x => x.Result!.Messages)
                    .ToList();
            if (!string.IsNullOrEmpty(field.Error))
                validationErrors.Add(new MultiLanguageName(field.Error));
            if (validationErrors.Count > 0)
            {
                errors.Add($"{field.Title}:{Environment.NewLine}" +
                        MultiLanguageName.Join(Environment.NewLine, validationErrors));
            }
        }
    }

    public IDocumentModelPolicy<TModel>[] GetPolicies()
    {
        return _prototype.Policies.Values.ToArray();
    }
}
