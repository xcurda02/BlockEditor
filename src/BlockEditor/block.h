#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "port.h"

class Block : public QGraphicsPixmapItem
{
public:
    enum { Type = UserType + 15};
    enum BlockType {addBlock, subBlock, mulBlock, divBlock};
    Block(BlockType blockType, QGraphicsItem *parent = 0);
    BlockType getBlockType();
    Port *getOutPort();
    void addPort(Port *port);
    void setProcessed();
    void setNotProcessed();
    bool isProcessed();
    int type() const Q_DECL_OVERRIDE {return Type;}
private:
    QList<Port*> ports;
    bool processed;
    BlockType blockType;

protected:


    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
};

#endif // BLOCK_H
