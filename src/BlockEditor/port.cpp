#include <QDebug>
#include <QBrush>
#include "port.h"
#include "wire.h"

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
Port::~Port(){
    if (wire != nullptr){
        Port *port;
        if(inputPort)
            port = wire->getOutPort();
        else
            port = wire->getInPort();
        port->removeWire();
        delete wire;
    }
}

void Port::addWire(Wire *wire){
    this->wire = wire;
}

void Port::removeWire(){
    wire = nullptr;
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

void Port::emph(){

    if (!emphasized){
        QRectF rec = boundingRect();
        setRect(rec.x()-1.5, rec.y()-1.5,15,15);
        QBrush brush(Qt::yellow);
        setBrush(brush);
        update();
        emphasized = true;
    }

}
void Port::unEmph(){
    if (emphasized){
        QRectF rec = boundingRect();
        setRect(rec.x()+2.5, rec.y()+2.5,10,10);
        QBrush brush(Qt::gray);
        setBrush(brush);
        update();
        emphasized = false;
    }
}
double Port::getValue(){
    return this->value;
}



void Port::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Enter:";
     emph();
     QGraphicsItem::hoverEnterEvent(event);
}
void Port::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    qInfo() << "mouse move na port Enter:";
    emph();
    QGraphicsItem::mouseMoveEvent(event);
}

void Port::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
     qInfo() << "Hover Leave:";
     unEmph();
     QGraphicsItem::hoverLeaveEvent(event);

}
