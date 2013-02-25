#include <QtGui>
#include <Malachite/GenericImage>
#include <Malachite/PixelConversion>
#include "util.h"

#include "drawutil.h"

namespace PaintField
{

namespace DrawUtil
{

using namespace Malachite;

void drawMLSurface(QPainter *painter, const QPoint &point, const Surface &surface)
{
	for (const QPoint &key : surface.keys())
		drawMLImage(painter, point + key * Surface::tileWidth(), surface.tile(key));
}

void drawMLImage(QPainter *painter, const QPoint &point, const Image &image)
{
	ImageU8 imageU8 = image.toImageU8();
	painter->drawImage(point, imageU8.wrapInQImage());
}

}

}
