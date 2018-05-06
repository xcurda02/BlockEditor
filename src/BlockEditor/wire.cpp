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
       valueSet = false;
       setAcceptHoverEvents(true);
       setFlag(QGraphicsItem::ItemIsSelectable, true);
       hovered = false;
       setZValue(getLowerPortZValue()-0.1);
}

/**
 * @brief Wire::~Wire Destruktor tridy Wire - odnastaveni dratu na portech
 */
Wire::~Wire(){
    startItem->removeWire();
    endItem->removeWire();
    emit setStatusBarText(QString("Ready"));
}

/**
 * @brief Wire::isValueSet zjisti, jestli je na propoji nastavena hodnota
 * @return valueSet true or false podle toho jestli je hodnota nastavena
*/
bool Wire::isValueSet(){
    return valueSet;
}

/**
 * @brief Wire::getLowerPort Vrati vysku nizsiho portu
 * @return
 */
double Wire::getLowerPortZValue(){
    return startItem->zValue() < endItem->zValue() ? startItem->zValue() : endItem->zValue();
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
 * @brief Wire::getMiddlePoint Ziskani prostredku itemu
 * @param item
 * @return
 */
QPointF Wire::getMiddlePoint(QGraphicsItem *item){
    double width = fabs(item->boundingRect().topLeft().x() - item->boundingRect().topRight().x());
    double height = fabs(item->boundingRect().topRight().y() - item->boundingRect().bottomRight().y());
    return QPointF(width/2,height/2);
}

/**
 * @brief Wire::updatePosition Aktualizace pozice dratu podle portu
 */
void Wire::updatePosition()
{
    QPointF startMiddlePoint = getMiddlePoint(startItem);
    QPointF endMiddlePoint = getMiddlePoint(endItem);
    QLineF line(QPointF(startItem->boundingRect().topLeft().x()+startMiddlePoint.x()-0.5,startItem->boundingRect().topLeft().y()+startMiddlePoint.y()-0.5),
                QPointF(endItem->boundingRect().topLeft().x()+endMiddlePoint.x()-0.5,endItem->boundingRect().topLeft().y()+endMiddlePoint.y()-0.5));
    setLine(line);
}


/**
 * @brief Wire::paint Vykresleni dratu
 * @param painter Painter
 */
void Wire::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    QPen myPen = pen();
    painter->setBrush(QBrush(Qt::black));
    if(hovered)
        painter->setPen(QPen(QColor(80,80,80), 5));
    else
        painter->setPen(QPen(Qt::black, 3));
    updatePosition();
    painter->drawLine(line());
}

/**
 * @brief Wire::hoverEnterEvent Event najeti mysi na drat
 * @param event Event
 */
void Wire::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
    QString str;
    if (isValueSet()){
        str = QString::number(getValue());
    }else{
        str = QString("N/A");
    }
    str = QString("Value on wire: ") + str;
    emit setStatusBarText(str);
    hovered = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

/**
 * @brief Wire::hoverLeaveEvent Event odjeti mysi z dratu
 * @param event Event
 */
void Wire::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
    emit setStatusBarText(QString("Ready"));
    hovered = false;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}
