#ifndef BLOCKEDITORSCENE_H
#define BLOCKEDITORSCENE_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QtCore>
#include "block.h"
#include "port.h"

class BlockEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode {InsertBlock, MoveBlock, InsertWire};
    explicit BlockEditorScene(QObject *parent=0);
    void setSceneChanged(bool val);
    bool getSceneChanged();

    QList<Block *> getBlocks();
    QList<Wire *> getWires();
    Port *emphPort(Port *port=NULL);
    void unEmphPort();


public slots:
    void setMode(Mode mode);
    void setBlockType(Block::BlockType blockType);
    void setBlockInputs(int inputs);

signals:
    void blockInserted(Block *block);

private:
    Mode mode;
    Block::BlockType blockType;
    QGraphicsLineItem *wire;
    int blockInputs;

    bool sceneChanged;

    Port *emphasizedPort;


protected:
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
};

#endif // BLOCKEDITORSCENE_H
