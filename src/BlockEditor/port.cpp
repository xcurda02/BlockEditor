////// soubor: port.cpp
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Soubor s definicemi metod tridy Port

#include <QDebug>
#include <QBrush>
#include "port.h"
#include "wire.h"

/**
 * @brief Port::Port Konstruktor tridy Port
 * @param blockPos  Souradnice bloku
 * @param inputPort Informace, jestli je vytvareny port vstupni
 * @param block     Blok na kterem je port umisten
 * @param parent    Rodicovsky objekt
 */
Port::Port(QPointF blockPos,bool inputPort,Block *block, QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setRect(blockPos.x(),blockPos.y(),10,10);
    QBrush brush(Qt::gray);
    setBrush(brush);
    setAcceptHoverEvents(true);
    wire = NULL;
    this->block = block;
    this->inputPort = inputPort;
}

/**
 * @brief Port::~Port Destruktor tridy port
 * Maze pripadny drat
 */
Port::~Port(){
    if (wire != NULL){
        Port *port;
        if(inputPort)
            port = wire->getOutPort();
        else
            port = wire->getInPort();
        port->removeWire();
        delete wire;
    }
}

/**
 * @brief Port::addWire Pridani dratu
 * @param wire Drat
 */
void Port::addWire(Wire *wire){
    this->wire = wire;
}

/**
 * @brief Port::removeWire Odebrani dratu
 */
void Port::removeWire(){
    wire = NULL;
}

/**
 * @brief Port::getWire Ziskani dratu
 * @return Drat
 */
Wire *Port::getWire(){
    return wire;
}

/**
 * @brief Port::isInputPort
 * @return true - port je vstupni | false - port je vystupni
 */
bool Port::isInputPort(){
    return inputPort;
}

/**
 * @brief Port::getBlock
 * @return Blok na kterem je port
 */
Block *Port::getBlock(){
    return block;
}

/**
 * @brief Port::emph Zvyrazneni portu
 */
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

/**
 * @brief Port::unEmph Zruseni zvyrazneni portu
 */
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

/**
 * @brief Port::hoverEnterEvent Event najeti mysi na port
 * @param event Event
 */
void Port::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
     emph();
     QGraphicsItem::hoverEnterEvent(event);
}

/**
 * @brief Port::hoverEnterEvent Event odjeti mysi z portu
 * @param event Event
 */
void Port::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
     unEmph();
     QGraphicsItem::hoverLeaveEvent(event);

}
