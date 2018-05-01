#ifndef PORT_H
#define PORT_H

#include <QGraphicsEllipseItem>
#include "block.h"

class Wire;
class Block;

class Port : public QGraphicsEllipseItem
{

public:
    Port(QPointF blockPos,bool inputPort, Block *block, QGraphicsItem *parent=0);
    void addWire(Wire *wire);
    Wire *getWire();

    bool isInputPort();
    Block *getBlock();


protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:
    Block *block;
    bool inputPort;
    Wire *wire;


};

#endif // PORT_H
