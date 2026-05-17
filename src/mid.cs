using System.Collections.Concurrent;
using System.Linq.Expressions;
using ForPeople.Absence.BusinessService;
using ForPeople.Absence.Configuration;
using ForPeople.Absence.Configurator;
using ForPeople.Absence.DbOperations;
using ForPeople.Absence.Documents;
using ForPeople.Absence.Domain.Enums;
using ForPeople.Absence.Domain.Interface;
using ForPeople.Absence.Planing.DbOperations;
using ForPeople.Absence.Security;
using ForPeople.Core;
using ForPeople.Core.Configuration;
using ForPeople.Core.DocumentModel;
using ForPeople.Core.DocumentModel.Contract;
using ForPeople.Core.Exceptions;
using ForPeople.Core.Extensions;
using ForPeople.Core.IoC;
using ForPeople.Core.Localization;
using ForPeople.Core.References;
using ForPeople.Core.Security;
using ForPeople.Domain.Actions.ActionContextFactories;
using ForPeople.Domain.Actions.Interfaces;
using ForPeople.Domain.Admin.AccessRoleMatrix.Services;
using ForPeople.Domain.Configurations;
using ForPeople.Domain.Configurator.Persistence;
using ForPeople.Domain.Data;
using ForPeople.Domain.Data.Documents.Auth;
using ForPeople.Domain.Data.Org.Services;
using ForPeople.Domain.Data.StateEntities;
using ForPeople.Domain.DocumentModels;
using ForPeople.Domain.FileStorage.Interfaces;
using ForPeople.Domain.Managers;
using ForPeople.Domain.Notifications;
using ForPeople.Domain.Security;
using ForPeople.Domain.Security.Access;
using ForPeople.Domain.Security.Access.Scopes;
using ForPeople.Domain.Security.Logs;
using ForPeople.Domain.Security.Navigation;
using ForPeople.Domain.Workflow;
using ForPeople.Domain.Workflow.Configurator.Editors.BusinessProcess.Configurations;
using ForPeople.Domain.Workflow.DistributedTransaction;
using ForPeople.Domain.Workflow.Documents.ChangeRequest;
using ForPeople.Domain.Workflow.Handling;
using MongoDB.Bson;
using MongoDB.Driver;
using MoreLinq;
using Newtonsoft.Json.Linq;
using static ForPeople.Absence.Localization.Constants;

namespace ForPeople.Absence.Domain.Services;

public partial class AbsenceService : IAbsenceService, INotificationByConfigurationService
{
    public const string ApprovedTaskId = "Approved";
    readonly IResolveScope _resolveScope;
    readonly ChangeRequestDbOperations _changeRequestDbOperations;
    readonly IConfigurationItemDocumentHolder _configurationItemDocumentHolder;
    readonly IDocumentModelRegistry _documentModelRegistry;
    readonly IProcessingService _processingService;
    readonly IUserIdentity _userIdentity;
    readonly IUserIdentityCore _userIdentityCore;
    readonly ILocalizationService _localizationService;
    readonly OrgEmployeeDataService _employeeData;
    readonly IUserManager _userManager;
    readonly IAbsenceConfiguration? _clientConfiguration;
    readonly IDateTimeProvider _dateTimeProvider;
    readonly ISecurityProvider _securityProvider;
    readonly AbsenceTelemetry _telemetry;
    readonly AbsenceStateDbOperations _absenceStateDbOperations;
    readonly AbsenceStateDataService _absenceStateDataService;
    readonly AbsenceEditorService _absenceEditorService;
    readonly IConfigurationManagerService _configurationManagerService;
    readonly IAbsenceTransactionsService _transactionsService;

