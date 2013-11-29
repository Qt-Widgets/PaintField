######################################################################
# Automatically generated by qmake (2.01a) ? 10? 28 23:57:50 2012
######################################################################

TEMPLATE = lib
TARGET = malachite

DEFINES += MALACHITE_LIBRARY

include(../malachite.pri)

LIBS += -lfreeimage

# Input
HEADERS += bitmap.h \
           blendmode.h \
           blendop.h \
           brush.h \
           color.h \
           colorgradient.h \
           container.h \
           curves.h \
           curvesubdivision.h \
           division.h \
           fixedpolygon.h \
           genericimage.h \
           global.h \
           image.h \
           imageio.h \
           misc.h \
           paintable.h \
           paintengine.h \
           painter.h \
           polygon.h \
           surface.h \
           surfacepainter.h \
           surfaceselection.h \
           private/agg_array.h \
           private/agg_basics.h \
           private/agg_clip_liang_barsky.h \
           private/agg_config.h \
           private/agg_gamma_functions.h \
           private/agg_math.h \
           private/agg_rasterizer_cells_aa.h \
           private/agg_rasterizer_scanline_aa.h \
           private/agg_rasterizer_sl_clip.h \
           private/agg_scanline_p.h \
           private/clipper.hpp \
    private/filler.h \
    private/gradientgenerator.h \
    private/imagepaintengine.h \
    private/renderer.h \
    private/scalinggenerator.h \
    private/surfacepaintengine.h \
    vector_generic.h \
    vector_sse.h \
    interval.h \
    genericpixel.h \
    pixel.h \
    vec2d.h \
    mat2d.h \
    affine2d.h \
    genericsurface.h \
    blendtraits.h \
    pixeliterator.h \
    pixelarray.h
SOURCES += blendmode.cpp \
           blendop.cpp \
           brush.cpp \
           color.cpp \
           colorgradient.cpp \
           curves.cpp \
           curvesubdivision.cpp \
           fixedpolygon.cpp \
           image.cpp \
           imageio.cpp \
           misc.cpp \
           paintengine.cpp \
           painter.cpp \
           polygon.cpp \
           surface.cpp \
           surfacepainter.cpp \
           surfaceselection.cpp \
           private/clipper.cpp \
    private/imagepaintengine.cpp \
    private/renderer.cpp \
    private/surfacepaintengine.cpp
RESOURCES += resources.qrc
