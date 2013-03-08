#include <QApplication>
#include <QClipboard>

#include "smartpointer.h"
#include "util.h"
#include "appcontroller.h"
#include "toolmanager.h"
#include "palettemanager.h"
#include "extension.h"
#include "extensionmanager.h"
#include "canvas.h"
#include "rasterlayer.h"

#include "workspace.h"

namespace PaintField
{

struct Workspace::Data
{
	QList<Canvas *> canvases;
	QPointer<Canvas> currentCanvas;
	
	ToolManager *toolManager = 0;
	PaletteManager *paletteManager = 0;
	
	QActionList actions;
	WorkspaceExtensionList extensions;
	
	QActionList nullCanvasActions;
	CanvasExtensionList nullCanvasModules;
	
	WorkspaceView *view = 0;
};

Workspace::Workspace(QObject *parent) :
    QObject(parent),
    d(new Data)
{
	d->toolManager = new ToolManager(this);
	d->paletteManager = new PaletteManager(this);
	
	d->actions << Util::createAction("paintfield.window.splitVertically", this, SIGNAL(splitVerticallyRequested()));
	d->actions << Util::createAction("paintfield.window.splitHorizontally", this, SIGNAL(splitHorizontallyRequested()));
	d->actions << Util::createAction("paintfield.window.closeCurrentSplit", this, SIGNAL(closeCurrentSplitRequested()));
	
	d->actions << Util::createAction("paintfield.file.new", this, SLOT(newCanvas()));
	d->actions << Util::createAction("paintfield.file.open", this, SLOT(openCanvas()));
	d->actions << Util::createAction("paintfield.file.newFromImageFile", this, SLOT(newCanvasFromImageFile()));
	d->actions << Util::createAction("paintfield.file.newFromClipboard", this, SLOT(newCanvasFromClipboard()));
	
	d->actions << Util::createAction("paintfield.window.closeWorkspace", this, SLOT(tryClose()));
	
	addExtensions(appController()->extensionManager()->createWorkspaceExtensions(this, this));
	addNullCanvasExtensions(appController()->extensionManager()->createCanvasExtensions(0, this));
}

Workspace::~Workspace()
{
	delete d;
}

ToolManager *Workspace::toolManager()
{
	return d->toolManager;
}

PaletteManager *Workspace::paletteManager()
{
	return d->paletteManager;
}

void Workspace::setView(WorkspaceView *view)
{
	d->view = view;
}

WorkspaceView *Workspace::view()
{
	return d->view;
}

void Workspace::addExtensions(const QList<WorkspaceExtension *> &extensions)
{
	for (auto extension : extensions)
		addActions(extension->actions());
	
	d->extensions += extensions;
}

WorkspaceExtensionList Workspace::extensions()
{
	return d->extensions;
}

void Workspace::addActions(const QActionList &actions)
{
	d->actions += actions;
}

QActionList Workspace::actions()
{
	return d->actions;
}

void Workspace::addNullCanvasExtensions(const CanvasExtensionList &extensions)
{
	for (auto extension : extensions)
		addNullCanvasActions(extension->actions());
	
	d->nullCanvasModules += extensions;
}

CanvasExtensionList Workspace::nullCanvasExtensions()
{
	return d->nullCanvasModules;
}

void Workspace::addNullCanvasActions(const QActionList &actions)
{
	d->nullCanvasActions += actions;
}

QActionList Workspace::nullCanvasActions()
{
	return d->nullCanvasActions;
}

void Workspace::newCanvas()
{
	auto canvas = Canvas::fromNew(this);
	if (canvas)
	{
		addAndShowCanvas(canvas);
		setCurrentCanvas(canvas);
	}
}

void Workspace::newCanvasFromImageFile()
{
	auto canvas = Canvas::fromNewFromImageFile(this);
	if (canvas)
	{
		addAndShowCanvas(canvas);
		setCurrentCanvas(canvas);
	}
}

void Workspace::newCanvasFromClipboard()
{
	auto pixmap = qApp->clipboard()->pixmap();
	if (pixmap.isNull())
		return;
	
	auto layer = RasterLayer::createFromImage(pixmap.toImage());
	auto document = new Document(appController()->unduplicatedNewFileTempName(), pixmap.size(), {layer});
	auto canvas = new Canvas(document, this);
	
	addAndShowCanvas(canvas);
	setCurrentCanvas(canvas);
}

void Workspace::openCanvas()
{
	auto canvas = Canvas::fromOpen(this);
	if (canvas)
	{
		addAndShowCanvas(canvas);
		setCurrentCanvas(canvas);
	}
}

void Workspace::openCanvasFromFilepath(const QString &filepath)
{
	auto canvas = Canvas::fromFile(filepath, this);
	if (canvas)
	{
		addAndShowCanvas(canvas);
		setCurrentCanvas(canvas);
	}
}

bool Workspace::tryClose()
{
	for (Canvas *canvas : d->canvases)
	{
		if (!canvas->closeCanvas())
			return false;
	}
	emit shouldBeDeleted(this);
	return true;
}

void Workspace::setFocus()
{
	emit focused();
}

void Workspace::setCurrentCanvas(Canvas *canvas)
{
	if (d->currentCanvas != canvas)
	{
		d->currentCanvas = canvas;
		if (canvas)
			canvas->onSetCurrent();
		
		PAINTFIELD_DEBUG << "current canvas changed:" << canvas;
		emit currentCanvasChanged(canvas);
	}
}

void Workspace::addAndShowCanvas(Canvas *canvas)
{
	addCanvas(canvas);
	emit canvasShowRequested(canvas);
}

void Workspace::addCanvas(Canvas *canvas)
{
	if (!canvas)
		return;
	
	if (!d->canvases.contains(canvas))
	{
		d->canvases << canvas;
		connect(canvas, SIGNAL(shouldBeDeleted(Canvas*)),
		        this, SLOT(deleteCanvas(Canvas*)));
		connect(canvas, SIGNAL(documentPropertyChanged()), this, SLOT(onCanvasDocumentPropertyChanged()));
	}
}

QList<Canvas *> Workspace::canvases()
{
	return d->canvases;
}

Canvas *Workspace::currentCanvas()
{
	return d->currentCanvas;
}

void Workspace::deleteCanvas(Canvas *canvas)
{
	if (d->canvases.contains(canvas))
	{
		if (d->currentCanvas == canvas)
			setCurrentCanvas(0);
		
		d->canvases.removeAll(canvas);
		canvas->deleteLater();
	}
}

QActionList Workspace::currentCanvasActions()
{
	return d->currentCanvas ? d->currentCanvas->actions() : d->nullCanvasActions;
}

CanvasExtensionList Workspace::currentCanvasModules()
{
	return d->currentCanvas ? d->currentCanvas->extensions() : d->nullCanvasModules;
}

void Workspace::onCanvasDocumentPropertyChanged()
{
	auto canvas = qobject_cast<Canvas *>(sender());
	if (canvas && d->canvases.contains(canvas))
	{
		emit canvasDocumentPropertyChanged(canvas);
		
		if (canvas == d->currentCanvas)
			emit currentCanvasDocumentPropertyChanged();
	}
}

}
