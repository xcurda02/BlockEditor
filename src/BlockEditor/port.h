#ifndef PORT_H
#define PORT_H

#include <QGraphicsEllipseItem>

class Wire;

class Port : public QGraphicsEllipseItem
{

public:
    Port(QPointF blockPos,bool inputPort, QGraphicsItem *parent=0);
    void addWire(Wire *wire);
    Wire *getWire();
    bool isInputPort();
    bool isValueSet();
    void setValue(double value);


protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:
    bool inputPort;
    bool valueSet;
    double value;
    Wire *wire;


};

#endif // PORT_H
