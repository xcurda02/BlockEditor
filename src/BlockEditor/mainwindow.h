 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTextStream>
#include <QtCore>
#include "block.h"
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void blockInserted(Block *block);
    void blockButtonClicked(int button_id);
    void toolbarButtonGroupClicked(int button_id);


private:
    enum {addButton, subButton, mulButton, divButton, stepButton, runButton, moveButton, wireButton, invAButton, invBButton};

    Ui::MainWindow *ui;

    QButtonGroup *blocksButtonGroup;
    QButtonGroup *toolbarButtonGroup;


    void showMsg(QString msg);

    QToolBox *toolBox;
    QToolBar *toolBar;
    QSpinBox *inputsSpinBox;

    Calculator *calc;

    QMenu *fileMenu;
    QMenu *aboutMenu;

    BlockEditorScene *scene;

    QGraphicsView *view;

    void createToolBox();
    void createToolbar();
    QAbstractButton *createBlockButton(int buttonType);
    QToolButton *createToolbarButton(int buttonType);

};

#endif // MAINWINDOW_H
