#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QPointF>
#include <cstdio>
#include <QTimer>

extern "C"{
#include "../P4_library/P4Game.h"
}

#define SQUARE_SIZE      100
#define PIECE_RADIUS    40


const int EmptyBoard[BOARD_RAW][BOARD_COLUMN] = {
    { 0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0},
    { 0,0,0,0,0,0,0}
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this->ui->pushButton, &QPushButton::released, this, &MainWindow::handleStartButton);
    this->pGame = NULL;

    this->status = 0;

}

MainWindow::~MainWindow()
{
    delete ui;
}

QColor MainWindow::pieceColor(int pieceType){
    if(pieceType == 1) return  QColorConstants::Svg::red;
    if(pieceType == 2) return  QColorConstants::Svg::yellow;

    //default
    return  QColorConstants::Svg::grey;
}





void MainWindow::handleStartButton(){
    int firstTurn;

    switch (status) {
    case 0:
        if(pGame != NULL) free(pGame);

        p4PlayerType_t p1Type, p2Type;

        if(this->ui->radioHuman->isChecked())
            p1Type = p4pt_human;
        else if(this->ui->radioSimple->isChecked())
            p1Type = p4pt_machine;
        else {
            p1Type = p4pt_online;
        }

        if(this->ui->radioHuman_2->isChecked())
            p2Type = p4pt_human;
        else
            p2Type = p4pt_machine;

        pGame = NewP4Game(p1Type, p2Type);
        //tree = NewTree(6,mcEv_MCTS);


        this->ui->pushButton->setText("Playing ...");
        this->ui->pushButton->setEnabled(false);
        status = 1;
        ActivePlayerEvent(firstTurn);
        break;
    case 2:

        //reset the game
        //deleteNode(tree->root);
        //free(tree);
        //free(pGame);
        //tree = NewTree(6,mcEv_MCTS);
        //pGame = NewP4Game(p4pt_human, p4pt_machine);
        //this->ui->pushButton->setText("Start");
        //this->update();
        break;
    }


    this->update();

}



void MainWindow::endGame(){
    this->ui->pushButton->setEnabled(true);
    this->ui->pushButton->setText("Restart Game");
    this->status = 0;
    this->update();
}

void MainWindow::algoIterate(){

    int AI_Turn = 0;
    //AI_Turn = nextComputerTurn(pGame, getCurrentlayer(pGame));

    p4TurnResult_e playerResult = p4Game_nextTurn(pGame , AI_Turn);

    ProcessResult(playerResult, AI_Turn);

    this->update();


}


void MainWindow::ProcessResult(p4TurnResult_e playerResult, int prevTurn){

    switch(playerResult){
    case p4tr_player1Turn:
    case p4tr_player2Turn:
        //on continue
        ActivePlayerEvent(prevTurn);

        break;
     case p4tr_player1Win:

        this->ui->plainTextEdit->appendPlainText("Player 1 win !!");
        endGame();
        break;
    case p4tr_player2Win:

       this->ui->plainTextEdit->appendPlainText("Player 2 win !!");
       endGame();
       break;
    case p4tr_draw:
        this->ui->plainTextEdit->appendPlainText("Match Nul !!");
        endGame();
        break;



    }




}

void MainWindow::ActivePlayerEvent(int prevTurn){

    switch (p4Game_getCurrentPlayerType(pGame)) {
        case p4pt_human :
            //active le mouseEvent

            break;
    case p4pt_machine :
        //active the oneShot Timer
        QTimer::singleShot(0, this, SLOT(algoIterate()));
        break;

    }


}


void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton)    // Left button...
     {
       // Do something related to the left button
        if(pGame == NULL) return;
        if(status == 0) return;

        if(p4Game_getCurrentPlayerType(pGame) == p4pt_human){

            int column =(int)( event->position().x() / SQUARE_SIZE);

            p4TurnResult_e playerResult = p4Game_nextTurn(pGame , column);

            ProcessResult(playerResult, column);

            this->update();
        }
     }

}

void MainWindow::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);

    QRectF rectangle(0 , 0, BOARD_COLUMN*SQUARE_SIZE, BOARD_RAW * SQUARE_SIZE);

    QPainter painter(this);
    //painter.drawRect(rectangle);
    painter.fillRect(rectangle, QColorConstants::Svg::blue);

     int const (*board)[BOARD_RAW][BOARD_COLUMN];

    if(this->pGame == NULL){
        board = &EmptyBoard;
    }
    else{
        board = &pGame->board;
    }
    if(board != NULL){

        for(int r=0;r<BOARD_RAW;r++){
            for(int c=0;c<BOARD_COLUMN;c++){

                int offset_pos = (SQUARE_SIZE - PIECE_RADIUS*2)/2;

                int x_pos = c *  SQUARE_SIZE + offset_pos;
                int y_pos = (BOARD_RAW - 1 - r) * SQUARE_SIZE + offset_pos;

                painter.setBrush(pieceColor((*board)[r][c]));
                painter.drawEllipse(QRectF(x_pos, y_pos, PIECE_RADIUS*2, PIECE_RADIUS*2));




            }


        }



    }



}
