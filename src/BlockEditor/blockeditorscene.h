#ifndef BLOCKEDITORSCENE_H
#define BLOCKEDITORSCENE_H

#include <QGraphicsScene>
#include "block.h"

class BlockEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {InsertBlock, InsertWire, MoveBlock};
    explicit BlockEditorScene(QObject *parent=0);


public slots:
    void setMode(Mode mode);
    void setBlockType(Block::BlockType blockType);

signals:
    void blockInserted(Block *block);

private:
    Mode mode;
    Block::BlockType blockType;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BLOCKEDITORSCENE_H
