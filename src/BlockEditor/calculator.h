#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <QtCore>
#include "blockeditorscene.h"
#include "block.h"

QT_BEGIN_NAMESPACE
class BlockEditorScene;
class QGraphicsItem;
QT_END_NAMESPACE

class Calculator
{

public:
    explicit Calculator(BlockEditorScene *scene, QWidget *parent = nullptr);
    void setBlocksNotProcessed();
    bool allBlocksProcessed();

signals:

public slots:

private:
    BlockEditorScene *scene;
};

#endif // CALCULATOR_H
