#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chip8Core.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>
#include <string>
#include <iostream>

using namespace std;

chip8Core myChip8;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("CHIP-8 Emulator");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("Alterado");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "CHIP-8 Emulator\nCreated by: irbp\nVersion: 1.0.0");
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open ROM"), "/home", tr("CHIP-8 ROM (*.ch8)"));
    const char * dirName = fileName.toStdString().c_str();
    myChip8.loadRom(dirName);
}
