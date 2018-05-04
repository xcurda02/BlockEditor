////// soubor: block.h
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Definice tridy Block
#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "port.h"

class Port;
class QPainter;


/**
 * @brief Trida blok
 * Dedi od Qt Tridy QGraphicsPixmapItem
 * Slouzi pro reprezentaci bloku
 */
class Block : public QGraphicsPixmapItem
{
public:
    /**
     * @brief redefinice Typu
     */
    enum { Type = UserType + 15};
    /**
     * @brief Typ bloku
     */
    enum BlockType {addBlock, subBlock, mulBlock, divBlock};
    Block(BlockType blockType, QGraphicsItem *parent = 0);
    ~Block();
    BlockType getBlockType();
    Port *getOutPort();
    void addPort(Port *port);
    void setProcessed();
    void setNotProcessed();

    bool isToSkip();
    void skip();
    void unSkip();

    bool isProcessed();
    void emph();
    void unEmph();

    QList<Port*> getPorts();

    int type() const Q_DECL_OVERRIDE {return Type;}
    QList<Port*> ports;         ///> Seznam portu bloku

private:

    bool processed;             ///> Blok zpracovan
    bool toSkip;                ///> Preskoceni bloku
    BlockType blockType;        ///> Typ bloku


protected:


    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
};

#endif // BLOCK_H
