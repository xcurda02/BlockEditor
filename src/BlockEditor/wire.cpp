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
       myStartItem = startItem;
       myEndItem = endItem;
       setFlag(QGraphicsItem::ItemIsSelectable, true);
       setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
       qInfo() << "wire created";
}

QRectF Wire::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}


void Wire::updatePosition()
{
    QLineF line(QPointF(myStartItem->boundingRect().topLeft().x(),myStartItem->boundingRect().topLeft().y()), QPointF(myEndItem->boundingRect().topLeft().x(),myEndItem->boundingRect().topLeft().y()));
    setLine(line);
}



void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    QPen myPen = pen();
    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 4));
    QLineF line(QPointF(myStartItem->boundingRect().topLeft().x()+5.0,myStartItem->boundingRect().topLeft().y()+5.0), QPointF(myEndItem->boundingRect().topLeft().x()+5.0,myEndItem->boundingRect().topLeft().y()+5.0));
    painter->drawLine(line);
}
