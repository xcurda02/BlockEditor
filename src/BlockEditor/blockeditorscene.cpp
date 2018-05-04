////// soubor: blockeditorscene.cpp
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Soubor se definicemi metod tridy BlockEditorScene

#include "blockeditorscene.h"
#include "port.h"
#include "wire.h"
#include "mainwindow.h"
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <cmath>
#include <QDebug>

/**
 * @brief BlockEditorScene::BlockEditorScene Konstruktor BlockEditorScene
 * @param parent Rodicovsky objekt
 */
BlockEditorScene::BlockEditorScene(QObject *parent) : QGraphicsScene(parent)
{
    mode = MoveBlock;
    blockType = Block::addBlock;
    sceneChanged = false;
    emphasizedPort = NULL;

}

/**
 * @brief BlockEditorScene::setMode Nastaveni modu (viz. @ref Mode)
 * @param mode Mod
 */
void BlockEditorScene::setMode(Mode mode){
    this->mode = mode;
}

/**
 * @brief BlockEditorScene::setBlockType Nastaveni typu bloku na vlozeni (viz. @ref Block::BlockType)
 * @param blockType Typ bloku
 */
void BlockEditorScene::setBlockType(Block::BlockType blockType){
    this->blockType = blockType;
}

/**
 * @brief BlockEditorScene::setBlockInputs Nastaveni poctu vstupu nove vkladaneho bloku
 * @param inputs    Pocet vstupu
 */
void BlockEditorScene::setBlockInputs(int inputs){
    blockInputs = inputs;
}

/**
 * @brief BlockEditorScene::setSceneChanged Nastaveni zmeny na scene
 * @param val Zmena
 */
void BlockEditorScene::setSceneChanged(bool val){
    sceneChanged=val;
}

/**
 * @brief BlockEditorScene::getSceneChanged Ziskani informace o zmene na scene
 * @return true - zmena | false - nezmena
 */
bool BlockEditorScene::getSceneChanged(){
    return sceneChanged;
}

/**
 * @brief BlockEditorScene::getBlocks Ziskani vsech bloku na scene
 * @return  List bloku
 */
QList<Block *> BlockEditorScene::getBlocks(){
    QList<Block *> blocks;
    foreach (QGraphicsItem *item, items()) {
        if( item->type() == Block::Type){
            blocks.append(qgraphicsitem_cast<Block *>(item));
        }
    }
    return blocks;
}

/**
 * @brief BlockEditorScene::getWires Ziskani vsech dratu na scene
 * @return List dratu
 */
QList<Wire *> BlockEditorScene::getWires(){
    QList<Wire *> wires;
    foreach (QGraphicsItem *item, items()) {
        if( item->type() == Wire::Type){
            wires.append(qgraphicsitem_cast<Wire *>(item));
        }
    }
    return wires;
}

/**
 * @brief BlockEditorScene::emphPort Zvyrazneni portu
 * @param port  Port
 * @return
 */
Port *BlockEditorScene::emphPort(Port *port){
    if(port == NULL)
        return emphasizedPort;
    unEmphPort();

    port->emph();
    emphasizedPort = port;
    return NULL;
}

/**
 * @brief BlockEditorScene::unEmphPort Vraceni zvyrazneneho portu do normalu
 */
void BlockEditorScene::unEmphPort(){
    if (emphasizedPort){
        emphasizedPort->unEmph();
        emphasizedPort = NULL;
    }

}

/**
 * @brief BlockEditorScene::mousePressEvent Event zmacknuti tlacitka mysi na scene
 * Metoda implementuje pridavani bloku a dratu na scenu
 *
 * @param mouseEvent    Event
 */
void BlockEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::LeftButton){
        switch(mode){
            case InsertBlock:                       // Pridavani bloku
            {
                Block *block = new Block(blockType);
                this->setSceneChanged(true);

                addItem(block);                     // Pridani bloku na scenu
                block->setPos(mouseEvent->scenePos());
                block->setZValue(1000.0);

                /* Vypocet hodnot pro umisteni portu */
                double blockTopEdge = fabs(block->boundingRect().topRight().x() - block->boundingRect().topLeft().x());
                double margin = (double) blockTopEdge / (double)(blockInputs+1);

                /* Pridavani vstupnich portu */
                for(int i = 1; i < blockInputs+1; i++){
                    Port *port = new Port(QPointF(mouseEvent->scenePos().x()+(i*margin)-5,mouseEvent->scenePos().y()-5),true, block);
                    port->setZValue(1001.0);
                    addItem(port);
                    block->addPort(port);
                }

                /* Pridani vystupniho portu */
                double blockHeight = fabs(block->boundingRect().topRight().y() - block->boundingRect().bottomRight().y());
                Port *port = new Port(QPointF(mouseEvent->scenePos().x()+(blockTopEdge/2)-5,mouseEvent->scenePos().y()+blockHeight-5),false, block);
                addItem(port);
                block->addPort(port);

                break;
            }
            case InsertWire:                        // Tvoreni docasne cary reprezentujici drat
                wire = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
                wire->setPen(QPen(Qt::black));
                wire->setZValue(1001.0);
                addItem(wire);
                break;

        }
        QGraphicsScene::mousePressEvent(mouseEvent);
    }

}


