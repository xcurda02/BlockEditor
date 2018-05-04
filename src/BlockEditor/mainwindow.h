////// soubor: mainwindow.h
////// autori: Vojtech Curda (xcurda02), Miroslav Bulicka (xbulic02)
////// Definice Tridy MainWindow
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsPixmapItem>
#include <QMainWindow>
#include <QtWidgets>
#include <QTextStream>
#include <QtCore>
#include <QCloseEvent>
#include "block.h"
#include "port.h"
#include "wire.h"
#include "calculator.h"


class BlockEditorScene;
class Calculator;

QT_BEGIN_NAMESPACE
class QGraphicsView;
class QButtonGroup;
class QAbstractButton;
class QToolBox;
QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class Trida MainWindow se stara o veskerou praci v okne mimo grafickou scenu
 * Vytvari widgety a stara se o ukladani schematu
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void blockButtonClicked(int button_id);
    void toolbarButtonGroupClicked(int button_id);


    void about();
    void open();
    void save();

private:

    enum ButtonType {addButton, subButton, mulButton, divButton, stepButton, runButton, moveButton, wireButton, invAButton, invBButton};

    void showMsg(QString msg);
    void createToolBox();
    void createToolbar();
    QAbstractButton *createBlockButton(ButtonType buttonType);
    QToolButton *createToolbarButton(ButtonType buttonType);
    void createMenus();
    void createActions();

    Ui::MainWindow *ui;                 ///> Rozhrani

    QButtonGroup *blocksButtonGroup;    ///> Tlacitka bloku
    QButtonGroup *toolbarButtonGroup;   ///> Tlacitka toolbaru (Move, Wire, Step, Run)

    QToolBox *toolBox;                  ///> ToolBox - misto, kde jsou umisteny tlacitka bloku a nastaveni poctu vstupu
    QToolBar *toolBar;                  ///> ToolBar - oddeleni s tlacitky Move, Wire, Step, Run

    QSpinBox *inputsSpinBox;            ///> SpinBox pro nastaveni poctu vstupu

    Calculator *calc;                   ///> Objekt pro vypocitavani schematu

    QMenu *fileMenu;                    ///> Menu File
    QMenu *aboutMenu;                   ///> Menu About

    BlockEditorScene *scene;            ///> Graficka scena

    QGraphicsView *view;                ///> View

    QAction *aboutAction;               ///> Akce v menu - About
    QAction *saveAction;                ///> Akce v menu - Save
    QAction *openAction;                ///> Akce v menu - Open
protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
