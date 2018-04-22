#include <QtWidgets>
#include <QTextStream>
#include <QtCore>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blockeditorscene.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createToolBox();
    createToolBar();
    scene = new BlockEditorScene(this);
    scene->setSceneRect(QRectF(0, 0, 500, 500));
    connect(scene, SIGNAL(blockInserted(Block*)),
                this, SLOT(blockInserted(Block*)));

    view = new QGraphicsView(scene);
    QHBoxLayout *layout = new QHBoxLayout;

    QVBoxLayout *vlayout = new QVBoxLayout;

    vlayout->addWidget(toolBar);
    vlayout->addWidget(toolBox);




    QWidget *leftPanelWidget = new QWidget;
    leftPanelWidget->setLayout(vlayout);



    layout->addWidget(leftPanelWidget);
    layout->addWidget(view);


    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);

}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief MainWindow::createToolBox
 */
void MainWindow::createToolBox(){
    blocksButtonGroup = new QButtonGroup(this);
    //blocksButtonGroup->setExclusive(false);
    /* TODO: osetrit klikani */
    connect(blocksButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(blockButtonClicked(int)));

    QGridLayout *gridLayout = new QGridLayout;



    gridLayout->addWidget(createBlockButton("+",addButton),0,0);
    gridLayout->addWidget(createBlockButton("-",subButton),0,1);
    gridLayout->addWidget(createBlockButton("*",mulButton),1,0);
    gridLayout->addWidget(createBlockButton("/",divButton),1,1);

    gridLayout->setRowStretch(3, 10);
    gridLayout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(gridLayout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Insert Blocks"));

}

void MainWindow::createToolBar(){

    QToolButton *objMoveButton = new QToolButton;
    objMoveButton->setCheckable(true);
    objMoveButton->setChecked(true);
    objMoveButton->setText("M");

    QToolButton *objWireButton = new QToolButton;
    objWireButton->setCheckable(true);
    objWireButton->setText("W");

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(objMoveButton, int(moveButton));
    pointerTypeGroup->addButton(objWireButton, int(wireButton));
     /* TODO connect*/

    QToolButton *objStepButton = new QToolButton;
    objStepButton->setText("S");

    QToolButton *objRunButton = new QToolButton;
    objRunButton->setText("R");

    actionTypeGroup = new QButtonGroup(this);
    actionTypeGroup->addButton(objStepButton, int(stepButton));
    actionTypeGroup->addButton(objRunButton, int(runButton));





    toolBar = addToolBar(tr("ToolBar"));
    toolBar->addWidget(objMoveButton);
    toolBar->addWidget(objWireButton);
    toolBar->addWidget(objStepButton);
    toolBar->addWidget(objRunButton);
}

void MainWindow::blockButtonClicked(int button_id){

    qInfo() << "button clicked: id" << button_id;
    scene->setMode(BlockEditorScene::InsertBlock);
    scene->setBlockType(Block::BlockType(button_id));
}

void MainWindow::blockInserted(Block *block){
    qInfo() << "scene items: " << scene->items().count();
    pointerTypeGroup->button(int(moveButton))->setChecked(true);
    scene->setMode(BlockEditorScene::Mode(pointerTypeGroup->checkedId()));
    blocksButtonGroup->button(int(block->getBlockType()))->setChecked(false);
}



QAbstractButton *MainWindow::createBlockButton(const QString &text, int buttonType){

    QToolButton *button = new QToolButton;



    QPixmap pixmap;
    switch(buttonType){
        case addButton:
            pixmap = QPixmap(":/images/images/addBlock.png");
            break;
        case subButton:
            pixmap = QPixmap(":/images/images/subBlock.png");
            break;
        case mulButton:
            pixmap = QPixmap(":/images/images/mulBlock.png");
            break;
        case divButton:
            pixmap = QPixmap(":/images/images/divBlock.png");
            break;
    }

    QIcon ButtonIcon(pixmap);
    button->setIcon(ButtonIcon);
    button->setIconSize(QSize(65,65));
    button->setCheckable(true);
    button->setMinimumSize(60,60);
    blocksButtonGroup->addButton(button, int(buttonType));

    return button;

}
