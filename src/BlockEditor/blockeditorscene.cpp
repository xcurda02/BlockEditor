#include "blockeditorscene.h"
#include "port.h"
#include "wire.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QDebug>


BlockEditorScene::BlockEditorScene(QObject *parent) : QGraphicsScene(parent)
{
    mode = MoveBlock;
    blockType = Block::addBlock;
    emphasizedPort = nullptr;

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
    if(port == nullptr)
        return emphasizedPort;

    port->emph();
    emphasizedPort = port;
    return nullptr;
}

void BlockEditorScene::unEmphPort(){
    if (emphasizedPort){
        emphasizedPort->unEmph();
        emphasizedPort = nullptr;
    }
}

void BlockEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::LeftButton){
        switch(mode){
            case InsertBlock:
        {
                Block *block = new Block(blockType);
                addItem(block);
                block->setPos(mouseEvent->scenePos());


                double blockTopEdge = abs(block->boundingRect().topRight().x() - block->boundingRect().topLeft().x());
                qInfo() << "--(block)block top edge : " << blockTopEdge;
                double margin = (double) blockTopEdge / (double)(blockInputs+1);
                for(int i = 1; i < blockInputs+1; i++){
                    Port *port = new Port(QPointF(mouseEvent->scenePos().x()+(i*margin)-5,mouseEvent->scenePos().y()-5),true, block);
                    port->setZValue(1001.0);
                    addItem(port);
                    block->addPort(port);
                }
                double blockHeight = abs(block->boundingRect().topRight().y() - block->boundingRect().bottomRight().y());
                Port *port = new Port(QPointF(mouseEvent->scenePos().x()+(blockTopEdge/2)-5,mouseEvent->scenePos().y()+blockHeight-5),false, block);
                addItem(port);
                block->addPort(port);


                emit blockInserted(block);

                //pridano po pridani InsertWire. Po vlozeni bloku s nim neslo hybat, ikdyz move tlacitko bylo zaskrknute
                mode = MoveBlock;
                break;
        }
            case InsertWire:
                wire = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                wire->setPen(QPen(Qt::black));
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
        if (itemsHere.count() && itemsHere.first()->type() == Port::Type){
            Port *port = qgraphicsitem_cast<Port *>(itemsHere.first());
            emphPort(port);
        }else{
            unEmphPort();
        }

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

        printf("wire->line().p1().x()");
        printf("x begin - %f\n",wire->line().p1().x());
        printf("x end - %f\n",wire->line().p2().x());

        removeItem(wire);
        delete wire;

        //kontrola jestli cara zacina i konci na portu a pripadne vytvoreni objektu
        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == Port::Type &&
            endItems.first()->type() == Port::Type &&
            startItems.first() != endItems.first()) {

            Port *startItem = qgraphicsitem_cast<Port *>(startItems.first());
            Port *endItem = qgraphicsitem_cast<Port *>(endItems.first());

            Wire *wire = new Wire(startItem, endItem);


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
    wire = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void BlockEditorScene::keyPressEvent(QKeyEvent *keyEvent){
    if (keyEvent->key() == Qt::Key_Delete){

        if(selectedItems().first()->type() == Block::Type){
            qInfo() << "bude se mazat block";
            Block *block = qgraphicsitem_cast<Block *>(selectedItems().first());
            delete block;

        }
    }
}

