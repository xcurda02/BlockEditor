#ifndef PORT_H
#define PORT_H

#include <QGraphicsEllipseItem>

class Wire;

class Port : public QGraphicsEllipseItem
{

public:
    Port(QPointF blockPos, QGraphicsItem *parent=0);
    void addWire(Wire *wire);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
private:
    QList<Wire *> wires;
};

#endif // PORT_H
