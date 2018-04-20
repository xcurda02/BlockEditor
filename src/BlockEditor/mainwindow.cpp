#include <QtWidgets>
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

void MainWindow::createToolBox(){
    blocksButtonGroup = new QButtonGroup(this);
    blocksButtonGroup->setExclusive(false);
    /* TODO: osetrit klikani */

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
/*
void MainWindow::blockButtonClicked(int button){

}*/



QAbstractButton *MainWindow::createBlockButton(const QString &text, Buttons buttonType){

    QToolButton *button = new QToolButton;

    button->setText(text);
    button->setIconSize(QSize(50,50));
    button->setCheckable(true);
    button->setMinimumSize(60,60);
    blocksButtonGroup->addButton(button, int(buttonType));

    return button;

}
