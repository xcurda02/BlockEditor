#include "wire.h"
#include <stdio.h>
#include <math.h>

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
       setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
       qInfo() << "wire created";

       valueSet = false;
}
bool Wire::isValueSet(){
    return valueSet;
}
void Wire::setValue(double value){
    this->value = value;
}
double Wire::getValue(){
    return value;
}


QRectF Wire::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

/*
 * neco s sipkou na konci cary
QPainterPath Wire::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
*/

void Wire::updatePosition()
{
    QLineF line(QPointF(startItem->boundingRect().topLeft().x(),startItem->boundingRect().topLeft().y()), QPointF(endItem->boundingRect().topLeft().x(),endItem->boundingRect().topLeft().y()));
    setLine(line);
}



//zjednodusena funkce kresleni cary
//v arrow::paint je:
//kdyz tahnu z/do prostred bloku, cara se vykresli z kraje na kraj
//kresleni konce sipky

//Wire::paint - zatim jednoduche vykresleni cary
//problem startItem endItem predavaji souradnice [0.0, 0.0]
void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    QPen myPen = pen();
    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 4));
    QLineF line(QPointF(startItem->boundingRect().topLeft().x()+5.0,startItem->boundingRect().topLeft().y()+5.0), QPointF(endItem->boundingRect().topLeft().x()+5.0,endItem->boundingRect().topLeft().y()+5.0));
    painter->drawLine(line);
}
