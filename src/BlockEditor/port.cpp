#include <QDebug>
#include <QBrush>
#include "port.h"

Port::Port(QPointF blockPos, QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setRect(0,0,10,10);
    QBrush brush(Qt::gray);
    setBrush(brush);
    //setFlag(QGraphicsItem::ItemIsMovable, true);
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
}

QVariant Port::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){

            qInfo() << "(port)port pos chanded x:" << x() << " y:" << y();

    }
    return value;
}

void Port::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Enter";
     setRect(x(),y(),15,15);
     QBrush brush(Qt::green);
     setBrush(brush);
     update();
     QGraphicsItem::hoverEnterEvent(event);
}

void Port::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Enter";
     setRect(x(),y(),10,10);
     QBrush brush(Qt::gray);
     setBrush(brush);
     update();
     QGraphicsItem::hoverLeaveEvent(event);
}