    public AbsenceService(
        IResolveScope resolveScope,
        IConfigurationItemDocumentHolder configurationItemDocumentHolder,
        IDocumentModelRegistry documentModelRegistry,
        IProcessingService processingService,
        IUserIdentity userIdentity,
        ChangeRequestDbOperations changeRequestDbOperations,
        BusinessProcessModelService businessProcessEditor,
        ILocalizationService localizationService,
        OrgEmployeeDataService employeeData,
        OrgItemDataService orgItemData,
        OrgUnitDataService orgUnitData,
        IUserManager userManager,
        IRoleGroupService roleGroupService,
        IClientConfiguration clientConfiguration,
        IScopedDataContext scopedDataContext,
        IDateTimeProvider dateTimeProvider,
        ISecurityLogService securityLogService,
        ISecurityProvider securityProvider,
        IMediaService mediaService,
        AbsenceTelemetry telemetry,
        ChangeRequestDbOperations changeRequestDb,
        AbsenceStateDbOperations absenceStateDbOperations,
        AbsenceStateDataService absenceStateDataService,
        AbsenceDbOperations absenceDbOperations,
        AbsenceEditorService absenceEditorService,
        IUserIdentityCore userIdentityCore,
        IUserRoleNavigationService userRoleNavigationService,
        ThemeConfiguration themeConfiguration,
        IConfigurationManagerService configurationManagerService,
        IValueFormatter valueFormatter,
        IActionComposer actionComposer,
        EntityContextFactoryScope entityContextScope,
        ICurrentTimePointProvider timePointProvider,
        AbsenceEmployeePlanDbOperations absenceEmployeePlanDbOperations,
        AbsenceEntityCatalogService absenceEntityCatalogService,
        IAbsenceBalanceService balanceService,
        IAbsenceTransactionsService transactionsService)
    {
        _resolveScope = resolveScope;
        _configurationItemDocumentHolder = configurationItemDocumentHolder;
        _documentModelRegistry = documentModelRegistry;
        _processingService = processingService;
        _userIdentity = userIdentity;
        _changeRequestDbOperations = changeRequestDbOperations;
        _localizationService = localizationService;
        _employeeData = employeeData;
        _userManager = userManager;
        _dateTimeProvider = dateTimeProvider;
        _telemetry = telemetry;
        _absenceStateDbOperations = absenceStateDbOperations;
        _absenceStateDataService = absenceStateDataService;
        _absenceEditorService = absenceEditorService;
        _userIdentityCore = userIdentityCore;
        _securityProvider = securityProvider;
        _configurationManagerService = configurationManagerService;
        _transactionsService = transactionsService;

        if (clientConfiguration is IAbsenceConfiguration absenceConfiguration)
            _clientConfiguration = absenceConfiguration;
        else
        {
            if (_resolveScope.Resolve<AppConfiguration>().Modules.IsModuleEnabled(AbsenceConfigurationConstant.ModuleName))
            {
                throw new InvalidOperationException(
                    "Client configuration should implement IAbsenceConfiguration in order to use Absence module");
            }
        }
    }

    public AbsenceConfiguration? GetAbsenceType(string? code)
    {
        if (code != null)
        {
            var config = _configurationManagerService.GetConfiguration<AbsenceConfiguration>(code);
            return config;
        }

        return null;
    }

    public List<AbsenceConfiguration> GetAbsenceTypes()
    {
        var configs = _configurationManagerService.GetConfigurations<AbsenceConfiguration>();
        return configs;
    }


    public async Task<bool> IsAvailableAbsenceConfiguration(string absenceCode, ObjectId employeeId)
    {
        return (await GetAvailableAbsenceForEmployee(employeeId, absenceCode)).Any();
    }

    public DocumentModel<AbsenceDocument>? GetAbsenceDocumentModel(string? absenceTypeId, bool throwIfNotExists = true)
    {
        var modelName = AbsenceConfigurationHelper.GetModelName(absenceTypeId);
        if (_documentModelRegistry.TryGetModel<AbsenceDocument>(modelName, out var model))
            return model;
        else if (throwIfNotExists)
            throw new ValidationMultiLangException(Strings.ConfigurationIsNotRegistred, absenceTypeId);
        return null;
    }

    public DocumentModel<AbsenceDocument> GetAbsenceDocumentModelFromRequest(DocumentModelUpdateRequest request)
    {
        var absenceTypeObject = request.GetFieldValue<object>(nameof(AbsenceDocument.AbsenceConfigurationCode));

        string? absenceTypeId;
        if (absenceTypeObject is JObject jObject)
        {
            absenceTypeId = jObject.GetValue("value")?.ToString();
        }
        else
        {
            absenceTypeId = absenceTypeObject?.ToString();
        }

        var modelName = AbsenceConfigurationHelper.GetModelName(absenceTypeId);
        if (!_documentModelRegistry.HasModel(modelName))
            throw new ValidationMultiLangException(Strings.ConfigurationIsNotRegistred, absenceTypeId);
        return _documentModelRegistry.GetModel<AbsenceDocument>(modelName);
    }

    public List<AbsenceDocument> GetAbsences(ObjectId employeeId, int? year = null)
    {
        var editingAbsences = _absenceStateDataService.Find(
                a => a.Employee.Id == employeeId,
                virtualLayer: nameof(AbsenceChangeType.Edit))
            .ToList()
            .Where(x => year == null ||
               (x.StartDate.Year <= year.Value &&
                (x.EndDate?.Year ?? x.StartDate.Year) >= year.Value
               ));
        return editingAbsences.ToList();
    }

