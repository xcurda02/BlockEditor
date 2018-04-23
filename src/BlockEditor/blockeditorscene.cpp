#include "blockeditorscene.h"
#include "port.h"
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
                    addItem(port);
                    block->addPort(port);
                }


                emit blockInserted(block);
                break;
            /*case InsertWire:
                wire = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                wire->setPen(QPen(QColor::black()));
                addItem(wire);
                break;*/

        }
        QGraphicsScene::mousePressEvent(mouseEvent);
    }

}
/*
void BlockEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mode == InsertWire && wire != nullptr){
        wire->setLine(wire->line()->x1, wire->line()->y1, mouseEvent->scenePos()->x, mouseEvent->scenePos()->y);
    } else if (mode == MoveBlock){
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}*/
