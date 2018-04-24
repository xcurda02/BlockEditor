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

}

void BlockEditorScene::setMode(Mode mode){
    this->mode = mode;
}

void BlockEditorScene::setBlockType(Block::BlockType blockType){
    this->blockType = blockType;
}

void BlockEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::LeftButton){
        switch(mode){
            case InsertBlock:
        {
                Block *block = new Block(blockType);
                addItem(block);
                block->setPos(mouseEvent->scenePos());
                qInfo() << "block inserted at x:" << mouseEvent->scenePos().x() << " y:" << mouseEvent->scenePos().y();
                qInfo() << "block top left corener x: " << block->boundingRect().topLeft().x() << " y:" << block->boundingRect().topLeft().y();
                qInfo() << "block  x: " << block->x() << " y:" << block->y();


                int portCount = 5;
                double blockTopEdge = abs(block->boundingRect().topRight().x() - block->boundingRect().topLeft().x());
                qInfo() << "--(block)block top edge : " << blockTopEdge;
                double margin = (double) blockTopEdge / (double)(portCount+1);
                for(int i = 1; i < portCount+1; i++){
                    Port *port = new Port(QPointF(mouseEvent->scenePos().x()+(i*margin),mouseEvent->scenePos().y()-5));
                    port->setZValue(1001.0);
                    addItem(port);
                    block->addPort(port);
                }


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
    } else if (mode == MoveBlock){

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

            //konstruktoru wire se predavaji spatne souradnice portu
            printf("x - %f\n",startItem->x());
            printf("y - %f\n",startItem->y());
            printf("xx - %f\n",endItem->x());
            printf("yy - %f\n",endItem->y());
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

