#include "blockeditorscene.h"
#include "port.h"
#include <QGraphicsSceneMouseEvent>


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
                Port *port = new Port(mouseEvent->scenePos());

                block->setPos(mouseEvent->scenePos());

                addItem(port);
                block->addPort(port);

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
