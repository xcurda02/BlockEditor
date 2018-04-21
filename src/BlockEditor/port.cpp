#include <QDebug>
#include "port.h"

Port::Port(QPointF blockPos, QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setRect(0,0,10,10);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QVariant Port::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){

            qInfo() << "(port)port pos chanded x:" << x() << " y:" << y();

    }
    return value;
}
