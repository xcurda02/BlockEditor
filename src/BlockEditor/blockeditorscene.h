#ifndef BLOCKEDITORSCENE_H
#define BLOCKEDITORSCENE_H

#include <QGraphicsScene>
#include "block.h"

class BlockEditorScene : public QGraphicsScene
{
public:
    enum Mode {InsertBlock, InsertWire, MoveItem};


    explicit BlockEditorScene(QObject *parent=0);

private:
    Mode mode;
    Block::BlockType blockType;
};

#endif // BLOCKEDITORSCENE_H
