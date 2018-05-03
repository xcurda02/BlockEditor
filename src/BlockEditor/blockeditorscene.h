////// soubor: blockeditorscene.h
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Hlavickovy soubor tridy BlockEditorScene

#ifndef BLOCKEDITORSCENE_H
#define BLOCKEDITORSCENE_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QtCore>
#include "block.h"
#include "port.h"

/**
 * @brief The BlockEditorScene Trida sceny
 * Dedi od Qt tridy QGraphicsScene
 * Stara se o pridavani a editace prvku na scene
 */
class BlockEditorScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /**
     * @brief enum Mode
     * Znaci mod sceny - co se stane pri stistnuti tlacitka mysi
     */
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
    Mode mode;                      ///< Aktualni mod sceny
    Block::BlockType blockType;     ///< Typ bloku, ktery se ma pridavat
    QGraphicsLineItem *wire;        ///< Docasny drat
    int blockInputs;                ///< Pocet vstupu pro blok ktery se ma pridavat
    bool sceneChanged;              ///< Zmena an scene
    Port *emphasizedPort;           ///< Port, ktery je zrovna zvyraznen

protected:
    void keyPressEvent(QKeyEvent *keyEvent) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
};

#endif // BLOCKEDITORSCENE_H
