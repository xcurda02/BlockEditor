#ifndef WIRE_H
#define WIRE_H

#include <QMainWindow>
#include <QObject>
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
    void setColor(const QColor &color) { myColor = color; }
    Port *getStartItem() const { return startItem; }
    Port *getEndItem() const { return endItem; }
    Port *getInPort();
    Port *getOutPort();
    void updatePosition();

    bool isValueSet();
    void setValue(double value);
    void unsetValue();
    double getValue();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    Port *startItem;
    Port *endItem;
    QColor myColor;

    bool valueSet;
    double value;

};

#endif // WIRE_H
