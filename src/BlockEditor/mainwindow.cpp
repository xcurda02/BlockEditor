////// soubor: mainwindow.cpp
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Soubor se definicemi metod tridy Mainwindow
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blockeditorscene.h"
#include <QGraphicsScene>
#include <cmath>


/**
 * @brief MainWindow::MainWindow Konstruktor tridy Mainwindow
 * Umisteni widgetu do okna
 * @param parent    Rodicovsky objekt
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    createActions();
    createMenus();
    scene = new BlockEditorScene(this);
    scene->setSceneRect(QRectF(-200, -200, 400, 400));

    createToolBox();
    createToolbar();

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

/**
 * @brief MainWindow::~MainWindow Destruktor tridy Mainwindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief MainWindow::createToolBox Vytvoreni Toolboxu - Tlacitka pro bloky a nastaveni poctu vstupu
 */
void MainWindow::createToolBox(){
    blocksButtonGroup = new QButtonGroup(this);
    connect(blocksButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(blockButtonClicked(int)));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(createBlockButton(addButton),0,0);
    gridLayout->addWidget(createBlockButton(subButton),0,1);
    gridLayout->addWidget(createBlockButton(mulButton),1,0);
    gridLayout->addWidget(createBlockButton(divButton),1,1);

    inputsSpinBox = new QSpinBox;
    inputsSpinBox->setValue(2);
    connect(inputsSpinBox, SIGNAL(valueChanged(int)),scene , SLOT (setBlockInputs(int)));

    QLabel *label = new QLabel;
    label->setText(QString("Inputs:"));

    gridLayout->addWidget(label);
    gridLayout->addWidget(inputsSpinBox);
    gridLayout->addWidget(createBlockButton(invBButton));


    gridLayout->setRowStretch(3, 10);
    gridLayout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(gridLayout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Insert Blocks"));
}

/**
 * @brief MainWindow::createToolbar Vytvoreni toolbaru - Tlacitka Move, Wire, Step, Run
 */
void MainWindow::createToolbar(){



    QToolButton *objMoveButton = createToolbarButton(moveButton);
    QToolButton *objWireButton = createToolbarButton(wireButton);
    QToolButton *objInvButton = createToolbarButton(invAButton);
    QToolButton *objStepButton = createToolbarButton(stepButton);
    QToolButton *objRunButton = createToolbarButton(runButton);



    toolbarButtonGroup = new QButtonGroup(this);
    toolbarButtonGroup->addButton(objMoveButton, int(moveButton));
    toolbarButtonGroup->addButton(objWireButton, int(wireButton));
    toolbarButtonGroup->addButton(objInvButton, int(invAButton));
    toolbarButtonGroup->addButton(objStepButton, int(stepButton));
    toolbarButtonGroup->addButton(objRunButton, int(runButton));

    connect(toolbarButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(toolbarButtonGroupClicked(int)));

    toolBar = addToolBar(tr("ToolBar"));
    toolBar->addWidget(objMoveButton);
    toolBar->addWidget(objWireButton);
    toolBar->addWidget(objInvButton);
    toolBar->addWidget(objStepButton);
    toolBar->addWidget(objRunButton);
}

/**
 * @brief MainWindow::toolbarButtonGroupClicked Kliknuto tlacitko toolbaru
 * @param button_id     Id tlacitka
 */
void MainWindow::toolbarButtonGroupClicked(int button_id){
    if(button_id == stepButton){            // Tlacitko Step
        if (calc->oneOutPortUnwired()){     // Kontrola, ze je jeden vystupni port nenapojen
            double result;
            bool exc = false;
            try{
                if(!calc->makeStep(result)){
                    calc->setDefaultItemValues();       // Nastaveni hodnot na objektu na scene na vychozi
                }
            }catch(int e){
                if(e == CANCEL_EXCEPTION){              // Zmacknut Cancel pri zadavani hodnoty
                    qInfo() << "Pressed Cancel:" << result;
                } else if(e == ZERO_DIV_EXCEPTION) {    // Deleni nulou
                    calc->setDefaultItemValues();
                    showMsg("Dividing by zero");
                }
                exc = true;
            }
            if(!exc){
                showMsg("Step result: " + QString::number(result));
            }
         }else
            showMsg("Exactly one output port needs to be unwired");

    } else if(button_id == runButton){      // Tlacitko Run
        calc->setDefaultItemValues();
        if(calc->noCycles() ){              // Kontrola smycek
            if(calc->oneOutPortUnwired()){  // Kontrola, ze je jeden vystupni port nenapojen
                double result;
                bool exc = false;
                try{
                    while(calc->makeStep(result)){}         // Provadeni kroku
                }catch(int e){
                    if(e == CANCEL_EXCEPTION){
                        qInfo() << "Pressed Cancel:" << result;
                    } else if(e == ZERO_DIV_EXCEPTION) {
                        showMsg("Dividing by zero");
                    }
                    exc = true;
                }
                calc->setDefaultItemValues();
                if (!exc){
                    showMsg("Final result: " + QString::number(result));
                }

            }else{
                showMsg("Exactly one output port needs to be unwired");
            }

        } else {
            showMsg("A cycle in schema detected");
        }
    }else{
        /* Pro tlacitka Move(ID=6) a Wire(ID=7) v mnozine mode MoveBlock(1) InsertWire(2) proto -5 k ID */
        scene->setMode(BlockEditorScene::Mode(toolbarButtonGroup->checkedId()-5));
        blocksButtonGroup->button(int(invBButton))->setChecked(true);
    }
}

/**
 * @brief MainWindow::showMsg Vypsani zpravy v dialogovem okne
 * @param msg   Retezec zpravy
 */
void MainWindow::showMsg(QString msg){
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

/**
 * @brief MainWindow::blockButtonClicked Zmacknuto tlacitko bloku
 * Nastaveni modu sceny a typu bloku
 * @param button_id     Id tlacitka
 */
void MainWindow::blockButtonClicked(int button_id){
    scene->setMode(BlockEditorScene::InsertBlock);
    scene->setBlockType(Block::BlockType(button_id));
    toolbarButtonGroup->button(int(invAButton))->setChecked(true);
}

/**
 * @brief MainWindow::createToolbarButton Vytvoreni tlacitka toolbaru
 * @param buttonType    Typ tlacitka (viz. @ref Button::ButtonType)
 * @return Tlacitko
 */
QToolButton *MainWindow::createToolbarButton(ButtonType buttonType){
    QToolButton *button = new QToolButton;
    QPixmap pixmap;
    switch(buttonType){
        case moveButton:
            pixmap = QPixmap(":/images/images/move.png");
            button->setCheckable(true);
            button->setToolTip(QString("Move"));
            break;
        case wireButton:
            pixmap = QPixmap(":/images/images/wire.png");
            button->setCheckable(true);
            button->setToolTip(QString("Wire"));
            break;
        case stepButton:
            pixmap = QPixmap(":/images/images/step.png");
            button->setToolTip(QString("Step"));
            break;
        case runButton:
            pixmap = QPixmap(":/images/images/run.png");
            button->setToolTip(QString("Run"));
            break;
        case invAButton:
            pixmap = QPixmap(":/images/images/run.png");
            button->setCheckable(true);
            button->hide();
            break;
    }

    QIcon ButtonIcon(pixmap);
    button->setIcon(ButtonIcon);
    button->setIconSize(QSize(30,30));
    button->setMinimumSize(30,30);

    return button;
}

/**
 * @brief MainWindow::createBlockButton Vytvoreni tlacitka bloku
 * @param buttonType     Typ tlacitka (viz. @ref Button::ButtonType)
 * @return Tlacitko
 */
QAbstractButton *MainWindow::createBlockButton(ButtonType buttonType){
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
        case invBButton:
            pixmap = QPixmap(":/images/images/divBlock.png");
            button->hide();
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

//! úprava close event
/*!
    \brief Před ukončením programu proběhne dotaz na uložení, pokud byly provedeny změny ve schématu.
*/
void MainWindow::closeEvent(QCloseEvent *event){
    if(scene->getSceneChanged()){
        event->ignore();
        if (QMessageBox::Yes == QMessageBox::question(this, "Save", "Do you want to save changes before exit?", QMessageBox::Yes | QMessageBox::No)){
            this->save();
        }
    }
    event->accept();
}


//! Vytvoření akci pro vrchní menu
/*!
*/
void MainWindow::createActions()
{
    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    saveAction = new QAction(tr("&Open"), this);
    saveAction->setShortcut(tr("Ctrl+O"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(open()));

    openAction = new QAction(tr("&Save"), this);
    openAction->setShortcut(tr("Ctrl+S"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(save()));
}

//!Vytvoření vrchního menu
/*!
*/
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(openAction);


    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

//! MessageBox About
/*!
*/
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Block Editor"),
                       tr("This is Block Editor."));
}

//! MessageBox Save
/*!
 \brief akce uložení schématu.
*/
void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("Save Block Scheme"), "",
           tr("Block Scheme (*.bsch)"));

    if (fileName.isEmpty()) {
        return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
            return;
        }
        //zapis do souboru
        QList<Block*> blocks = scene->getBlocks();
        for (int i = 0; i != blocks.length(); ++i){
            int blockType;
            if (blocks[i]->getBlockType() == Block::addBlock)
                blockType = 0;
            else if (blocks[i]->getBlockType() == Block::subBlock)
                blockType = 1;
            else if (blocks[i]->getBlockType() == Block::mulBlock)
                blockType = 2;
            else if (blocks[i]->getBlockType() == Block::divBlock)
                blockType = 3;
            QString text;
            QTextStream outputStream(&file);
            outputStream << i;
            outputStream << ";";
            outputStream << blockType;
            outputStream << ";";
            outputStream << blocks[i]->x();
            outputStream << ",";
            outputStream << blocks[i]->y();
            outputStream << ";";
            outputStream << blocks[i]->ports.count()-1;
            outputStream << ";";
            for(int j=0; j != blocks[i]->ports.length()-1; ++j){
                if (blocks[i]->ports[j]->getWire()!= NULL){
                    outputStream << "w";
                    int blockNum;
                    for(int k=0; k!=blocks.length();++k){
                        if(blocks[k]->getOutPort()==blocks[i]->ports[j]->getWire()->getEndItem() || blocks[k]->getOutPort()==blocks[i]->ports[j]->getWire()->getStartItem()){
                            blockNum=k;
                        }
                    }
                    outputStream << blockNum;
                    outputStream << ";";
                } else {
                    outputStream << "NULL";
                    outputStream << ";";
                }

            }
            outputStream << "\n";
        }
        scene->setSceneChanged(false);
    }
}


