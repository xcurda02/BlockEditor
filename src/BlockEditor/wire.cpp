////// soubor: wire.cpp
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Soubor s definicemi metod tridy Wire

#include "wire.h"
#include <stdio.h>
#include <math.h>
#include <QPen>
#include <QPainter>
#include <QDebug>

/**
 * @brief Wire::wire Konstruktor tridy Wire
 * @param startItem objekt Port, zacatecni bod propoje
 * @param endItem objekt Port, koncovy bod propoje
 * @param parent rodicovsky objekt
 */
Wire::Wire(Port *startItem,  Port *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
       this->startItem = startItem;
       this->endItem = endItem;
       setFlag(QGraphicsItem::ItemIsSelectable, true);
       setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

       valueSet = false;
}

/**
 * @brief Wire::isValueSet zjisti, jestli je na propoji nastavena hodnota
 * @return valueSet true or false podle toho jestli je hodnota nastavena
*/
bool Wire::isValueSet(){
    return valueSet;
}

/**
 * @brief Wire::setValue nastavi hodnotu na propoj
 * @param value hodnota ktera se nastavi
*/
void Wire::setValue(double value){
    valueSet = true;
    this->value = value;
}

/**
 * @brief Wire::getValue vraci hodnotu na propoji
 * @return value hodnota na propoji
*/
double Wire::getValue(){
    return value;
}

/**
 * @brief Wire::unsetValue maze hodnotu z propoje
*/
void Wire::unsetValue(){
    valueSet = false;
}

/**
 * @brief Wire::getInPort vraci vstupni port
 * @return vstupni port
 */
Port *Wire::getInPort(){
    if (endItem->isInputPort())
        return endItem;
    else
        return startItem;
}

/**
 * @brief Wire::getOutPort vraci vystupni port
 * @return vystupni port
 */
Port *Wire::getOutPort(){
    if (startItem->isInputPort())
        return endItem;
    else
        return startItem;
}

/**
 * @brief Wire::boundingRect vraci obdelnik kolem portu
 * @return obdelnik
 */
QRectF Wire::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

/**
 * @brief Wire::updatePosition pri pohybu bloku prekresli propojeni
 */
void Wire::updatePosition()
{
    QLineF line(QPointF(startItem->boundingRect().topLeft().x(),startItem->boundingRect().topLeft().y()), QPointF(endItem->boundingRect().topLeft().x(),endItem->boundingRect().topLeft().y()));
    setLine(line);
}


/**
 * @brief Wire::paint kresli propojeni mezi porty
 * @param painter
 */
void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    QPen myPen = pen();
    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 2));
    QLineF line(QPointF(startItem->boundingRect().topLeft().x()+5.0,startItem->boundingRect().topLeft().y()+5.0), QPointF(endItem->boundingRect().topLeft().x()+5.0,endItem->boundingRect().topLeft().y()+5.0));
    painter->drawLine(line);
}
