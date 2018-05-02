
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blockeditorscene.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    scene = new BlockEditorScene(this);
    scene->setSceneRect(QRectF(-200, -200, 400, 400));

    connect(scene, SIGNAL(blockInserted(Block*)),
                this, SLOT(blockInserted(Block*)));

    createToolBox();
    createToolBar();
    scene->setBlockInputs(inputsSpinBox->value());


    view = new QGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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

    calc = new Calculator(scene);

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

    inputsSpinBox = new QSpinBox;
    inputsSpinBox->setValue(2);
    connect(inputsSpinBox, SIGNAL(valueChanged(int)),scene , SLOT (setBlockInputs(int)));
    QLabel *label = new QLabel;
    label->setText(QString("Vstupy:"));

    gridLayout->addWidget(label);
    gridLayout->addWidget(inputsSpinBox);


    gridLayout->setRowStretch(3, 10);
    gridLayout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(gridLayout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Insert Blocks"));

}

//pridano
void MainWindow::pointerGroupClicked(int)
{
    //pro tlacitka Move(ID=6) a Wire(ID=7) v mnozine mode MoveBlock(1) InsertWire(2) proto -5 k ID
    scene->setMode(BlockEditorScene::Mode(pointerTypeGroup->checkedId()-5));
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

    //pridano
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));


    actionTypeGroup = new QButtonGroup(this);
    QToolButton *objStepButton = new QToolButton;
    objStepButton->setText("S");

    QToolButton *objRunButton = new QToolButton;
    objRunButton->setText("R");

    actionTypeGroup->addButton(objStepButton, int(stepButton));
    actionTypeGroup->addButton(objRunButton, int(runButton));

    connect(actionTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(actionTypeGroupClicked(int)));




    toolBar = addToolBar(tr("ToolBar"));
    toolBar->addWidget(objMoveButton);
    toolBar->addWidget(objWireButton);
    toolBar->addWidget(objStepButton);
    toolBar->addWidget(objRunButton);
}

void MainWindow::actionTypeGroupClicked(int button_id){
    if(button_id == stepButton){
        if (calc->oneOutPortUnwired()){
            double result;
            if(!calc->makeStep(result)){
                qInfo() << "byl proveden posledni krok, hodnoty se vraci na default";
                calc->setDefaultItemValues();
            }
            qInfo() << "res:" << result;
            showMsg("Vysledek kroku: " + QString::number(result));

         }else{
            showMsg("Presne jeden vystupni port musi byt nenapojen");
        }
    } else if(button_id == runButton){
        calc->setDefaultItemValues();
        if(calc->noCycles() ){
            if(calc->oneOutPortUnwired()){
                double result;
                while(calc->makeStep(result)){}
                calc->setDefaultItemValues();
                showMsg("final result is: " + QString::number(result));
            }else{
                showMsg("Presne jeden vystupni port musi byt nenapojen");
            }

        } else {
            showMsg("Zjistena smycka ve schematu");
        }
    }

}
void MainWindow::showMsg(QString msg){
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
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
    button->setIconSize(QSize(50,50));
    button->setCheckable(true);
    button->setMinimumSize(50,50);
    blocksButtonGroup->addButton(button, int(buttonType));

    return button;

}

