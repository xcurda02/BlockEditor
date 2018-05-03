#include "calculator.h"
/**
 * @brief Calculator::Calculator Konstruktor tridy Calculator
 * @param scene Graficka scena
 * @param paren Rodicovsky objekt
 */
Calculator::Calculator(BlockEditorScene *scene,QObject *parent): QObject(parent)
{
    this->scene = scene;
}

/**
 * @brief Calculator::setDefaultItemValues Nastaveni vychozich hodnot na blocich
 * Metoda je volana po kazdem spusteni schematu, nebo pri nalezeni chyby ve schematu
 */
void Calculator::setDefaultItemValues(){
    setBlocksNotProcessed();
    unSkipAll();
    setDefaultWireValues();
}
/**
 * @brief Calculator::setBlocksNotProcessed Nastaveni vsech bloku jako nezpracovane
 */
void Calculator::setBlocksNotProcessed(){
    QList<Block *> blocks = scene->getBlocks();
    foreach (Block *block, blocks)
        block->setNotProcessed();
}

/**
 * @brief Calculator::setDefaultWireValues Odnastaveni hodnot na dratech
 */
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
 * @brief Calculator::allBlocksProcessed Vraceni informace o tom, jestli vsechny bloky prosly vypoctem
 * @return true - vsechny blocky byly zpracovany, nebo zadne nejsou na scene | false - vsechny bloky nebyly zpracovany
 */
bool Calculator::allBlocksProcessed(){
    QList<Block *> blocks = scene->getBlocks();

    foreach (Block *block, blocks)
        if (!block->isProcessed())
            return false;
    return true;
}

/**
 * @brief getLevel1Blocks Vraceni listu bloku 1. levelu (jejich vstupni porty nejsou napojeny)
 * @return list bloku
 */
QList<Block *> Calculator::getLevel1Blocks(){
    QList<Block *> blocks = scene->getBlocks();
    QList<Block *> level1Blocks;
    foreach (Block *block, blocks){
        bool unwired = true;
        foreach (Port *port, block->getPorts()) {
            if(port->isInputPort()){
                if (port->getWire() != NULL)
                    unwired = false;
            }
        }
        if (unwired)
            level1Blocks.append(block);
    }
    return level1Blocks;
}

/**
 * @brief Calculator::makeStep Provedeni jednoho kroku
 * @param result Vysledek vypoctu
 * @return false - krok byl proveden na poslednim bloku ve schematu | true - krok byl proveden uprostred schematu
 */
bool Calculator::makeStep(double &result){
    Block *block;               // Blok, na kterem bude provaden vypocet
    QList<double> inputValues;  // Seznam vstupnich hodnot
    unSkipAll();                // Nastaveni bloku aby se nepreskakovaly
    
    while(inputValues.empty()){

        block = getNextBlock();
        foreach (Port *port, block->getPorts()) {

            if(port->isInputPort()){

                if (port->getWire() == NULL){   // Port neni napojen - je potreba zjitit hodnotu pomoci dialogu

                    block->emph();      // Zvyrazneni bloku
                    port->emph();       // Zvyrazneni portu
                    bool ok;
                    /* Ziskani vstupni hodnoty z dialogu */
                    double inputValue = QInputDialog::getDouble(0, tr("QInputDialog::getDouble()"), tr("Amount:"), 37.56, -10000, 10000, 2, &ok);
                    port->unEmph();
                    block->unEmph();

                    if (ok){                    // Stisknuti OK v dialogu
                        inputValues.append(inputValue);
                    } else{                     // Stisknuti Cancel
                        throw CANCEL_EXCEPTION;
                    }

                } else {                        // Port je napojen na drat

                    if (port->getWire()->isValueSet()){     // Na dratu je hodnota
                        inputValues.append(port->getWire()->getValue());
                    } else {                                // Na dratu neni hodnota, blok se preskoci
                        inputValues.clear();
                        block->skip();
                        break;
                    }
                }
            }
        }
    }
    /* Vypocet vstupnich hodnot na bloku */
    result = calculate(inputValues, block->getBlockType() );

    Wire *outWire = block->getOutPort()->getWire();     // Vystupni drat bloku
    block->setProcessed();

    if(outWire != NULL){            //Blok je napojen vystupem na dalsi
        outWire->setValue(result);
        return true;
    } else{                         //Blok neni napojen, konecny vysledek
        return false;
    }
}

/**
 * @brief Calculator::getNextBlock Ziskani dalsiho bloku potencialne vhodneho k vypoctu (blok jeste nebyl zpracovan, nebo neni oznacen na preskoceni)
 * @return  Blok
 */
Block *Calculator::getNextBlock(){
    QList<Block *> blocks = scene->getBlocks();
    foreach (Block *block, blocks)
        if (!block->isProcessed() && !block->isToSkip())
            return block;
}

/**
 * @brief Calculator::calculate Vypocet vstupnich hodnot
 * @param values    vstupni hodnoty
 * @param blockType typ bloku (jeho operace)
 * @return          vysledna hodnota
 */
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
            if (values[i] == 0)
                throw ZERO_DIV_EXCEPTION;
            values[0] /= values[i];
            break;
        }
    }
    return values[0];

}

/**
 * @brief Calculator::isSchemeValid DEtekce smycek ve schematu
 * Prochazeni jednotlivych bloku levelu 1 ke koreni, v pripade pruchodu pres jeden blok dvakrat z jednoho level 1 bloku - detekovana smycka
 *
 * @return true - zadne smycky | false - nalezena smycka
 */
bool Calculator::noCycles(){

    foreach (Block *block, getLevel1Blocks()) {
        Block *iterBlock = block;                           // Promenna bloku ktera bude iterovat skrze schema
        Wire *wire = iterBlock->getOutPort()->getWire();
        while (wire != NULL) {

            iterBlock->setProcessed();                      // Oznaceni bloku jako zpracovany
            iterBlock = wire->getInPort()->getBlock();      // Ziskani dalsiho bloku na konci dratu

            if (iterBlock->isProcessed()){                  // Blok byl jiz zpracovany - smycka
                setBlocksNotProcessed();                    // Nastaveni vsech bloku jako nezpracovane
                return false;
            }

            wire = iterBlock->getOutPort()->getWire();


        }
        setBlocksNotProcessed();                            // Nastaveni vsech bloku jako nezpracovane
    }

    return true;
}

/**
 * @brief Calculator::oneOutPortUnwired Kontrola poctu nenapojenych vystupnich portu
 * Pro navraceni hodnoty true musi schema mit pocet vystupnich portu nenapojenych na drat roven jedne, drat musi byt vzdy spojen s jednim
 * vystupnim a jednim vstupnim portem.
 *
 * @return true - jeden port neni napojen | false - vse ostatni
 */
bool Calculator::oneOutPortUnwired(){

    int outPortsNotWired = 0;
    QList<Block *> blocks = scene->getBlocks();

    foreach (Block *block, blocks){

        if (block->getOutPort()->getWire() == NULL)
            outPortsNotWired++;
    }

    if(outPortsNotWired != 1){
        return false;
    }

    return true;
}

