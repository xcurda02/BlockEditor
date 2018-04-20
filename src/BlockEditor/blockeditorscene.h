#ifndef BLOCKEDITORSCENE_H
#define BLOCKEDITORSCENE_H

#include <QGraphicsScene>


class BlockEditorScene : public QGraphicsScene
{
public:
    enum Mode {InsertBlock, InsertWire, MoveItem};

    explicit BlockEditorScene(QObject *parent=0);

private:
    Mode myMode;
};

#endif // BLOCKEDITORSCENE_H
