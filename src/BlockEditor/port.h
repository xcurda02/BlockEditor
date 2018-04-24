#ifndef PORT_H
#define PORT_H

#include <QGraphicsEllipseItem>

class Wire;

class Port : public QGraphicsEllipseItem
{

public:
    Port(QPointF blockPos,bool inputPort, QGraphicsItem *parent=0);
    void addWire(Wire *wire);
    Wire *wire;
    bool isInputPort();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:
    bool inputPort;

};

#endif // PORT_H
