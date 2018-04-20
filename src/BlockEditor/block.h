#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>

class Block : public QGraphicsPixmapItem
{
public:
    enum BlockType {addBlock, subBlock, divBlock, mulBlock};
    Block(const QPixmap & pixmap);
};

#endif // BLOCK_H
