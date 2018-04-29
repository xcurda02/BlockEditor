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

QT_BEGIN_NAMESPACE
class BlockEditorScene;
class QGraphicsItem;
class QInputDialog;
class QObject;

QT_END_NAMESPACE

class Calculator: public QObject
{
 Q_OBJECT
public:
    explicit Calculator(BlockEditorScene *scene, QObject *parent = nullptr);
    
    double makeStep();

signals:

public slots:

private:
    void setBlocksNotProcessed();
    bool allBlocksProcessed();
    Block *getNextBlock();
    double calculate(QList<double> &values, Block::BlockType blockType);
    BlockEditorScene *scene;
};

#endif // CALCULATOR_H
