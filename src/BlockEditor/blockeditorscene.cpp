#include "blockeditorscene.h"

BlockEditorScene::BlockEditorScene(QObject *parent) : QGraphicsScene(parent)
{
    mode = MoveItem;
    blockType = Block::addBlock;

}
