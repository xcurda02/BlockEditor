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

void Block::addPort(Port *port){
    ports.append(port);
}

QVariant Block::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){
        qInfo() << "(block)block pos changed x: " << this->x() << " y:" << this->y();

        double blockTopEdge = abs(boundingRect().topRight().x() - boundingRect().topLeft().x());
        double margin = blockTopEdge / (ports.count()+1);

        for (int i = 1; i < ports.count()+1; i++){
            Port *port = ports[i-1];
            port->setPos(QPointF(x()+(i*margin),y()-5));
            qInfo() << "(block)port pos chanded x:" << port->x() << " y:" << port->y();
        }
    }
    return value;
}

