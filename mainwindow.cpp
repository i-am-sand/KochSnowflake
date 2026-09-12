#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "snowflakewidget.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent), ui (new Ui::MainWindow)
{
  ui->setupUi (this);
  SnowflakeWidget *snowflake = new SnowflakeWidget(this);
  setCentralWidget(snowflake);
  setWindowTitle("Снежинка Коха");
  resize(800, 700);
}

MainWindow::~MainWindow () { delete ui; }
