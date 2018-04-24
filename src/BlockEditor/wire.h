#ifndef WIRE_H
#define WIRE_H

//#include <QDeclarativeItem>
#include <QMainWindow>
#include <QObject>
//#include <QQuickItem>
#include <QWidget>

#include "block.h"


QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE



class Wire : public QGraphicsLineItem
{
public:
    Wire(Port *startItem,  Port *endItem, QGraphicsItem *parent = 0);

    enum { Type = UserType +4};
    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    //QPainterPath shape() const Q_DECL_OVERRIDE;
    void setColor(const QColor &color) { myColor = color; }
    Port *startItem() const { return myStartItem; }
    Port *endItem() const { return myEndItem; }
    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    Port *myStartItem;
    Port *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;

};

#endif // WIRE_H
