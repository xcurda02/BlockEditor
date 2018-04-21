#include <QDebug>
#include "block.h"

Block::Block(BlockType blockType, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    port = nullptr;
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

void Block::addPort(Port *port){
    this->port = port;
}

QVariant Block::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){
        qInfo() << "(block)block pos changed x: " << this->x() << " y:" << this->y();
        if (port != nullptr){

           //port->setPos(20,20);
            port->setPos(x()+20,y()+20);
            qInfo() << "(block)port pos chanded x:" << port->x() << " y:" << port->y();
        }
    }
    return value;
}

