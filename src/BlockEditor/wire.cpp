#include "wire.h"
#include <stdio.h>
#include <cmath>

#include <QPen>
#include <QPainter>
#include <QDebug>

const qreal Pi = 3.14;

Wire::Wire(Port *startItem,  Port *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
       this->startItem = startItem;
       this->endItem = endItem;
       setFlag(QGraphicsItem::ItemIsSelectable, true);
       setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
       qInfo() << "wire created";

       valueSet = false;
}

bool Wire::isValueSet(){
    return valueSet;
}
void Wire::setValue(double value){
    valueSet = true;
    this->value = value;
}
double Wire::getValue(){
    return value;
}
void Wire::unsetValue(){
    valueSet = false;
}

Port *Wire::getInPort(){
    if (endItem->isInputPort())
        return endItem;
    else
        return startItem;
}
Port *Wire::getOutPort(){
    if (startItem->isInputPort())
        return endItem;
    else
        return startItem;
}

QRectF Wire::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
QPointF Wire::getMiddlePoint(QGraphicsItem *item){
    double width = fabs(item->boundingRect().topLeft().x() - item->boundingRect().topRight().x());
    double height = fabs(item->boundingRect().topRight().y() - item->boundingRect().bottomRight().y());
    return QPointF(width/2,height/2);
}

void Wire::updatePosition()
{
    qInfo() << "wire: update position";
    QPointF startMiddlePoint = getMiddlePoint(startItem);
    QPointF endMiddlePoint = getMiddlePoint(endItem);
    QLineF line(QPointF(startItem->boundingRect().topLeft().x()+startMiddlePoint.x()-0.5,startItem->boundingRect().topLeft().y()+startMiddlePoint.y()-0.5),
                QPointF(endItem->boundingRect().topLeft().x()+endMiddlePoint.x()-0.5,endItem->boundingRect().topLeft().y()+endMiddlePoint.y()-0.5));
    setLine(line);
}



void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    qInfo() << "wire: paint";
    QPen myPen = pen();
    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 1));
    updatePosition();
    painter->drawLine(line());
}
