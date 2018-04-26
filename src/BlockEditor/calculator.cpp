#include "calculator.h"

Calculator::Calculator(BlockEditorScene *scene,QWidget *parent)
{
    this->scene = scene;
}

/**
 * @brief Calculator::setBlocksNotProcessed Nastavi vsechny bloky jako nezpracovane
 */
void Calculator::setBlocksNotProcessed(){
    QList<Block *> blocks = scene->getBlocks();
    foreach (Block *block, blocks)
        block->setNotProcessed();
}

/**
 * @brief Calculator::allBlocksProcessed
 * @return vraci informaci o tom, jestli vsechny bloky prosly vypoctem, v pripade neexistence bloku vraci true
 */
bool Calculator::allBlocksProcessed(){
    QList<Block *> blocks = scene->getBlocks();

    if (blocks.empty())
        return true;

    foreach (Block *block, blocks)
        if (!block->isProcessed())
            return false;
    return true;
}


