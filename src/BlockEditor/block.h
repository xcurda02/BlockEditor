#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>

class Block : public QGraphicsPixmapItem
{
public:
    enum BlockType {addBlock, subBlock, divBlock, mulBlock};
    Block(BlockType blockType, QGraphicsItem *parent = 0);
    BlockType getBlockType();
protected:
    BlockType blockType;
};

#endif // BLOCK_H
