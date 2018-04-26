#include <QDebug>
#include <QBrush>
#include "wire.h"
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

Port *Block::getOutPort(){
    return ports[ports.count()-1];

}

void Block::setProcessed(){
    processed = true;
}

void Block::setNotProcessed(){
    processed = false;
}

bool Block::isProcessed(){
    return processed;
}

QVariant Block::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){

        double blockTopEdge = abs(boundingRect().topRight().x() - boundingRect().topLeft().x());
        double margin = blockTopEdge / (ports.count());

        for (int i = 1; i < ports.count()+1; i++){
            Port *port = ports[i-1];

            if (port->isInputPort()){
                port->setRect(x()+(i*margin),y()-5,10,10);
            } else{
                double blockHeight = abs(boundingRect().topRight().y() - boundingRect().bottomRight().y());
                port->setRect(x()+(blockTopEdge/2),y()+blockHeight-5,10,10);
            }


        }
        foreach (Port *port, ports ) {
            Wire *wire = port->getWire();
            if ( wire != NULL)
                wire->updatePosition();
        }
    }
    return value;
}

