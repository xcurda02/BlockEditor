#ifndef PORT_H
#define PORT_H

#include <QGraphicsEllipseItem>

class Port : public QGraphicsEllipseItem
{

public:
    Port(QPointF blockPos, QGraphicsItem *parent=0);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
};

#endif // PORT_H
