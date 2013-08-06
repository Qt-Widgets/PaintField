#include <QtCore>
#include <QDesktopServices>
#include "util.h"
#include "json.h"

#include "settingsmanager.h"

namespace PaintField {

struct SettingsManager::Data
{
	QVariantMap settings;
	QVariantMap userSettings;
	
	QVariant menuBarOrder, workspaceItemOrder;
	
	QHash<QString, ToolInfo> toolDeclarationHash;
	QHash<QString, ActionInfo> actionDeclarationHash;
	QHash<QString, SideBarInfo> sideBarDeclarationHash;
	QHash<QString, ToolBarInfo> toolbarInfoHash;
	QHash<QString, MenuInfo> menuDeclarationHash;
	
	QString lastFileDialogPath;
	
	QKeySequence findKeyBinding(const QString &actionId)
	{
		QVariantMap keyBindMap = settings[".key-bindings"].toMap();
		
		for (auto keyIter = keyBindMap.begin(); keyIter != keyBindMap.end(); ++keyIter)
		{
			if (keyIter.key() == actionId)
				return keyIter.value().toString();
		}
		return QKeySequence();
	}
	
	void applyKeyBindingsToActionDeclarations()
	{
		for (auto actionIter = actionDeclarationHash.begin(); actionIter != actionDeclarationHash.end(); ++actionIter)
			actionIter->shortcut = findKeyBinding(actionIter.key());
	}
};

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent),
    d(new Data)
{
	// prepare directiries
	
	auto documentPath = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
	QDir dir(documentPath);
	
	dir.mkdir("PaintField");
	dir.cd("PaintField");
	dir.mkdir("Settings");
	
	d->lastFileDialogPath = value({"last-file-dialog-path"}).toString();
	
	if (d->lastFileDialogPath.isEmpty())
		d->lastFileDialogPath = documentPath;
}

SettingsManager::~SettingsManager()
{
	delete d;
}

static const QString menuBarOrderFileName("menubar.json");
static const QString WorkspaceItemOrderFileName("workspace-items.json");
static const QString keyBindingFileName("key-bindings.json");

static QVariantMap joinSettings(const QVariantMap &dst, const QVariantMap &src)
{
	QVariantMap result = dst;
	
	for (auto iter = src.begin(); iter != src.end(); ++iter)
	{
		auto key = iter.key();
		auto value = iter.value();
		
		if (key.startsWith(".") && dst.contains(key)) // namespace
			result[key] = joinSettings(dst[key].toMap(), value.toMap());
		else
			result[key] = value;
	}
	
	return result;
}

void SettingsManager::loadSettings()
{
	auto loadSettingsFromDataDir = [](const QString &dirPath)->QVariantMap
	{
		QDir dir(dirPath);
		
		if (!dir.exists())
			return QVariantMap();
		
		if (!dir.cd("Settings"))
			return QVariantMap();
		
		const auto settings = Json::readFromFile(dir.filePath("settings.json")).toMap();
		const auto platformSpecificSettings = settings.value("platform-specific").toMap()[Util::platformName()].toMap();
		
		if (platformSpecificSettings.size())
			return joinSettings(settings, platformSpecificSettings);
		else
			return settings;
	};
	
	auto builtinSettings = loadSettingsFromDataDir(builtinDataDir());
	auto userSettings = loadSettingsFromDataDir(userDataDir());
	d->settings = joinSettings(builtinSettings, userSettings);
	d->userSettings = userSettings;
	
	d->applyKeyBindingsToActionDeclarations();
}

void SettingsManager::saveUserSettings()
{
	QDir dir(userDataDir());
	
	if (!dir.exists())
		return;
	
	if (!dir.cd("Settings"))
		return;
	
	Json::writeIntoFile(dir.filePath("settings.json"), d->userSettings);
}

static QVariant valueFromMapTree(const QVariantMap &original, const QStringList &path, const QVariant &defaultValue = QVariant())
{
	auto map = original;
	QVariant data;
	
	for (auto key : path)
	{
		if (!map.contains(key))
			return defaultValue;
		
		data = map[key];
		map = data.toMap();
	}
	
	return data;
}

static void setValueToMapTree(QVariantMap &original, const QStringList &path, const QVariant &value)
{
	auto last = path.last();
	auto init = path;
	init.removeLast();
	
	if (init.size() == 0)
	{
		original[last] = value;
	}
	else
	{
		auto map = valueFromMapTree(original, init).toMap();
		map[last] = value;
		setValueToMapTree(original, init, map);
	}
}

void SettingsManager::setValue(const QStringList &path, const QVariant &value)
{
	setValueToMapTree(d->settings, path, value);
	setValueToMapTree(d->userSettings, path, value);
}

QVariant SettingsManager::value(const QStringList &path, const QVariant &defaultValue)
{
	return valueFromMapTree(d->settings, path, defaultValue);
}

void SettingsManager::declareTool(const QString &name, const ToolInfo &info)
{
	d->toolDeclarationHash[name] = info;
}

void SettingsManager::declareAction(const QString &name, const ActionInfo &info)
{
	auto actualInfo = info;
	actualInfo.shortcut = d->findKeyBinding(name);
	d->actionDeclarationHash[name] = actualInfo;
}

void SettingsManager::declareSideBar(const QString &name, const SideBarInfo &info)
{
	d->sideBarDeclarationHash[name] = info;
}

void SettingsManager::declareToolbar(const QString &name, const ToolBarInfo &info)
{
	d->toolbarInfoHash[name] = info;
}

void SettingsManager::declareMenu(const QString &id, const MenuInfo &info)
{
	d->menuDeclarationHash[id] = info;
}

void SettingsManager::declareTool(const QString &name, const QString &text, const QIcon &icon, const QStringList &supportedLayerTypes)
{
	declareTool(name, ToolInfo(text, icon, supportedLayerTypes));
}

void SettingsManager::declareAction(const QString &name, const QString &text, const QKeySequence &defaultShortcut)
{
	declareAction(name, ActionInfo(text, defaultShortcut));
}

QHash<QString, ToolInfo> SettingsManager::toolInfoHash() const { return d->toolDeclarationHash; }
QHash<QString, ActionInfo> SettingsManager::actionInfoHash() const { return d->actionDeclarationHash; }
QHash<QString, SideBarInfo> SettingsManager::sideBarInfoHash() const { return d->sideBarDeclarationHash; }
QHash<QString, ToolBarInfo> SettingsManager::toolBarInfoHash() const { return d->toolbarInfoHash; }
QHash<QString, MenuInfo> SettingsManager::menuInfoHash() const { return d->menuDeclarationHash; }

QStringList SettingsManager::toolNames() const { return d->toolDeclarationHash.keys(); }
QStringList SettingsManager::actionNames() const { return d->actionDeclarationHash.keys(); }
QStringList SettingsManager::sidebarNames() const { return d->sideBarDeclarationHash.keys(); }
QStringList SettingsManager::toolbarNames() const { return d->toolbarInfoHash.keys(); }
QStringList SettingsManager::menuNames() const { return d->menuDeclarationHash.keys(); }

QString SettingsManager::builtinDataDir() const
{
	return QDir(qApp->applicationDirPath()).absolutePath();
}

QString SettingsManager::userDataDir() const
{
	return QDir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first()).filePath("PaintField");
}

QString SettingsManager::lastFileDialogPath() const { return d->lastFileDialogPath; }
void SettingsManager::setLastFileDialogPath(const QString &path)
{
	setValue({"last-file-dialog-path"}, path);
	d->lastFileDialogPath = path;
}

} // namespace PaintField
