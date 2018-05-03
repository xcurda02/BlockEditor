#include "blockeditorscene.h"
#include "port.h"
#include "wire.h"
#include "mainwindow.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <cmath>
#include <QDebug>


BlockEditorScene::BlockEditorScene(QObject *parent) : QGraphicsScene(parent)
{
    mode = MoveBlock;
    blockType = Block::addBlock;
    sceneChanged = false;
    emphasizedPort = NULL;

}

void BlockEditorScene::setMode(Mode mode){
    this->mode = mode;
}

void BlockEditorScene::setBlockType(Block::BlockType blockType){
    this->blockType = blockType;
}
void BlockEditorScene::setBlockInputs(int inputs){
    blockInputs = inputs;
}


void BlockEditorScene::addBlock(Block *block){
    blocks.append(block);
}
void BlockEditorScene::setSceneChanged(bool val){
    sceneChanged=val;
}
bool BlockEditorScene::getSceneChanged(){
    return sceneChanged;
}

QList<Block *> BlockEditorScene::getBlocks(){
    QList<Block *> blocks;
    foreach (QGraphicsItem *item, items()) {
        if( item->type() == Block::Type){
            blocks.append(qgraphicsitem_cast<Block *>(item));
        }
    }
    return blocks;
}
QList<Wire *> BlockEditorScene::getWires(){
    QList<Wire *> wires;
    foreach (QGraphicsItem *item, items()) {
        if( item->type() == Wire::Type){
            wires.append(qgraphicsitem_cast<Wire *>(item));
        }
    }
    return wires;
}

Port *BlockEditorScene::emphPort(Port *port){
    if(port == NULL)
        return emphasizedPort;
    unEmphPort();

    port->emph();
    emphasizedPort = port;
    return NULL;
}

void BlockEditorScene::unEmphPort(){
    if (emphasizedPort){
        emphasizedPort->unEmph();
        emphasizedPort = NULL;
    }

}

void BlockEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::LeftButton){
        switch(mode){
            case InsertBlock:
        {
                Block *block = new Block(blockType);
                this->setSceneChanged(true);
                addBlock(block);
                addItem(block);
                block->setPos(mouseEvent->scenePos());
                block->setZValue(1000.0);


                double blockTopEdge = fabs(block->boundingRect().topRight().x() - block->boundingRect().topLeft().x());
                qInfo() << "--(block)block top edge : " << blockTopEdge;
                double margin = (double) blockTopEdge / (double)(blockInputs+1);
                for(int i = 1; i < blockInputs+1; i++){
                    Port *port = new Port(QPointF(mouseEvent->scenePos().x()+(i*margin)-5,mouseEvent->scenePos().y()-5),true, block);
                    port->setZValue(1001.0);
                    addItem(port);
                    block->addPort(port);
                }
                double blockHeight = fabs(block->boundingRect().topRight().y() - block->boundingRect().bottomRight().y());


                Port *port = new Port(QPointF(mouseEvent->scenePos().x()+(blockTopEdge/2)-5,mouseEvent->scenePos().y()+blockHeight-5),false, block);

                addItem(port);
                block->addPort(port);


                emit blockInserted(block);

                //pridano po pridani InsertWire. Po vlozeni bloku s nim neslo hybat, ikdyz move tlacitko bylo zaskrknute

                break;
        }
            case InsertWire:
                wire = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                wire->setPen(QPen(Qt::black));
                wire->setZValue(1001.0);
                addItem(wire);
                break;

        }
        QGraphicsScene::mousePressEvent(mouseEvent);
    }

}


//kresleni cary dokud drzim tlacitko mysi
void BlockEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mode == InsertWire && wire != NULL){//predelano z nullptr na NULL
        QLineF newLine(wire->line().p1(), mouseEvent->scenePos());
        wire->setLine(newLine);

        QList<QGraphicsItem *> itemsHere = items(mouseEvent->scenePos());
        qInfo() << "Items here count:" << itemsHere.count();

        bool emphed = false;
        foreach (QGraphicsItem *item, itemsHere) {
            if(item->type() == Port::Type){
                Port *port = qgraphicsitem_cast<Port *>(item);
                unEmphPort();
                emphPort(port);
                emphed = true;
                break;
            }
        }
        if (!emphed)
            unEmphPort();

        /*
        if (itemsHere.count() && itemsHere.first()->type() == Port::Type){
            Port *port = qgraphicsitem_cast<Port *>(itemsHere.first());
            emphPort(port);
        }else{
            qInfo() << "unemph port";
            unEmphPort();
        }*/

    } else if (mode == MoveBlock){
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    } else if (mode == InsertWire && wire == NULL){
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}


//chyba v predavani parametru endItem a startItem
//zkopirovano a upraveno z ukazky v QT
void BlockEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (wire != 0 && mode == InsertWire) {
        QList<QGraphicsItem *> startItems = items(wire->line().p1());
        if (startItems.count() && startItems.first() == wire)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(wire->line().p2());
        if (endItems.count() && endItems.first() == wire)
            endItems.removeFirst();

        removeItem(wire);
        delete wire;
        //kontrola jestli cara zacina i konci na portu a pripadne vytvoreni objektu
        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == Port::Type &&
            endItems.first()->type() == Port::Type &&
            startItems.first() != endItems.first() ) {

            Port *startItem = qgraphicsitem_cast<Port *>(startItems.first());
            Port *endItem = qgraphicsitem_cast<Port *>(endItems.first());
            printf("%d %d\n",startItem->isInputPort(), endItem->isInputPort());
            if(startItem->getWire()== NULL && endItem->getWire() == NULL &&
                    ((startItem->isInputPort() == true && endItem->isInputPort() == false)  ||
                     (startItem->isInputPort() == false && endItem->isInputPort() == true))){
                Wire *wire = new Wire(startItem, endItem);
                this->setSceneChanged(true);
                //ulozi objekt Wire do seznamu (nemusi to byt seznam - jeden port = jeden Wire)
                startItem->addWire(wire);
                endItem->addWire(wire);

                //urcuje, jestli bude wire nad nebo pod ostatnimy prvky v GrScene
                wire->setZValue(1000.0);

                //prida wire na GrScene
                addItem(wire);
                wire->updatePosition();
            }

        }
    }
    wire = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void BlockEditorScene::keyPressEvent(QKeyEvent *keyEvent){
    if (keyEvent->key() == Qt::Key_Delete){

        if(selectedItems().count() && selectedItems().first()->type() == Block::Type){
            qInfo() << "bude se mazat block";
            Block *block = qgraphicsitem_cast<Block *>(selectedItems().first());
            delete block;

        }
    }
}

