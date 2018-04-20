#include "blockeditorscene.h"

#include <QGraphicsSceneMouseEvent>


BlockEditorScene::BlockEditorScene(QObject *parent) : QGraphicsScene(parent)
{
    mode = MoveBlock;
    blockType = Block::addBlock;

}

void BlockEditorScene::setMode(Mode mode){
    this->mode = mode;
}

void BlockEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::LeftButton){
        switch(mode){
            case InsertBlock:
                Block *block = new Block(blockType);
                addItem(block);
                block->setPos(mouseEvent->scenePos());
                emit blockInserted(block);
                break;

        }
    }
}
