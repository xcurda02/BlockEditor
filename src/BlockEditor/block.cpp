////// soubor: block.cpp
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Soubor se definicemi metod tridy Block
#include <QDebug>
#include <QBrush>
#include <QPainter>
#include <cmath>
#include "wire.h"
#include "block.h"

/**
 * @brief Block::Block Konstruktor tridy Block
 * @param blockType     Typ bloku, ktery se vytvori
 * @param parent        Rodicovsky objekt
 */
Block::Block(BlockType blockType, QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    this->blockType =  blockType;
    unEmph();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    processed = false;
    toSkip = false;
}
/**
 * @brief Block::~Block Destruktor tridy Block
 * Stara se o mazani portu
 */
Block::~Block(){
    foreach (Port *port, ports) {
        delete port;
    }
}

/**
 * @brief Block::getBlockType
 * @return  Typ bloku
 */
Block::BlockType Block::getBlockType(){
    return blockType;
}

/**
 * @brief Block::addPort Pridani portu do seznamu portu
 * @param port  Port
 */
void Block::addPort(Port *port){
    ports.append(port);
}

/**
 * @brief Block::getOutPort
 * @return Vystupni port bloku
 */
Port *Block::getOutPort(){
    return ports[ports.count()-1];

}

/**
 * @brief Block::emph Nastaveni obrazku obloku na nezvyraznenou verzi
 */
void Block::emph(){
    QPixmap pixmap;
    switch(blockType){
        case addBlock:
            pixmap = QPixmap(":/images/images/emphAddBlock.png");
            break;
        case subBlock:
            pixmap = QPixmap(":/images/images/emphSubBlock.png");
            break;
        case mulBlock:
            pixmap = QPixmap(":/images/images/emphMulBlock.png");
            break;
        case divBlock:
            pixmap = QPixmap(":/images/images/emphDivBlock.png");
            break;
    }
    QPixmap scaled_pix = pixmap.scaled(100,100);
    setPixmap(scaled_pix);
}

/**
 * @brief Block::unEmph Nastaveni obrazku bloku na zvyraznenou verzi
 */
void Block::unEmph(){
    QPixmap pixmap;
    switch(blockType){
        case addBlock:
            pixmap = QPixmap(":/images/images/addBlock.png");
            break;
        case subBlock:
            pixmap = QPixmap(":/images/images/subBlock.png");
            break;
        case mulBlock:
            pixmap = QPixmap(":/images/images/mulBlock.png");
            break;
        case divBlock:
            pixmap = QPixmap(":/images/images/divBlock.png");
            break;
    }
    QPixmap scaled_pix = pixmap.scaled(100,100);
    setPixmap(scaled_pix);
}

/**
 * @brief Block::setProcessed Nastaveni zpracovani bloku
 */
void Block::setProcessed(){
    processed = true;
}

/**
 * @brief Block::setNotProcessed Odnastaveni bloku
 */
void Block::setNotProcessed(){
    processed = false;
}

/**
 * @brief Block::isProcessed
 * @return true - blok byl jiz zpracovan | false - blok nebyl zpracovan
 */
bool Block::isProcessed(){
    return processed;
}

/**
 * @brief Block::isToSkip
 * @return true - blok bude preskocen | false - nebude preskocen
 */
bool Block::isToSkip(){
    return toSkip;
}

/**
 * @brief Block::skip Nastaveni bloku, aby se preskocil
 */
void Block::skip(){
    toSkip = true;
}

/**
 * @brief Block::unSkip Odnastaveni bloku aby se preskocil
 */
void Block::unSkip(){
    toSkip = false;
}

/**
 * @brief Block::getPorts
 * @return Seznam portu
 */
QList<Port*> Block::getPorts(){
    return ports;
}

/**
 * @brief Block::itemChange Reakce na zmenu objektu bloku na scene
 * Metoda implementuje aktualizaci pozice portu a dratu po pohnuti s blokem
 *
 * @param change    Zmena
 * @param value     Hodnota
 * @return      Hodnota
 */
QVariant Block::itemChange(GraphicsItemChange change, const QVariant & value){
    if(change == QGraphicsItem::ItemPositionChange){

        double blockTopEdge = fabs(boundingRect().topRight().x() - boundingRect().topLeft().x());
        double margin = blockTopEdge / (ports.count());

        for (int i = 1; i < ports.count()+1; i++){
            Port *port = ports[i-1];

            if (port->isInputPort()){
                port->setRect(x()+(i*margin)-5,y()-5,10,10);
            } else{
                double blockHeight = fabs(boundingRect().topRight().y() - boundingRect().bottomRight().y());
                port->setRect(x()+(blockTopEdge/2)-5,y()+blockHeight-5,10,10);
            }


        }
        foreach (Port *port, ports ) {
            Wire *wire = port->getWire();
            if ( wire != NULL)
                wire->updatePosition();
        }
    }
    return value;
}