    public AbsenceDocument? GetAbsenceById(ObjectId id)
    {
        var result = FindAbsenceAndUpdateStatus(x => x.DocumentId == id).Result.FirstOrDefault();
        return result;
    }
    async Task<BusinessProcessInstance> CreateAbsenceRequest(
        AbsenceDocument absenceDocument,
        IDistributedTransaction? transaction = null,
        string? comment = null,
        DocumentReference? documentReference = null)
    {
        if (!await CanCreateChangeRequest(absenceDocument))
            throw new AccessDeniedException(_userIdentity.User.Login, null);

        if (await HasAnotherActiveChangeRequest(absenceDocument))
            throw new MultiLangException(ForPeople.Absence.Localization.Constants.Messages.ExistAnotherActiveChangeRequest);
        var commit = transaction == null;
        transaction ??= await DistributedTransactionWithCommit.StartWithoutLock(_resolveScope);
        var user = (UserReference)await _resolveScope.Resolve<IUserManager>().GetUserByEmployee(absenceDocument.Employee.Id);
        var absenceType = GetAbsenceType(absenceDocument.AbsenceConfigurationCode);
        var businessProcessInstance = await _processingService.StartInstance(
            ModuleConstants.Absence.BusinessProcessType,
            d =>
            {
                d.Type = ModuleConstants.Absence.BusinessProcessType;
                d.Initiators = initiators;
                d.Name = AbsenceHelper.GetAbsenceChangeRequestName(absenceType?.DisplayName ?? "", user.Name);
                d.EffectiveAt = absenceDocument.StartDate;
            },
            async doc =>
            {
                absenceDocument.ChangeRequestId = doc.Id;
                var result = await _absenceEditorService.CreateNewState(absenceDocument.DocumentId, _dateTimeProvider.Today, absenceDocument,
                    StateReason.Create(_userIdentityCore).WithChangeRequest(doc.Id));
                return result;
            }, transaction, comment);
        if (commit)
            await transaction.Commit();
        return businessProcessInstance;
    }

    async Task<bool> HasAnotherActiveChangeRequest(AbsenceDocument absenceDocument)
    {
        var states = (await _absenceStateDataService.GetStates(absenceDocument.DocumentId)).Select(a => a.ChangeRequestId).ToList();
        return await _changeRequestDbOperations.Find(c => states.Contains(c.Id) && c.State == WorkflowState.Active).AnyAsync();
    }

    public async Task<bool> HasVirtualState(ObjectId documentId, DocumentStateType? currentStateType = null)
    {
        var hasCurrentVirtual = currentStateType == DocumentStateType.Virtual;
        var hasEditVirtualTask = FindAbsence(
            a => a.DocumentId == documentId,
            nameof(AbsenceChangeType.Edit));
        var hasCancelVirtualTask = FindAbsence(
            a => a.DocumentId == documentId,
            nameof(AbsenceChangeType.Cancel));
        await Task.WhenAll(hasEditVirtualTask, hasCancelVirtualTask);

        var hasEditVirtual = hasEditVirtualTask.Result.Any(a => a.StateType == DocumentStateType.Virtual);
        var hasCancelVirtual = hasCancelVirtualTask.Result.Any(a => a.StateType == DocumentStateType.Virtual);

        return hasCurrentVirtual || hasEditVirtual || hasCancelVirtual;
    }


    public async Task<List<AbsenceDocument>> GetAbsenceStates(ObjectId absenceId)
    {
        return await _absenceStateDbOperations
            .Find(x => x.DocumentId == absenceId)
            .ToListAsync();
    }

    public async Task<bool> CanCreateAbsence(AbsenceDocument document)
    {
        bool employeeInParticipationGroups = true;
        if (document.AbsenceConfigurationCode != null)
        {
            employeeInParticipationGroups = await IsAvailableAbsenceConfiguration(document.AbsenceConfigurationCode, document.Employee.Id);
        }
        return employeeInParticipationGroups && await CanExecute(document.Employee.Id, AbsenceDetailedSecurityClaims.CreateAbsence, document.AbsenceConfigurationCode);
    }
    public async Task<bool> CanExecute(ObjectId employeeId, SecurityActionClaim claim, string? absenceCode = null)
    {
        var employee = await _employeeData.GetById(employeeId);

        return _securityProvider.CanExecute(RequiredScope.EmployeeScope(employee), claim, absenceCode);
    }

    public async Task<bool> HasAbsenceByCode(string code)
    {
        var exist = await _absenceStateDataService
            .Find(a => a.AbsenceConfigurationCode == code)
            .AnyAsync();
        return exist;
    }
    public async Task<List<AbsenceDocument>> GetAbsencesByCode(string code)
    {
        var absences = await _absenceStateDataService
            .Find(a => a.AbsenceConfigurationCode == code)
            .ToListAsync();
        return absences;
    }
    public async Task<List<AbsenceDocument>> FindAbsence(Expression<Func<AbsenceDocument, bool>> expression, string virtualLayer = "", IBackgroundJobContext? jobContext = null)
    {
        return await _absenceStateDataService
           .Find(expression, virtualLayer: virtualLayer)
           .ToListAsync();
    }
    public async Task<List<AbsenceDocument>> FindAbsenceAndUpdateStatus(Expression<Func<AbsenceDocument, bool>> expression, string virtualLayer = "", IBackgroundJobContext? jobContext = null)
    {
        var absenceDocuments = await _absenceStateDataService
           .Find(expression, virtualLayer: virtualLayer)
           .ToListAsync();
        return await DetermineStatusAbsence(absenceDocuments, context: jobContext);
    }


