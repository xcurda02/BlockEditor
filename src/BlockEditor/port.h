////// soubor: port.h
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Soubor s definici tridy Port
///
#ifndef PORT_H
#define PORT_H

#include <QGraphicsEllipseItem>
#include "block.h"

class Wire;
class Block;

/**
 * @brief The Port class Trida port reprezentuje port, dedi od Qt tridy QGraphicsEllipseItem
 */
class Port : public QGraphicsEllipseItem
{

public:
    Port(QPointF blockPos,bool inputPort, Block *block, QGraphicsItem *parent=0);
    ~Port();
    void addWire(Wire *wire);
    void removeWire();
    Wire *getWire();

    bool isValueSet();
    void setValue(double value);
    double getValue();

    bool isInputPort();
    Block *getBlock();

    void emph();
    void unEmph();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:
    Block *block;           ///> Blok na kterem je port
    bool inputPort;         ///> Informace, jestli je port vstupni
    Wire *wire;             ///> Drat
    bool emphasized;        ///> Informace, jestli je port zvyraznen


};

#endif // PORT_H
