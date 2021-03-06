
#include <QPluginLoader>

#include "generaleditaction.h"
#include "util.h"
#include "workspacemanager.h"
#include "extensionmanager.h"
#include "settingsmanager.h"
#include "cursorstack.h"
#include "application.h"
#include "document.h"
#include "canvas.h"
#include "documentreferencemanager.h"
#include "blendmodetexts.h"
#include "formatsupportmanager.h"

#include "appcontroller.h"

namespace PaintField
{

struct AppController::Data
{
	Application *app = 0;
	
	FormatSupportManager *formatSupportManager = nullptr;
	BlendModeTexts *blendModeTexts = nullptr;
	WorkspaceManager *workspaceManager = nullptr;
	ExtensionManager *extensionManager = nullptr;
	SettingsManager *settingsManager = nullptr;
	CursorStack *cursorStack = nullptr;
	DocumentReferenceManager *documentReferenceManager = nullptr;
	
	QList<AppExtension *> extensions;
	QList<QAction *> actions;
};

AppController::AppController(Application *app, QObject *parent) :
	QObject(parent),
    d(new Data)
{
	d->app = app;
	d->formatSupportManager = new FormatSupportManager(this);
	d->blendModeTexts = new BlendModeTexts(this);
	d->workspaceManager = new WorkspaceManager(this);
	d->extensionManager = new ExtensionManager(this);
	d->settingsManager = new SettingsManager(this);
	d->cursorStack = new CursorStack(this);
	d->documentReferenceManager = new DocumentReferenceManager(this);
	
	_instance = this;
	
	declareMenus();
	createActions();
	
	connect(app, SIGNAL(fileOpenRequested(QString)), this, SLOT(openFile(QString)));
}

AppController::~AppController()
{
	delete d;
}

void AppController::begin()
{
	d->settingsManager->loadSettings();
	
	extensionManager()->initialize(this);
	addExtensions(extensionManager()->createAppExtensions(this, this));
	
	workspaceManager()->loadLastWorkspaces();
}

FormatSupportManager *AppController::formatSupportManager() { return d->formatSupportManager; }
BlendModeTexts *AppController::blendModeTexts() { return d->blendModeTexts; }
WorkspaceManager *AppController::workspaceManager() { return d->workspaceManager; }
ExtensionManager *AppController::extensionManager() { return d->extensionManager; }
SettingsManager *AppController::settingsManager() { return d->settingsManager; }
CursorStack *AppController::cursorStack() { return d->cursorStack; }
DocumentReferenceManager *AppController::documentReferenceManager() { return d->documentReferenceManager; }

void AppController::addExtensions(const AppExtensionList &extensions)
{
	for (AppExtension *extension : extensions)
		addActions(extension->actions());
	
	d->extensions += extensions;
}

QList<AppExtension *> AppController::extensions() { return d->extensions; }

void AppController::addActions(const QList<QAction *> &actions)
{
	d->actions += actions;
}

QList<QAction *> AppController::actions() { return d->actions; }

QString AppController::unduplicatedFileTempName(const QString &name)
{
	QStringList existingTempNames;
	
	for (Workspace *workspace : workspaceManager()->workspaces())
	{
		for (Canvas *canvas : workspace->canvases())
		{
			Document *eachDoc = canvas->document();
			
			if (eachDoc->filePath().isEmpty())
				existingTempNames << eachDoc->tempName();
		}
	}
	
	return Util::unduplicatedName(existingTempNames, name);
}

Application *AppController::app()
{
	return d->app;
}

void AppController::minimizeCurrentWindow()
{
	QWidget *widget = qApp->activeWindow();
	if (widget)
		widget->showMinimized();
}

void AppController::zoomCurrentWindow()
{
	QWidget *widget = qApp->activeWindow();
	if (widget)
		widget->showMaximized();
}

Canvas *AppController::findCanvasWithFilepath(const QString &filepath)
{
	QFileInfo info(filepath);
	
	if (!info.exists())
	{
		PAINTFIELD_WARNING << "file does not exist";
		return 0;
	}
	
	// search for existing canvas that already opens the file
	for (auto workspace : workspaceManager()->workspaces())
	{
		for (auto canvas : workspace->canvases())
		{
			if (canvas->document()->filePath() == info.canonicalFilePath())
				return canvas;
		}
	}
	
	return 0;
}

void AppController::setCurrentCanvas(Canvas *canvas)
{
	auto workspace = canvas->workspace();
	workspaceManager()->setCurrentWorkspace(workspace);
	workspace->setCurrentCanvas(canvas);
}

void AppController::openFile(const QString &path)
{
	if (path.isEmpty())
		return;
	
	auto canvas = findCanvasWithFilepath(path);
	
	if (canvas)
	{
		setCurrentCanvas(canvas);
	}
	else
	{
		auto workspace = workspaceManager()->currentWorkspace();
		
		if (workspace)
			workspace->openCanvasFromFilepath(path);
	}
}

void AppController::quit()
{
	d->settingsManager->saveUserSettings();
	d->app->quit();
}

void AppController::declareMenus()
{
	settingsManager()->declareMenu("paintfield.file",
	                               tr("File"));
	
	settingsManager()->declareAction("paintfield.file.new",
	                                 tr("New..."));

	settingsManager()->declareAction("paintfield.file.newFromImageFile",
									 tr("Import and New..."));

	settingsManager()->declareAction("paintfield.file.newFromClipboard",
	                                 tr("New from Clipboard"));
	
	settingsManager()->declareAction("paintfield.file.open",
	                                 tr("Open..."));
	settingsManager()->declareAction("paintfield.file.close",
	                                 tr("Close"));
	settingsManager()->declareAction("paintfield.file.save",
	                                 tr("Save"));
	settingsManager()->declareAction("paintfield.file.saveAs",
	                                 tr("Save As..."));
	settingsManager()->declareAction("paintfield.file.newCanvasIntoDocument",
	                                 tr("New Canvas Into Current Document"));
	settingsManager()->declareAction("paintfield.file.export",
	                                 tr("Export..."));
	settingsManager()->declareAction("paintfield.file.quit",
	                                 tr("Quit"));
	
	settingsManager()->declareMenu("paintfield.edit",
	                               tr("Edit"));
	
	settingsManager()->declareAction("paintfield.edit.undo",
	                                 tr("Undo"));
	settingsManager()->declareAction("paintfield.edit.redo",
	                                 tr("Redo"));
	settingsManager()->declareAction("paintfield.edit.cut",
	                                 tr("Cut"));
	settingsManager()->declareAction("paintfield.edit.copy",
	                                 tr("Copy"));
	settingsManager()->declareAction("paintfield.edit.paste",
	                                 tr("Paste"));
	settingsManager()->declareAction("paintfield.edit.delete",
	                                 tr("Delete"));
	settingsManager()->declareAction("paintfield.edit.selectAll",
	                                 tr("Select All"));
	
	settingsManager()->declareMenu("paintfield.view",
	                               tr("View"));
	
	settingsManager()->declareAction("paintfield.window.newWorkspace",
	                                 tr("New Workspace"));
	settingsManager()->declareAction("paintfield.window.splitVertically",
	                                 tr("Split Vertically"));
	settingsManager()->declareAction("paintfield.window.splitHorizontally",
	                                 tr("Split Horizontally"));
	settingsManager()->declareAction("paintfield.window.closeCurrentSplit",
	                                 tr("Close Current Split"));
	settingsManager()->declareAction("paintfield.window.closeWorkspace",
	                                 tr("Close Workspace"));
	
	settingsManager()->declareMenu("paintfield.window",
	                               tr("Window"));
	
	settingsManager()->declareAction("paintfield.window.minimize",
	                                 tr("Minimize"));
	settingsManager()->declareAction("paintfield.window.zoom",
	                                 tr("Zoom"));
	
	settingsManager()->declareMenu("paintfield.help",
	                               tr("Help"));
}

void AppController::createActions()
{
	d->actions << Util::createAction("paintfield.file.quit", d->workspaceManager, SLOT(closeAllAndQuit()));
	d->actions << Util::createAction("paintfield.window.minimize", this, SLOT(minimizeCurrentWindow()));
	d->actions << Util::createAction("paintfield.window.zoom", this, SLOT(zoomCurrentWindow()));
	d->actions << Util::createAction("paintfield.window.newWorkspace", d->workspaceManager, SLOT(newWorkspace()));
	
	d->actions << new GeneralEditAction("paintfield.edit.cut", this);
	d->actions << new GeneralEditAction("paintfield.edit.copy", this);
	d->actions << new GeneralEditAction("paintfield.edit.paste", this);
	d->actions << new GeneralEditAction("paintfield.edit.delete", this);
	d->actions << new GeneralEditAction("paintfield.edit.selectAll", this);
}


AppController *AppController::_instance = 0;


}
