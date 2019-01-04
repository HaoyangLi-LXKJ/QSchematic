#include <QPointF>
#include <QRectF>
#include <QVector2D>
#include "settings.h"

using namespace QSchematic;

Settings::Settings() :
    debug(false),
    gridSize(20),
    gridPointSize(3),
    showGrid(true),
    highlightRectPadding(10),
    resizeHandleSize(9),
    routeStraightAngles(true),
    preserveStraightAngles(true),
    antialiasing(true),
    popupDelay(400)
{
}

QPoint Settings::toGridPoint(const QPointF& point) const
{
    int gridX = qRound(point.x() / gridSize);
    int gridY = qRound(point.y() / gridSize);

    return QPoint(gridX, gridY);
}

QPoint Settings::toScenePoint(const QPoint& gridCoordinate) const
{
    return gridCoordinate * gridSize;
}

QPoint Settings::snapToGridPoint(const QPointF& scenePoint) const
{
    int xV = qRound(scenePoint.x() / gridSize) * gridSize;
    int yV = qRound(scenePoint.y() / gridSize) * gridSize;

    return QPoint(xV, yV);
}
QVector2D Settings::snapToGrid(const QVector2D& sceneVector) const
{
    int xV = qRound(sceneVector.x() / gridSize) * gridSize;
    int yV = qRound(sceneVector.y() / gridSize) * gridSize;

    return QVector2D(xV, yV);
}
