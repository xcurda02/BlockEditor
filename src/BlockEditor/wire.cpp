#include "wire.h"
#include <stdio.h>
#include <math.h>

#include <QPen>
#include <QPainter>

const qreal Pi = 3.14;

Wire::Wire(Port *startItem,  Port *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
       myStartItem = startItem;
       myEndItem = endItem;
       setFlag(QGraphicsItem::ItemIsSelectable, true);
       setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
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
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
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
    QLineF line(myStartItem->pos(), myEndItem->pos());
    //QLineF line(161.0,195.0,1.0,1.0);
    painter->drawLine(line);
}
