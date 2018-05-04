////// soubor: wire.h
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Definice tridy Block
#ifndef WIRE_H
#define WIRE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include "block.h"


QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE


/**
 * @brief The Port class Trida wire reprezentuje propoj, dedi od Qt tridy QGraphicsLineItem
 */
class Wire : public QGraphicsLineItem
{
public:
    Wire(Port *startItem,  Port *endItem, QGraphicsItem *parent = 0);

    enum { Type = UserType +4};
    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    Port *getStartItem() const { return startItem; }
    Port *getEndItem() const { return endItem; }
    Port *getInPort();
    Port *getOutPort();
    void updatePosition();

    bool isValueSet();
    void setValue(double value);
    void unsetValue();
    double getValue();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    QPointF getMiddlePoint(QGraphicsItem *item);
    Port *startItem;    ///> port kde propoj zacina
    Port *endItem;      ///> port kde propoj konci
    bool valueSet;      ///> priznak nastaveni hodnoty na propoji
    double value;       ///> hodnota na propoji

};

#endif // WIRE_H
