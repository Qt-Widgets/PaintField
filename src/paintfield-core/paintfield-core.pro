######################################################################
# Automatically generated by qmake (2.01a) ? 10? 28 13:02:30 2012
######################################################################

TEMPLATE = lib
TARGET = paintfield-core

include(../src.pri)

QMAKE_CXXFLAGS += -std=c++11

mac {
  #QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/
}

INCLUDEPATH += $$PWD $$PWD/../libs $$PWD/../libs/Malachite/include

LIBS += -L$$OUT_PWD/../libs/Malachite/src -L$$OUT_PWD/../libs/Minizip
LIBS += -lfreeimage -lz -lmalachite -lminizip -lqjson

# Input
HEADERS += \
           canvascontroller.h \
           document.h \
           documentio.h \
           drawutil.h \
           global.h \
           layer.h \
           layeredit.h \
           layermodel.h \
           layerrenderer.h \
           module.h \
           modulemanager.h \
           palettemanager.h \
           randomstring.h \
           tabletevent.h \
           tabletinputdata.h \
           thumbnail.h \
           tool.h \
           toolmanager.h \
           util.h \
           workspacecontroller.h \
           workspacemanager.h \
           workspaceview.h \
           dialogs/exportdialog.h \
           dialogs/newdocumentdialog.h \
           internal/layermodelcommand.h \
           widgets/colorbutton.h \
           widgets/colorslider.h \
           widgets/colorwheel.h \
           widgets/doubleslider.h \
           widgets/loosespinbox.h \
           widgets/modulardoublespinbox.h \
           widgets/simplebutton.h \
           widgets/widgetgroup.h \
    dialogs/messagebox.h \
    widgets/docktabwidget.h \
    widgets/tabwidget.h \
    widgets/splitareacontroller.h \
    workspacecanvasareacontroller.h \
    widgets/floatingdocktabwidget.h \
    widgets/splittabareacontroller.h \
    canvasview.h \
    smartpointer.h \
    interface.h \
    debug.h \
    qtsingleapplication/qtsingleapplication.h \
    qtsingleapplication/qtlockedfile.h \
    qtsingleapplication/qtlocalpeer.h \
    appcontroller.h \
    application.h \
    internal/applicationeventfilter.h \
    widgets/vanishingscrollbar.h \
    widgets/docktabmotherwidget.h \
    signalconverter.h \
    callbackanimation.h \
    librarymodel.h \
    list.h \
    widgets/navigatablearea.h \
    keytracker.h \
    dialogs/filedialog.h \
    canvastabwidget.h \
    canvassplitwidget.h \
    canvassplitareacontroller.h
FORMS += dialogs/exportdialog.ui dialogs/newdocumentdialog.ui
SOURCES += \
           canvascontroller.cpp \
           document.cpp \
           documentio.cpp \
           drawutil.cpp \
           layer.cpp \
           layeredit.cpp \
           layermodel.cpp \
           layerrenderer.cpp \
           module.cpp \
           modulemanager.cpp \
           palettemanager.cpp \
           randomstring.cpp \
           thumbnail.cpp \
           toolmanager.cpp \
           util.cpp \
           workspacecontroller.cpp \
           workspacemanager.cpp \
           workspaceview.cpp \
           dialogs/exportdialog.cpp \
           dialogs/newdocumentdialog.cpp \
           internal/layermodelcommand.cpp \
           widgets/colorbutton.cpp \
           widgets/colorslider.cpp \
           widgets/colorwheel.cpp \
           widgets/doubleslider.cpp \
           widgets/loosespinbox.cpp \
           widgets/modulardoublespinbox.cpp \
           widgets/simplebutton.cpp \
           widgets/widgetgroup.cpp \
    dialogs/messagebox.cpp \
    widgets/docktabwidget.cpp \
    widgets/docktabmotherwidget.cpp \
    widgets/tabwidget.cpp \
    widgets/splitareacontroller.cpp \
    workspacecanvasareacontroller.cpp \
    widgets/floatingdocktabwidget.cpp \
    widgets/splittabareacontroller.cpp \
    canvasview.cpp \
    tool.cpp \
    qtsingleapplication/qtsingleapplication.cpp \
    qtsingleapplication/qtlocalpeer.cpp \
    appcontroller.cpp \
    application.cpp \
    internal/applicationeventfilter.cpp \
    widgets/vanishingscrollbar.cpp \
    signalconverter.cpp \
    callbackanimation.cpp \
    librarymodel.cpp \
    widgets/navigatablearea.cpp \
    keytracker.cpp \
    dialogs/filedialog.cpp \
    canvastabwidget.cpp \
    canvassplitwidget.cpp \
    canvassplitareacontroller.cpp