//! MessageBox Open
/*!
 \brief akce otevření schématu.
*/
void MainWindow::open()
{
    if(scene->getSceneChanged()){
        if (QMessageBox::Yes == QMessageBox::question(this, "Save", "Do you want to save changes?", QMessageBox::Yes | QMessageBox::No)){
            this->save();
        }
    }
    //ziskani jmena souboru
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Block Scheme"), "",
        tr("Block Scheme (*.bsch)"));

    if (fileName.isEmpty()){
        return;
    } else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QList<Block*> blocks=scene->getBlocks();
        for(int i = 0;i<blocks.length();++i){
            delete blocks[i];
        }
        blocks.clear();
        //destruktor na bloky?
        QTextStream in(&file);
        //cteni souboru po radcich (co radek to jeden blok)
        while (!in.atEnd()){
            QString line = in.readLine();;
            //parsovani stringu
            QRegExp rx("[,;]");
            QStringList list = line.split(rx, QString::SkipEmptyParts);

            //pridani blocku
           Block::BlockType blockType;
            switch(list[1].toInt()){
            case 0:
                blockType =  Block::addBlock;
                break;
            case 1:
                blockType =  Block::subBlock;
                break;
            case 2:
                blockType =  Block::mulBlock;
                break;
            case 3:
                blockType =  Block::divBlock;
                break;

            }
            Block *block = new Block(blockType);
            scene->addItem(block);
            bool ok;
            qreal x = list[2].toDouble(&ok); qreal y = list[3].toDouble(&ok);
            block->setPos(x,y);
            block->setZValue(1000.0);

            double blockTopEdge = fabs(block->boundingRect().topRight().x() - block->boundingRect().topLeft().x());
            double margin = (double) blockTopEdge / (double)(list[4].toDouble(&ok)+1);
            for(int i = 1; i < list[4].toInt()+1; i++){
                Port *port = new Port(QPointF(x+(i*margin),y-5),true,block);
                port->setZValue(1001.0);
                scene->addItem(port);
                block->addPort(port);
            }
            double blockHeight = fabs(block->boundingRect().topRight().y() - block->boundingRect().bottomRight().y());
            Port *port = new Port(QPointF(x+(blockTopEdge/2),y+blockHeight-5),false,block);
            port->setZValue(1001.0);
            scene->addItem(port);
            block->addPort(port);

        }

        file.seek(0);
        int blockIndex = 0;
        //pridani wires pripadne nastaveni hodnoty portu
        blocks=scene->getBlocks();
        while(!in.atEnd()){
            QString line = in.readLine();;
            QRegExp rx("[,;]");
            QStringList list = line.split(rx, QString::SkipEmptyParts);
            int num = 5+list[4].toInt();
            for (int i = 5; i != num; ++i){
                if(list[i][0]=='w'){
                    QString str = list[i];
                    str.remove(0,1);
                    Port *startItem = (blocks[blockIndex]->ports[i-5]);
                    Port *endItem   = (blocks[str.toInt()]->getOutPort());
                    Wire *wire = new Wire(startItem, endItem);
                    startItem->addWire(wire);
                    endItem->addWire(wire);
                    wire->setZValue(1000.0);
                    scene->addItem(wire);
                    wire->updatePosition();
                }
            }
            blockIndex++;
        }
        scene->setSceneChanged(false);
        file.close();
    }
}
