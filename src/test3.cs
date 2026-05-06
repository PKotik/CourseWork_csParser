#nullable enable
using ForPeople.Absence.Intersections.Configuration;
using ForPeople.Core.MongoDb.Migration;
using ForPeople.Domain.Configurator;
using ForPeople.Domain.Configurator.Persistence;
using MongoDB.Driver;

namespace ForPeople.Absence.Intersections.Migrations;

/// <summary>
/// Миграция для инициализации конфигурации Фильтров в конфигурации Пересечений отсутствий
/// </summary>
public class AbsenceFilterInitialConfigurationMigration(
    IConfigurationItemDocumentHolder itemDocumentHolder,
    ConfigurationItemDbProvider configurationItemDbProvider)
    : DbMigrationBase
{
    protected override DateTime Version => new DateTime(2026, 03, 12);

    public override async Task Execute()
    {
        var config = (await itemDocumentHolder.GetActiveConfigurationByType(ConfigurationItemType.AbsenceIntersection)).FirstOrDefault();
        if (config == null)
            return;

        var current = config.Current?.To<AbsenceFilterConfigurationItemData>();
        var draft = config.Draft?.To<AbsenceFilterConfigurationItemData>();

        if (InitialConfigItem(current) | InitialConfigItem(draft))
            await configurationItemDbProvider.DbOperations.Update(config);
    }

    static bool InitialConfigItem(ConfigurationData<AbsenceFilterConfigurationItemData>? config)
    {
        if (config != null)
        {
            config.Item ??= new AbsenceFilterConfigurationItemData();
            return true;
        }
        return false;
    }
}