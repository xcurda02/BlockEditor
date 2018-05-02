#include <QDebug>
#include <QBrush>
#include <QPainter>
#include "wire.h"
#include "block.h"

class QPainter;

Block::Block(BlockType blockType, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{

    this->blockType =  blockType;

    unEmph();

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    qInfo() << "block created";
    processed = false;
    toSkip = false;
}
Block::~Block(){
    foreach (Port *port, ports) {
        delete port;
    }
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

void Block::emph(){
    QPixmap pixmap;
    switch(blockType){
        case addBlock:
            pixmap = QPixmap(":/images/images/emphAddBlock.png");
            break;
        case subBlock:
            pixmap = QPixmap(":/images/images/emphSubBlock.png");
            break;
        case mulBlock:
            pixmap = QPixmap(":/images/images/emphMulBlock.png");
            break;
        case divBlock:
            pixmap = QPixmap(":/images/images/emphDivBlock.png");
            break;
    }
    QPixmap scaled_pix = pixmap.scaled(100,100);
    setPixmap(scaled_pix);
}

void Block::unEmph(){
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
    QPixmap scaled_pix = pixmap.scaled(100,100);
    setPixmap(scaled_pix);
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

bool Block::isToSkip(){
    return toSkip;
}
void Block::skip(){
    toSkip = true;
}
void Block::unSkip(){
    toSkip = false;
}

QList<Port*> Block::getPorts(){
    return ports;
}

QVariant Block::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){

        double blockTopEdge = abs(boundingRect().topRight().x() - boundingRect().topLeft().x());
        double margin = blockTopEdge / (ports.count());

        for (int i = 1; i < ports.count()+1; i++){
            Port *port = ports[i-1];

            if (port->isInputPort()){
                port->setRect(x()+(i*margin)-5,y()-5,10,10);
            } else{
                double blockHeight = abs(boundingRect().topRight().y() - boundingRect().bottomRight().y());
                port->setRect(x()+(blockTopEdge/2)-5,y()+blockHeight-5,10,10);
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

