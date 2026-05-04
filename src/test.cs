using ForPeople.Absence.Domain.Interface;
using ForPeople.Absence.Domain.Models.Calendar;
using ForPeople.Absence.Intersections.Configuration;
using ForPeople.Absence.Planing.Interfaces;
using ForPeople.Absence.Security;
using ForPeople.Core;
using ForPeople.Core.DocumentModel;
using ForPeople.Core.DocumentModel.Contract;
using ForPeople.Core.Extensions;
using ForPeople.Core.IoC;
using ForPeople.Core.Security;
using ForPeople.Domain.Configurations;
using ForPeople.Domain.Configurations.FieldConfigurations;
using ForPeople.Domain.Configurator;
using ForPeople.Domain.Configurator.DocumentModel.DataTypes;
using ForPeople.Domain.Data;
using ForPeople.Domain.DocumentModels;
using ForPeople.Domain.Security.Access;
using ForPeople.Domain.Security.Access.Scopes;

namespace ForPeople.Absence.Initializers;

public partial class AbsenceModuleInitializer
{
    Task AbsenceIntersectionsInitializer(IResolveScope scope)
    {
        configurationItemService.EnableConfigurationItem(ConfigurationItemType.AbsenceIntersection);

        var configurationManager = scope.Resolve<IConfigurationManagerService>();
        var configurationEntitiesHelper = scope.Resolve<ConfigurationEntitiesHelper>();
        var customFiltersConfig =  configurationManager.GetSingleOrDefaultConfiguration<AbsenceFiltersConfiguration>();
        
        documentModelConfiguration.ConfigureModel<AbsenceFilters>(nameof(AbsenceFilters), b =>
        {
            b.Map(x => x.Statuses)
                .DataType(DocumentFieldInterfaceDataType.DropDownMulti)
                .Placeholder(Localization.Constants.AbsenceIntersections.Statuses)
                .AvailableValues(async (model, context) =>
                {
                    var year = model.Year ?? context.Resolve<IDateTimeProvider>().UtcNow.Year;
                    return await context.Resolve<IAbsenceCalendarService>().GetAvailableStatuses(model.EmployeeId, year);
                });

            b.Map(x => x.AbsenceTypes)
                .DataType(DocumentFieldInterfaceDataType.DropDownMulti)
                .Placeholder(Localization.Constants.AbsenceIntersections.AbsenceTypes)
                .AvailableValues(async (model, context) =>
                {
                    var year = model.Year ?? context.Resolve<IDateTimeProvider>().UtcNow.Year;
                    return await context.Resolve<IAbsenceCalendarService>().GetAvailableAbsenceTypes(model.EmployeeId, year);
                });

            b.Map(x => x.Employees)
                .DataType(DocumentFieldInterfaceDataType.DropDownMulti)
                .FromEmployee(getAvailableValuesUrl: (_, _) => Task.FromResult(string.Empty))
                .AvailableValues(async (doc, context) =>
                {
                    var employeeIds =
                        (await context.Resolve<IAbsenceCalendarService>().GetEmployeeIds(doc.IntersectionTypes, doc.EmployeeId!.Value));
                    if (doc.PlanningId != null)
                    {
                        var planning = await context.Resolve<IAbsencePlanningService>().GetDocumentById(doc.PlanningId.Value);
                        var populationIds = planning.Population.Select(e => e.Id);
                        employeeIds = employeeIds.Where(id => populationIds.Contains(id)).ToList();
                    }
                    var employee = await context.Resolve<IScopedDataContext>().GetEmployeeList(employeeIds);

                    return employee.Select(x => new AvailableValueItemDto(x.DocumentId, x.Name)).ToArray();
                })
                .Placeholder(Localization.Constants.AbsenceIntersections.EmployeeName)
                .IsAvailable(model => model.From != AbsenceFilters.FromCalendar);

            b.Map(x => x.IntersectionTypes)
                .DataType(DocumentFieldInterfaceDataType.DropDown)
                .IsAffectingDocumentStructure()
                .Placeholder(Localization.Constants.AbsenceIntersections.IntersectionTypes)
                .ShowClear((_, _) => true)
                .AvailableValues(async (model, context) => await context.Resolve<IAbsenceCalendarService>().GetAvailableValuesIntersectionTypes(model.EmployeeId))
                .InitialValue(async (model, context) =>
                {
                    var types = await context.Resolve<IAbsenceCalendarService>()
                        .GetAvailableValuesIntersectionTypes(model.EmployeeId);
                    return types.FirstOrDefault()?.Value;
                })
                .IsAvailable(async (model, context) =>
                {
                    if (model.From == AbsenceFilters.FromCalendar)
                        return false;

                    var types = await context.Resolve<IAbsenceCalendarService>()
                        .GetAvailableValuesIntersectionTypes(model.EmployeeId);
                    return types.Any();
                });

            b.Map(x => x.IncludeExistingAbsences)
                .DataType(DocumentFieldInterfaceDataType.Boolean)
                .CorrectValue(false)
                .IsAvailable(x => x.PlanningId != null)
                .Title(Localization.Constants.AbsenceIntersections.IncludeExistingAbsences);

            b.Map(x => x.RequiresApproval)
                .DataType(DocumentFieldInterfaceDataType.Boolean)
                .CorrectValue(false)
                .IsAvailable(x => x.PlanningId != null)
                .Title(Localization.Constants.AbsenceIntersections.RequiresApproval);

            b.Map(x => x.Year)
                .IsAvailable((model, context) => false);

            b.Map(x => x.From)
                .IsAvailable((model, context) => false);


            // Кастомные фильтры
            var typeFactory = scope.Resolve<ConfiguratorDataTypeFactory>();
            Dictionary <ViewLineDataSource, IEnumerable<FieldConfigurationBase>> sourceFields = [];
            foreach (var filter in customFiltersConfig.Filters.OrderByDescending(f => f.Weight))
            {
                if (filter?.Field?.Type == null || filter.Field.Id.IsNullOrEmpty() || filter?.Name == null)
                    continue;
                var fieldBuilder = b.Map(x => x.CustomFilters, filter.Code);
                switch (filter.FilterSource)
                {
                    case FilterSource.UserGroupSource:
                        fieldBuilder = fieldBuilder
                            .ConfigurationType(DocumentModelFieldConfigurationType.String)
                            .DataType(DocumentFieldInterfaceDataType.String);
                        break;
                    case FilterSource.OrgUnit:
                    case FilterSource.EmployeeSource:
                        if (filter.Field.Type == ViewLineDataSource.None)
                            throw new InvalidOperationException();
                        var fields = sourceFields.GetOrAdd(filter.Field.Type.Value, 
                            () => configurationEntitiesHelper.CollectFields(f => f, source: filter.Field.Type));
                        var fieldConfig = fields.FirstOrDefault(i => i.Id == filter.Field.Id);
                        if (fieldConfig == null) continue;
                        fieldBuilder = PatchControlForFilterView(typeFactory, fieldConfig, fieldBuilder).ConfigurationType(fieldConfig.DataType); ;
                        break;
                    default:
                        throw new NotImplementedException();

                }
                var field = fieldBuilder
                    .Placeholder(filter.Name)
                    .Title(filter.Name)
                    .IsAvailable((model, context) =>
                    {
                        var claim = new SecurityActionClaim(
                            AbsenceCommonSecurityClaims.ModuleName,
                            filter.Code,
                            RequiredScopeType.Generic,
                            filter.Name!);
                        return model.From != AbsenceFilters.FromCalendar
                            && context.Resolve<ISecurityProvider>().CanExecute(RequiredScope.GenericScope(), claim);
                    });
            }

        });

        return Task.CompletedTask;
    }

    private static DocumentModelFieldBuilder<AbsenceFilters> PatchControlForFilterView(ConfiguratorDataTypeFactory typeFactory, FieldConfigurationBase fieldConfig, DocumentModelFieldBuilder<AbsenceFilters> fieldBuilder)
    {
        switch (fieldConfig.DataType)
        {
            case DocumentModelFieldConfigurationType.Boolean:
                fieldBuilder = fieldBuilder.Control(new BooleanConfiguratorDataType(true));
                break;
            default:
                var control = fieldConfig.Control.Copy() ?? typeFactory.Create(fieldConfig);
                if (control.SupportsMultiValue && control is MultiValueConfiguratorDataType multiValueType)
                {
                    multiValueType.IsMultiValue = true;
                    fieldBuilder = fieldBuilder.IsMultiValue(true);
                }
                if (fieldConfig.Control != null)
                    fieldBuilder = fieldBuilder.Control(control);
                break;
        }
        return fieldBuilder;
    }
}