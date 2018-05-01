#include <QDebug>
#include <QBrush>
#include "port.h"

Port::Port(QPointF blockPos,bool inputPort,Block *block, QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setRect(blockPos.x(),blockPos.y(),10,10);
    QBrush brush(Qt::gray);
    setBrush(brush);
    setAcceptHoverEvents(true);
    wire = nullptr;
    this->block = block;

    this->inputPort = inputPort;
}

void Port::addWire(Wire *wire){
    this->wire = wire;
}

Wire *Port::getWire(){
    return wire;
}

bool Port::isInputPort(){
    return inputPort;
}

Block *Port::getBlock(){
    return block;
}




void Port::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Enter:";
     QRectF rec = boundingRect();
     setRect(rec.x()-1.5, rec.y()-1.5,15,15);
     QBrush brush(Qt::green);
     setBrush(brush);
     update();
     QGraphicsItem::hoverEnterEvent(event);
}

void Port::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Leave:";
     QRectF rec = boundingRect();
     setRect(rec.x()+2.5, rec.y()+2.5,10,10);
     QBrush brush(Qt::gray);
     setBrush(brush);
     update();
     QGraphicsItem::hoverLeaveEvent(event);

}
