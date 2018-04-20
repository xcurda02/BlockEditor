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
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
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



    gridLayout->addWidget(createBlockButton("+",Buttons::addButton),0,0);
    gridLayout->addWidget(createBlockButton("-",Buttons::subButton),0,1);
    gridLayout->addWidget(createBlockButton("*",Buttons::mulButton),1,0);
    gridLayout->addWidget(createBlockButton("/",Buttons::divButton),1,1);

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

    QToolButton *moveButton = new QToolButton;
    moveButton->setCheckable(true);
    moveButton->setChecked(true);
    moveButton->setText("M");

    QToolButton *wireButton = new QToolButton;
    wireButton->setCheckable(true);
    wireButton->setText("W");

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(moveButton, int(Buttons::moveButton));
    pointerTypeGroup->addButton(wireButton, int(Buttons::wireButton));
     /* TODO connect*/

    QToolButton *stepButton = new QToolButton;
    stepButton->setText("S");

    QToolButton *runButton = new QToolButton;
    runButton->setText("R");

    actionTypeGroup = new QButtonGroup(this);
    actionTypeGroup->addButton(stepButton, int(Buttons::stepButton));
    actionTypeGroup->addButton(runButton, int(Buttons::runButton));





    toolBar = addToolBar(tr("ToolBar"));
    toolBar->addWidget(moveButton);
    toolBar->addWidget(wireButton);
    toolBar->addWidget(stepButton);
    toolBar->addWidget(runButton);
}

void MainWindow::blockButtonClicked(int button_id){

    qInfo() << "button clicked: id" << button_id;
    scene->setMode(BlockEditorScene::InsertBlock);
    scene->setBlockType(Block::BlockType(button_id));
}

void MainWindow::blockInserted(Block *block){
    pointerTypeGroup->button(int(Buttons::moveButton))->setChecked(true);
    scene->setMode(BlockEditorScene::Mode(pointerTypeGroup->checkedId()));
    blocksButtonGroup->button(int(block->getBlockType()))->setChecked(false);
}



QAbstractButton *MainWindow::createBlockButton(const QString &text, Buttons buttonType){

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
