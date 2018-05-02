#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "port.h"

class Port;



class Block : public QGraphicsPixmapItem
{
public:
    enum { Type = UserType + 15};
    enum BlockType {addBlock, subBlock, mulBlock, divBlock};
    Block(BlockType blockType, QGraphicsItem *parent = 0);
    ~Block();
    BlockType getBlockType();
    Port *getOutPort();
    void addPort(Port *port);
    void setProcessed();
    void setNotProcessed();

    bool isToSkip();
    void skip();
    void unSkip();

    bool isProcessed();
    void emph();
    void unEmph();

    QList<Port*> getPorts();

    int type() const Q_DECL_OVERRIDE {return Type;}
    QList<Port*> ports;

private:

    bool processed;
    bool toSkip;
    BlockType blockType;


protected:


    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
};

#endif // BLOCK_H
