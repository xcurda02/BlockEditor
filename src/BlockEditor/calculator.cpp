#include "calculator.h"

Calculator::Calculator(BlockEditorScene *scene,QObject *parent): QObject(parent)
{
    this->scene = scene;
}

void Calculator::setDefaultItemValues(){
    setBlocksNotProcessed();
    unSkipAll();
    setDefaultWireValues();
}
/**
 * @brief Calculator::setBlocksNotProcessed Nastavi vsechny bloky jako nezpracovane
 */
void Calculator::setBlocksNotProcessed(){
    QList<Block *> blocks = scene->getBlocks();
    foreach (Block *block, blocks)
        block->setNotProcessed();
}

void Calculator::setDefaultWireValues(){
    QList<Wire *> wires = scene->getWires();
    foreach (Wire *wire, wires)
        wire->unsetValue();
}

/**
 * @brief Calculator::unSkipAll Vsechny bloky se nastavi, aby se nepreskakovaly
 */
void Calculator::unSkipAll(){
    QList<Block *> blocks = scene->getBlocks();

    foreach (Block *block, blocks)
        block->unSkip();
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
 * @brief getLevel1Blocks Vraci list bloku 1. levelu (jejich vstupni porty nejsou napojeny)
 * @return
 */
QList<Block *> Calculator::getLevel1Blocks(){
    QList<Block *> blocks = scene->getBlocks();
    QList<Block *> level1Blocks;
    foreach (Block *block, blocks){
        bool unwired = true;
        foreach (Port *port, block->getPorts()) {
            if(port->isInputPort()){
                if (port->getWire() != nullptr)
                    unwired = false;
            }
        }
        if (unwired)
            level1Blocks.append(block);
    }
    return level1Blocks;
}

/**
 * @brief Calculator::makeStep Proved jeden krok vypoctu
 * @return
 */
bool Calculator::makeStep(double &result){
    Block *block;
    QList<double> inputValues;
    unSkipAll();
    
    while(inputValues.empty() && ((block = getNextBlock()) != nullptr)){

        foreach (Port *port, block->getPorts()) {
            if(port->isInputPort()){
                if (port->getWire() == nullptr){
                    qInfo() << "port nema wire";
                    //dialog na nacteni hodnoty
                    block->emph();


                    bool ok;
                    port->emph();
                    double d = QInputDialog::getDouble(0, tr("QInputDialog::getDouble()"), tr("Amount:"), 37.56, -10000, 10000, 2, &ok);
                    port->unEmph();
                    block->unEmph();
                    if (ok){
                        inputValues.append(d);
                    } else{
                        throw 20;
                    }

                } else {
                    if (port->getWire()->isValueSet()){
                        qInfo() << "hodnota na wire:" << port->getWire()->getValue();
                        inputValues.append(port->getWire()->getValue());
                    } else {
                        inputValues.clear();
                        block->skip();
                        break;
                    }

                }

            }
        }

    }
    result = calculate(inputValues, block->getBlockType() );
    Wire *outWire = block->getOutPort()->getWire();

    block->setProcessed();

    if(outWire != nullptr){         //Blok je napojen na vystupem na dalsi
        qInfo() << "outWire found, setting output value";
        outWire->setValue(result);
        return true;
    } else{                         //Blok neni napojen, konecny vysledek
        return false;
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

/** Pro navraceni hodnoty true musi schema mit pocet vystupnich portu nenapojenych na drat roven jedne, drat musi byt vzdy spojen s jednim
 * vystupnim a jednim vstupnim portem.
 * @brief Calculator::isSchemeValid Kontrola validity vytvoreneho schematu
 * @return
 */
bool Calculator::noCycles(){
    qInfo() << "zacina kontrola cyklu";
    foreach (Block *block, getLevel1Blocks()) {
        Block *iterBlock = block;
        Wire *wire = iterBlock->getOutPort()->getWire();
        while (wire != nullptr) {
            iterBlock->setProcessed();
            iterBlock = wire->getInPort()->getBlock();
            if (iterBlock->isProcessed()){
                qInfo() << "ERROR: smycka";
                setBlocksNotProcessed();
                return false;
            }
            wire = iterBlock->getOutPort()->getWire();


        }
        setBlocksNotProcessed();
    }

    qInfo() << "cykly nenalezeny";
    return true;
}

/** Pro navraceni hodnoty true musi schema mit pocet vystupnich portu nenapojenych na drat roven jedne, drat musi byt vzdy spojen s jednim
 * vystupnim a jednim vstupnim portem.
 * @brief Calculator::oneOutPortUnwired Kontrola validity vytvoreneho schematu
 * @return
 */
bool Calculator::oneOutPortUnwired(){
    /* Kontrola poctu nenapojenych vystupnich portu */
    int outPortsNotWired = 0;
    QList<Block *> blocks = scene->getBlocks();
    foreach (Block *block, blocks){
        if (block->getOutPort()->getWire() == nullptr)
            outPortsNotWired++;
    }
    if(outPortsNotWired != 1){
        return false;
        qInfo() << "ERROR: vice jak jeden nenapojeny vystupni port";
    }

    return true;
}

