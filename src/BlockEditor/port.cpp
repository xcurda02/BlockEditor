#include <QDebug>
#include <QBrush>
#include "port.h"

Port::Port(QPointF blockPos, QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    qInfo() << "port created at (" << blockPos.x() << ";" << blockPos.y();
    setRect(blockPos.x(),blockPos.y(),10,10);


    QBrush brush(Qt::gray);
    setBrush(brush);
    //setFlag(QGraphicsItem::ItemIsMovable, true);
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    wire = NULL;
}

QVariant Port::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){

            qInfo() << "(port)port pos chanded x:" << x() << " y:" << y();

    }
    return value;
}

void Port::addWire(Wire *wire){
    this->wire = wire;
}


void Port::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Enter:";
     QRectF rec = boundingRect();
     qInfo() << "old bounding rect x:" << rec.x() << "y:" << rec.y();
     setRect(rec.x()-1.5, rec.y()-1.5,15,15);
     qInfo() << "new bounding rect x:" << boundingRect().x() << "y:" << boundingRect().y();
     QBrush brush(Qt::green);
     setBrush(brush);
     update();
     QGraphicsItem::hoverEnterEvent(event);
     qInfo() << "\n";
}

void Port::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Leave:";
     QRectF rec = boundingRect();
     qInfo() << "old bounding rect x:" << rec.x() << "y:" << rec.y();
     setRect(rec.x()+2.5, rec.y()+2.5,10,10);
     qInfo() << "new bounding rect x:" << boundingRect().x() << "y:" << boundingRect().y();
     QBrush brush(Qt::gray);
     setBrush(brush);
     update();
     QGraphicsItem::hoverLeaveEvent(event);
     qInfo() << "\n";

}
