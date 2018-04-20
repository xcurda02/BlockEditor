#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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


private:
    enum Buttons {addButton, subButton, divButton, mulButton, stepButton, runButton, moveButton, wireButton};

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
    //void blockButtonClicked(int button);
    QAbstractButton *createBlockButton(const QString &text, Buttons buttonType);

};

#endif // MAINWINDOW_H
