 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTextStream>
#include <QtCore>
#include "block.h"

class BlockEditorScene;

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
    void blockButtonClicked(int button);
    void pointerGroupClicked(int id);

    void about();
    void open();
    void save();

private:
    enum {addButton, subButton, mulButton, divButton, stepButton, runButton, moveButton, wireButton};

    Ui::MainWindow *ui;

    QButtonGroup *blocksButtonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *actionTypeGroup;
    QToolBox *toolBox;
    QToolBar *toolBar;
    QSpinBox *inputsSpinBox;

    BlockEditorScene *scene;

    QGraphicsView *view;

    void createToolBox();
    void createToolBar();
    QAbstractButton *createBlockButton(const QString &text, int buttonType);

    void createMenus();
    void createActions();

    QAction *aboutAction;
    QAction *saveAction;
    QAction *openAction;

    QMenu *fileMenu;
    QMenu *aboutMenu;

};

#endif // MAINWINDOW_H
