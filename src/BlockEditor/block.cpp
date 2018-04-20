#include <QDebug>
#include "block.h"

Block::Block(BlockType blockType, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    this->blockType =  blockType;
    QPixmap pixmap;
    switch(blockType){
        case addBlock:
            pixmap = QPixmap(":/images/images/addBlock.png");
            break;
        case subBlock:
            pixmap = QPixmap(":/images/images/subBlock.png");
            break;
        case mulBlock:
            pixmap = QPixmap(":/images/images/mulBlock.png");
            break;
        case divBlock:
            pixmap = QPixmap(":/images/images/divBlock.png");
            break;
    }
    setPixmap(pixmap);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    qInfo() << "block created";

}

Block::BlockType Block::getBlockType(){
    return blockType;
}
