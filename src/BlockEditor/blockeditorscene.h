#ifndef BLOCKEDITORSCENE_H
#define BLOCKEDITORSCENE_H

#include <QGraphicsScene>
#include "block.h"
#include "port.h"

class BlockEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {InsertBlock, MoveBlock, InsertWire};
    explicit BlockEditorScene(QObject *parent=0);


public slots:
    void setMode(Mode mode);
    void setBlockType(Block::BlockType blockType);

signals:
    void blockInserted(Block *block);

private:
    Mode mode;
    Block::BlockType blockType;
    QGraphicsLineItem *wire;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
};

#endif // BLOCKEDITORSCENE_H
