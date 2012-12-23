#include <QtGui>
#include <Malachite/SurfacePainter>

#include "paintfield-core/layeredit.h"
#include "paintfield-core/canvascontroller.h"
#include "paintfield-core/workspacecontroller.h"
#include "paintfield-core/palettemanager.h"
#include "paintfield-core/tabletevent.h"
#include "paintfield-core/debug.h"
#include "paintfield-core/widgets/simplebutton.h"

#include "brushstroker.h"

#include "brushtool.h"


using namespace Malachite;

namespace PaintField {

BrushTool::BrushTool(CanvasView *parent) :
	Tool(parent)
{
	setCustomCursorEnabled(true);
}

BrushTool::~BrushTool() {}

void BrushTool::drawLayer(SurfacePainter *painter, const Layer *layer)
{
	Q_UNUSED(layer)
	painter->drawTransformedSurface(QPoint(), _surface);
}

void BrushTool::drawCustomCursor(QPainter *painter, const Vec2D &pos)
{
	double radius = _brushSize * 0.5 * canvasView()->scale();
	painter->setPen(Qt::black);
	painter->setBrush(Qt::NoBrush);
	painter->drawEllipse(pos, radius, radius);
}

QRect BrushTool::customCursorRect(const Vec2D &pos)
{
	double radius = _brushSize * 0.5 * canvasView()->scale() + 0.5;
	return QRectF(pos.x - radius, pos.y - radius, radius * 2, radius * 2).toAlignedRect();
}

void BrushTool::tabletPressEvent(CanvasTabletEvent *event)
{
	event->accept();
}

void BrushTool::tabletMoveEvent(CanvasTabletEvent *event)
{
	//PAINTFIELD_DEBUG << "tablet event x:" << event->data.pos.x << "y:" << event->data.pos.y << "pressure:" << event->data.pressure;
	
	if (_stroker)
	{
		if (_trailingEnabled)
		{
			if (_trailing)
			{
				_trailing = false;
				endStroke(event->data);
			}
			else
			{
				drawStroke(event->data);
				if (event->data.pressure == 0)
					_trailing = true;
			}
		}
		else
		{
			if (event->data.pressure)
				drawStroke(event->data);
			else
				endStroke(event->data);
		}
	}
	else if (event->data.pressure)
	{
		if (!_dataPrevSet)
			setPrevData(event->data);
		
		beginStroke(event->data);
	}
	
	setPrevData(event->data);
	
	event->accept();
}

void BrushTool::tabletReleaseEvent(CanvasTabletEvent *event)
{
	event->accept();
}

void BrushTool::beginStroke(const TabletInputData &data)
{
	if (!_strokerFactory)
		return;
	
	_layer = currentLayer();
	
	if (_layer->type() != Layer::TypeRaster)
		return;
	
	PAINTFIELD_CALC_SCOPE_ELAPSED_TIME;
	
	_surface = _layer->surface();
	
	_stroker.reset(_strokerFactory->createStroker(&_surface));
	_stroker->loadSettings(_settings);
	_stroker->setArgb(_argb);
	_stroker->setRadiusBase(double(_brushSize) * 0.5);
	
	addCustomDrawLayer(_layer);
	
	// discard pressure for the 1st time to reduce overshoot
	TabletInputData newData = data;
	newData.pressure = 0;
	
	if (_trailingEnabled)
	{
		_stroker->moveTo(_dataBeforePrev);
		_stroker->lineTo(_dataPrev);
		_stroker->lineTo(newData);
	}
	else
	{
		_stroker->moveTo(_dataPrev);
		_stroker->lineTo(newData);
	}
}

void BrushTool::drawStroke(const TabletInputData &data)
{
	PAINTFIELD_CALC_SCOPE_ELAPSED_TIME;
	
	_stroker->lineTo(data);
	updateTiles();
}

void BrushTool::endStroke(const TabletInputData &data)
{
	PAINTFIELD_CALC_SCOPE_ELAPSED_TIME;
	
	_stroker->lineTo(data);
	_stroker->end();
	updateTiles();
	
	if (_layer && _layer == currentLayer())
	{
		document()->layerModel()->makeSkipNextUpdate();
		document()->layerModel()->editLayer(document()->layerModel()->indexForLayer(_layer), new LayerSurfaceEdit(_surface, _stroker->totalEditedKeys()), tr("Brush"));
	}
	
	_stroker.reset();
	clearCustomDrawLayer();
}

void BrushTool::updateTiles()
{
	emit requestUpdate(_stroker->lastEditedKeysWithRects());
	_stroker->clearLastEditedKeys();
}

void BrushTool::setPrevData(const TabletInputData &data)
{
	if (_dataPrevSet)
	{
		_dataBeforePrev = _dataPrev;
		_dataPrev = data;
	}
	else
	{
		_dataBeforePrev = data;
		_dataPrev = data;
		_dataPrevSet = true;
	}
}

void BrushTool::setBrushSize(int size)
{
	_brushSize = size;
}

void BrushTool::setBrushSettings(const QVariantMap &settings)
{
	_settings = settings;
}

}
