######################################################################
# Automatically generated by qmake (2.01a) ? 10? 28 13:02:30 2012
######################################################################

TEMPLATE = lib
TARGET = paintfield-core

DEFINES += PAINTFIELD_LIBRARY

include(../paintfield-core.pri)

# Input
HEADERS += \
           document.h \
           drawutil.h \
           global.h \
           layer.h \
           layeredit.h \
           layerrenderer.h \
           palettemanager.h \
           randomstring.h \
           tabletevent.h \
           tabletinputdata.h \
           thumbnail.h \
           tool.h \
           toolmanager.h \
           util.h \
           workspacemanager.h \
           workspaceview.h \
           dialogs/exportdialog.h \
           dialogs/newdocumentdialog.h \
           widgets/colorbutton.h \
           widgets/colorslider.h \
           widgets/colorwheel.h \
           widgets/loosespinbox.h \
           widgets/modulardoublespinbox.h \
           widgets/simplebutton.h \
           widgets/widgetgroup.h \
    dialogs/messagebox.h \
    widgets/docktabwidget.h \
    interface.h \
    appcontroller.h \
    application.h \
    widgets/vanishingscrollbar.h \
    widgets/docktabmotherwidget.h \
    signalconverter.h \
    callbackanimation.h \
    librarymodel.h \
    keytracker.h \
    dialogs/filedialog.h \
    canvastabwidget.h \
    canvassplitwidget.h \
    canvassplitareacontroller.h \
    widgets/memorizablesplitter.h \
    settingsmanager.h \
    workspacetabwidget.h \
    scopedtimer.h \
    canvas.h \
    workspace.h \
    extension.h \
    extensionmanager.h \
    selection.h \
    tabletpointerinfo.h \
    cursorstack.h \
    generaleditaction.h \
    proxyaction.h \
    rasterlayer.h \
    grouplayer.h \
    shapelayer.h \
    layerfactorymanager.h \
    documentcontroller.h \
    documentreferencemanager.h \
    rectlayer.h \
    layerscene.h \
    layeritemmodel.h \
    colorbuttongroup.h \
    textlayer.h \
    pathrecorder.h \
    abstractrectlayer.h \
    serializationutil.h \
    widgets/abstractlayerpropertyeditor.h \
    json.h \
    canvasviewportmipmap.h \
    canvasviewportsurface.h \
    canvasviewportstate.h \
    blendmodetexts.h \
    formatsupport.h \
    singlelayerformatsupport.h \
    formatsupportmanager.h \
    dialogs/formatexportdialog.h \
    zip.h \
    paintfieldformatsupport.h \
    widgets/looseslider.h \
    internal/applicationeventfilter.h \
    observablevariantmap.h \
    canvasviewport.h \
    canvastooleventfilter.h \
    canvasnavigatoreventfilter.h \
    canvasview.h \
    canvasupdatemanager.h \
    selectionsurface.h \
    closureundocommand.h \
    canvastransforms.h

FORMS +=
SOURCES += \
           document.cpp \
           drawutil.cpp \
           layer.cpp \
           layeredit.cpp \
           layerrenderer.cpp \
           palettemanager.cpp \
           randomstring.cpp \
           thumbnail.cpp \
           toolmanager.cpp \
           util.cpp \
           workspacemanager.cpp \
           workspaceview.cpp \
           dialogs/exportdialog.cpp \
           dialogs/newdocumentdialog.cpp \
           widgets/colorbutton.cpp \
           widgets/colorslider.cpp \
           widgets/colorwheel.cpp \
           widgets/loosespinbox.cpp \
           widgets/modulardoublespinbox.cpp \
           widgets/simplebutton.cpp \
           widgets/widgetgroup.cpp \
    dialogs/messagebox.cpp \
    widgets/docktabwidget.cpp \
    widgets/docktabmotherwidget.cpp \
    tool.cpp \
    appcontroller.cpp \
    application.cpp \
    widgets/vanishingscrollbar.cpp \
    signalconverter.cpp \
    callbackanimation.cpp \
    librarymodel.cpp \
    keytracker.cpp \
    dialogs/filedialog.cpp \
    canvastabwidget.cpp \
    canvassplitwidget.cpp \
    canvassplitareacontroller.cpp \
    widgets/memorizablesplitter.cpp \
    settingsmanager.cpp \
    workspacetabwidget.cpp \
    canvas.cpp \
    workspace.cpp \
    extension.cpp \
    extensionmanager.cpp \
    selection.cpp \
    cursorstack.cpp \
    generaleditaction.cpp \
    proxyaction.cpp \
    rasterlayer.cpp \
    grouplayer.cpp \
    shapelayer.cpp \
    layerfactorymanager.cpp \
    documentcontroller.cpp \
    documentreferencemanager.cpp \
    rectlayer.cpp \
    layerscene.cpp \
    layeritemmodel.cpp \
    colorbuttongroup.cpp \
    textlayer.cpp \
    pathrecorder.cpp \
    abstractrectlayer.cpp \
    serializationutil.cpp \
    widgets/abstractlayerpropertyeditor.cpp \
    json.cpp \
    canvasviewportmipmap.cpp \
    blendmodetexts.cpp \
    formatsupport.cpp \
    singlelayerformatsupport.cpp \
    formatsupportmanager.cpp \
    dialogs/formatexportdialog.cpp \
    zip.cpp \
    paintfieldformatsupport.cpp \
    widgets/looseslider.cpp \
    internal/applicationeventfilter.cpp \
    observablevariantmap.cpp \
    canvasviewport.cpp \
    canvasnavigatoreventfilter.cpp \
    canvastooleventfilter.cpp \
    canvasview.cpp \
    canvasupdatemanager.cpp \
    selectionsurface.cpp \
    canvasviewportstate.cpp

RESOURCES += \
    resources/resource-paintfield-core.qrc

TRANSLATIONS += core_ja.ts

