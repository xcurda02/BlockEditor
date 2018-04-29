#include "calculator.h"

Calculator::Calculator(BlockEditorScene *scene,QObject *parent): QObject(parent)
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

    foreach (Block *block, blocks)
        if (!block->isProcessed())
            return false;
    return true;
}

/**
 * @brief Calculator::makeStep Proved jeden krok vypoctu
 * @return
 */
double Calculator::makeStep(){
    Block *block;
    QList<double> inputValues;
    
    while(inputValues.empty() && ((block = getNextBlock()) != nullptr)){

        foreach (Port *port, block->getPorts()) {
            if(port->isInputPort()){
                if (port->getWire() == nullptr){
                    //dialog na nacteni hodnoty
                    block->emph();

                    bool ok;
                    double d = QInputDialog::getDouble(0, tr("QInputDialog::getDouble()"), tr("Amount:"), 37.56, -10000, 10000, 2, &ok);
                    if (ok){

                        inputValues.append(d);
                    }

                } else {
                    if (port->getWire()->isValueSet()){
                        inputValues.append(port->getWire()->getValue());
                    } else {
                        inputValues.clear();
                        block->skip();
                        break;
                    }

                }

            }
        }
        if (inputValues.count() > 1){
            double res = calculate(inputValues, block->getBlockType() );
            block->unEmph();
            return res;

        } else{
            block->unEmph();
            return inputValues[0];
        }
    }
    
}

Block *Calculator::getNextBlock(){
    QList<Block *> blocks = scene->getBlocks();
    foreach (Block *block, blocks)
        if (!block->isProcessed() && !block->isToSkip())
            return block;

    return nullptr;
}

double Calculator::calculate(QList<double> &values, Block::BlockType blockType){

    for (int i = 1; i < values.count(); i++){
        switch (blockType) {
        case Block::addBlock:
            values[0] += values[i];
            break;
        case Block::subBlock:
            values[0] -= values[i];

            break;
        case Block::mulBlock:
            values[0] *= values[i];
            break;
        case Block::divBlock:
            values[0] /= values[i];
            break;
        }
    }
    return values[0];

}