/**
 * @brief BlockEditorScene::mouseMoveEvent Event tazeni mysi na scene
 * Metoda implementuje kresleni docasne cary reprezentujici drat behem tazeni mysi.
 * Stara se take o zvyraznovani portu pri tomto pohybu.
 *
 * @param mouseEvent Event
 */
void BlockEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){

    if (mode == InsertWire && wire != NULL){
        QLineF newLine(wire->line().p1(), mouseEvent->scenePos());
        wire->setLine(newLine);

        /* Zvyrazneni portu pokud je nad nim tazena mys */
        bool emphed = false;
        foreach (QGraphicsItem *item, items(mouseEvent->scenePos())) {
            if(item->type() == Port::Type){
                unEmphPort();
                emphPort(qgraphicsitem_cast<Port *>(item));
                emphed = true;
                break;
            }
        }
        if (!emphed)
            unEmphPort();

    } else if (mode == MoveBlock){
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    } else if (mode == InsertWire && wire == NULL){
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}


/**
 * @brief BlockEditorScene::mouseReleaseEvent Event Pusteni tlacitka mysi
 * Metoda implementuje vytvoreni dratu, pokud byl zrovna kreslen a jeho pripojeni k portum
 *
 * @param mouseEvent Event
 */
void BlockEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){

    if (wire != NULL && mode == InsertWire) {

        QList<QGraphicsItem *> startItems = items(wire->line().p1());       // Objekty na scene na zacatku cary
        if (startItems.count() && startItems.first() == wire)
            startItems.removeFirst();                                       // Vymazani dratu ze seznamu

        QList<QGraphicsItem *> endItems = items(wire->line().p2());         // Objekty na scene na konci cary
        if (endItems.count() && endItems.first() == wire)
            endItems.removeFirst();                                         // Vymazani dratu ze seznamu

        /* Vymazani docaneho tahu cary */
        removeItem(wire);
        delete wire;

        /* Kontrola jestli cara zacina i konci na portu a pripadne vytvoreni objektu */
        if (startItems.count() > 0 &&
            endItems.count() > 0 &&
            startItems.first()->type() == Port::Type &&
            endItems.first()->type() == Port::Type &&
            startItems.first() != endItems.first()){                                    //konec podminky

            /* Ziskani portu */
            Port *startItem = qgraphicsitem_cast<Port *>(startItems.first());
            Port *endItem = qgraphicsitem_cast<Port *>(endItems.first());

            /* Overeni, ze se nespojuji dva vstupni, nebo dva vystupni porty a ze porty jiz nejsou napojeny */
            if(startItem->getWire() == NULL && endItem->getWire() == NULL &&
              ((startItem->isInputPort() == true && endItem->isInputPort() == false)||
              (startItem->isInputPort() == false && endItem->isInputPort() == true))){  // konec podminky

                Wire *wire = new Wire(startItem, endItem);
                this->setSceneChanged(true);

                /* Nastaveni dratu do portu */
                startItem->addWire(wire);
                endItem->addWire(wire);


                wire->setZValue(1000.0);        //urcuje, jestli bude wire nad nebo pod ostatnimy prvky ve scene


                addItem(wire);                  //prida wire na scenu
                wire->updatePosition();
            }

        }
    }
    wire = NULL;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/**
 * @brief BlockEditorScene::keyPressEvent Event stisknuti klavesy
 * Metoda implementuje mazani vybraneho bloku pri stisknuti klavesy delete
 *
 * @param keyEvent  Event
 */
void BlockEditorScene::keyPressEvent(QKeyEvent *keyEvent){
    if (keyEvent->key() == Qt::Key_Delete){
        if(selectedItems().count() && selectedItems().first()->type() == Block::Type){

            Block *block = qgraphicsitem_cast<Block *>(selectedItems().first());
            removeItem(block);
            delete block;

        }
    }
}

