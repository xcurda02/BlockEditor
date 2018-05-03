#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QtCore>
#include <QInputDialog>
#include <QObject>
#include "blockeditorscene.h"
#include "block.h"
#include "port.h"
#include "wire.h"
#include "mainwindow.h"

#define CANCEL_EXCEPTION 1
#define ZERO_DIV_EXCEPTION 2

QT_BEGIN_NAMESPACE
class BlockEditorScene;
class QGraphicsItem;
class QInputDialog;
class QObject;
QT_END_NAMESPACE

/**
 * @brief Trida Calculator se stara o vypocty hodnot na blocich schematu a kontrolu validity schematu
 */
class Calculator: public QObject
{
 Q_OBJECT
public:
    explicit Calculator(BlockEditorScene *scene, QObject *parent = NULL);
    bool makeStep(double &result);
    bool noCycles();
    bool oneOutPortUnwired();
    void setDefaultItemValues();
signals:

public slots:

private:
    void setBlocksNotProcessed();
    QList<Block *> getLevel1Blocks();
    bool allBlocksProcessed();
    void setDefaultWireValues();
    void unSkipAll();
    Block *getNextBlock();
    double calculate(QList<double> &values, Block::BlockType blockType);
    BlockEditorScene *scene;    ///< Objekt sceny pro ziskavani objektu na scene
};

#endif // CALCULATOR_H
