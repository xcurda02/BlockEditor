 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    enum {addButton, subButton, mulButton, divButton, stepButton, runButton, moveButton, wireButton};

    Ui::MainWindow *ui;

    QButtonGroup *blocksButtonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *actionTypeGroup;
    QToolBox *toolBox;
    QToolBar *toolBar;

    QMenu *fileMenu;
    QMenu *aboutMenu;

    BlockEditorScene *scene;

    QGraphicsView *view;

    void createToolBox();
    void createToolBar();
    QAbstractButton *createBlockButton(const QString &text, int buttonType);

};

#endif // MAINWINDOW_H
