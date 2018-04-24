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

QVariant Block::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){
        qInfo() << "--(block)block pos changed x: " << x() << " y:" << y();
        QPointF mapscene(x(),y());
        mapscene = mapToScene(mapscene);
        qInfo() << "--(block)block pos mapped to scene x: " << mapscene.x() << " y:" << mapscene.y();


        double blockTopEdge = abs(boundingRect().topRight().x() - boundingRect().topLeft().x());
        qInfo() << "--(block)block top edge : " << blockTopEdge;

        double margin = blockTopEdge / (ports.count()+1);

        for (int i = 1; i < ports.count()+1; i++){
            Port *port = ports[i-1];
            qInfo() << "setting port pos(" << x()+(i*margin) << "," << y()-5 << ")" ;
            port->setRect(x()+(i*margin),y()-5,10,10);

            qInfo() << "port xy: (" << port->boundingRect().topLeft().x() << "," << port->boundingRect().topLeft().y() << ")";




        }
        foreach (Port *port, ports ) {
            if (port->wire != nullptr)
                port->wire->updatePosition();
        }
    }
    return value;
}

