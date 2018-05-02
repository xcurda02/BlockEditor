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
    ~Port();
    void addWire(Wire *wire);
    void removeWire();
    Wire *getWire();

    bool isInputPort();
    Block *getBlock();

    void emph();
    void unEmph();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
    Block *block;
    bool inputPort;
    Wire *wire;
    bool emphasized;


};

#endif // PORT_H
