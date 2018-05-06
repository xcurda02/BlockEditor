////// soubor: wire.h
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Definice tridy Wire
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
class Wire : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
signals:
    void setStatusBarText(QString str);
public:
    Wire(Port *startItem,  Port *endItem, QGraphicsItem *parent = 0);
    ~Wire();

    /**
     * @brief redefinice typu
     */
    enum { Type = UserType +4};
    /**
     * @brief Ziskani typu itemu
     * @return typ itemu
     */
    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;

    /**
     * @brief getStartItem Ziskani portu na zacatku dratu
     * @return port na zacatku dratu
     */
    Port *getStartItem() const { return startItem; }

    /**
     * @brief getEndItem Ziskani portu na konci dratu
     * @return port na konci dratu
     */
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
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

private:
    QPointF getMiddlePoint(QGraphicsItem *item);
    double getLowerPortZValue();
    Port *startItem;    ///> port kde propoj zacina
    Port *endItem;      ///> port kde propoj konci
    bool valueSet;      ///> priznak nastaveni hodnoty na propoji
    double value;       ///> hodnota na propoji
    bool hovered;       ///> na dratu je najety kurzor mysi

};

#endif // WIRE_H