    public async Task<List<AbsenceDocument>> DetermineStatusAbsence(List<AbsenceDocument> allAbsences, TransactionSource transactionSource = TransactionSource.System, IBackgroundJobContext? context = null)
    {
        var configurationsDict = GetAbsenceTypes()
            .ToDictionary(x => x.Name, x => x);

        var absenceChangeRequests = await GetAbsenceChangeRequests(allAbsences);
        if (context != null)
            await context.SetProgress(state: $"Обновление отсутствий. Всего {absenceChangeRequests.Count}");

        var absencesToUpdate = new List<AbsenceDocument>();

        foreach (var (absence, request) in absenceChangeRequests.Where(a => configurationsDict.ContainsKey(a.absence.AbsenceConfigurationCode)))
        {
            try
            {
                var oldStatus = absence.Status;
                var newStatusId = GetStatusCode(absence, request);

                if (newStatusId != oldStatus)
                {
                    absence.Status = newStatusId;

                    absencesToUpdate.Add(absence);
                }
            } catch (Exception ex)
            {
                if (context != null)
                    await context.RegisterEvent(
                                BackgroundJobEventLevel.Error,
                                $"Ошибка при обновлении статуса отсутствия {absence.Name} ({absence.DocumentId})",
                                absence.Name,
                                absence.DocumentId, ex);
            }
        }

        if (absencesToUpdate.Any())
        {
            var bulkUpdate = _absenceStateDbOperations.GetBulkUpdateScope();
            foreach (var absence in absencesToUpdate)
            {
                bulkUpdate.Update(absence.VersionId,
                    u => u.Set(x => x.Status, absence.Status)
                );
            }

            await bulkUpdate.Execute();
        }

        return allAbsences;

    }


    AbsenceStatus GetStatusCode(AbsenceDocument absenceDocument, ChangeRequestDocument? requestDocument)
    {
        if (absenceDocument.Status == AbsenceStatus.Cancelled) return AbsenceStatus.Cancelled;

        if (requestDocument?.State == WorkflowState.Draft)
        {
            return AbsenceStatus.Draft;
        }

        if (requestDocument?.State == WorkflowState.Active)
        {
            return AbsenceStatus.InProgress;
        }

        if ((requestDocument == null || requestDocument.State == WorkflowState.Done) && absenceDocument.StartDate > _dateTimeProvider.Today)
        {
            return AbsenceStatus.Approved;
        }

        if ((requestDocument == null || requestDocument.State == WorkflowState.Done) && absenceDocument.StartDate <= _dateTimeProvider.Today && absenceDocument.EndDate != null && absenceDocument.EndDate.Value.AddDays(1) > _dateTimeProvider.Today)
        {
            return AbsenceStatus.Active;
        }

        return AbsenceStatus.Draft;
    }

    public Task<IEnumerable<string>> GetConfigurationCodesWithNotifications()
    {
        var configurations = GetAbsenceTypes();
        return Task.FromResult(configurations.Where(c => c.HasEmailNotifications).Select(c => c.Name));
    }

    public bool ExistWorkflowConditions(
        string absenceConfigurationCode,
        AbsenceChangeType changeType,
        List<BusinessProcessConfiguration> workflows)
    {
        if (!workflows.Any()) return false;
        foreach (var workflow in workflows)
        {
            if (workflow.WorkflowCondition is AbsenceWorkflowConditionConfiguration absenceCondition &&
                absenceCondition.ChangeTypes.Contains(changeType) &&
                (absenceCondition.Configurations.Select(c => c.Code).Contains(absenceConfigurationCode) || absenceCondition.AnyChange))
                return true;
        }

        return false;
    }
    public async Task<bool> ExistWorkflowConditions(string absenceConfigurationCode, AbsenceChangeType changeType)
    {
        var defenition = _resolveScope.Resolve<WorkflowConfigurationContainer>().GetBusinessProcessDefinition(_resolveScope, ModuleConstants.Absence.BusinessProcessType);
        var schemas = defenition?.WorkflowSchemas?.Select(x => x.Configuration?.Id) ?? [];
        var workflows = (await
                _configurationItemDocumentHolder.Get(x => schemas.Contains(x.Id))).Select(c => c.Current?.To<BusinessProcessConfiguration>().Item)
            .ToList();

        return ExistWorkflowConditions(absenceConfigurationCode, changeType, workflows!);
    }
}