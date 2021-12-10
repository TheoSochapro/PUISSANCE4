#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>

extern "C"{
#include "../P4_library/P4Game.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    p4Game_t *pGame;

    int status;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    void ActivePlayerEvent(int prevTurn);
    void ProcessResult(p4TurnResult_e playerResult, int prevTurn);

private slots:
    void handleStartButton();
    void algoIterate();

private:
    Ui::MainWindow *ui;

    QColor pieceColor(int pieceType);

    void endGame();

};
#endif // MAINWINDOW_H
