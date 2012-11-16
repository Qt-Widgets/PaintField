#ifndef WORKSPACEVIEW_H
#define WORKSPACEVIEW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QAction>
#include <QPointer>
#include <QVBoxLayout>

#include "global.h"
#include "canvasview.h"
#include "widgets/floatingdocktabwidget.h"
#include "widgets/docktabmotherwidget.h"

namespace PaintField
{

class WorkspaceView;

class SidebarTabWidget : public FloatingDockTabWidget
{
	Q_OBJECT
	Q_INTERFACES(PaintField::ReproductiveInterface)
	
public:
	
	SidebarTabWidget(WorkspaceView *workspaceView, QWidget *parent);
	SidebarTabWidget(SidebarTabWidget *other, QWidget *parent);
	
	bool isInsertableFrom(DockTabWidget *other) override;
	QObject *createNew() override;
	
private:
	
	void commonInit();
	
	WorkspaceView *_workspaceView;
};

class WorkspaceMotherWidget : public DockTabMotherWidget
{
	Q_OBJECT
	Q_INTERFACES(PaintField::DockTabDroppableInterface)
	
public:
	
	explicit WorkspaceMotherWidget(QWidget *parent = 0) : DockTabMotherWidget(parent) {}
	
	bool isInsertableFrom(DockTabWidget *tabWidget) override;
};

class WorkspaceMenuAction : public QAction
{
	Q_OBJECT
public:
	
	WorkspaceMenuAction(QObject *parent) : QAction(parent) {}
	
	void setBackendAction(QAction *action);
	QAction *backendAction() { return _backendAction; }
	
private slots:
	
	void onBackendActionChanged();
	
private:
	
	QPointer<QAction> _backendAction = 0;
};

class DockTabMotherWidget;
class WorkspaceTabWidget;

class SideBarFrame : public QWidget
{
	Q_OBJECT
	
public:
	
	SideBarFrame(QWidget *parent = 0);
	
	void setSideBar(QWidget *sideBar);
	
private:
	
	QVBoxLayout *_layout = 0;
	QWidget *_sideBar = 0;
};

class WorkspaceView : public QMainWindow
{
	Q_OBJECT
public:
	explicit WorkspaceView(QWidget *parent = 0);
	
	void createSideBarFrames(const SideBarDeclarationHash &sidebarDeclarations, const QVariant &order);
	void createToolBars(const ToolBarDeclarationHash &toolBarDeclarations, const QVariant &order);
	void createMenuBar(const ActionDeclarationHash &actionDeclarations, const MenuDeclarationHash &menuDeclarations,  const QVariant &order);
	
	void setCentralWidget(QWidget *widget);
	
	void setSidebar(const QString &id, QWidget *sidebar);
	QToolBar *toolBar(const QString &id);
	void associateMenuBarWithActions(const QActionList &actions);
	
	void registerTabWidget(WorkspaceTabWidget *tabWidget) { _tabWidgets << tabWidget; }
	
signals:
	
	void focusChanged(bool x);
	void closeRequested();
	
public slots:
	
protected:
	
	void closeEvent(QCloseEvent *event);
	
	void focusInEvent(QFocusEvent *);
	void focusOutEvent(QFocusEvent *);
	
private:
	
	void createSideBarFramesInArea(DockTabMotherWidget::Direction splitterDir, const SideBarDeclarationHash &sidebarDeclarations, const QVariant &areaOrder);
	void createSideBarFramesInSplitter(DockTabMotherWidget::Direction splitterDir, int splitterIndex, const SideBarDeclarationHash &sidebarDeclarations, const QVariant &splitterOrder);
	void createToolBarsInArea(Qt::ToolBarArea area, const ToolBarDeclarationHash &toolBarDeclarations, const QVariant &areaOrder);
	
	DockTabMotherWidget *_motherWidget = 0;
	
	QList<WorkspaceTabWidget *> _tabWidgets;
	QList<SideBarFrame *> _sideBarFrames;
	QList<QToolBar *> _toolBars;
};

}

#endif // WORKSPACEVIEW_H
