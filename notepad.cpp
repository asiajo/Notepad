#include "notepad.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this -> setCentralWidget(ui -> textEdit);
    setWindowTitle(tr("%1[*] - %2").arg("New File", QCoreApplication::applicationName()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    maybe_save();
    currentFile.clear();
    ui -> textEdit -> setText (QString());
    filename = "";
    setWindowTitle(tr("%1[*] - %2").arg("New File", QCoreApplication::applicationName()));
}

void MainWindow::on_actionOpen_triggered()
{
    maybe_save();
    filename = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Ooops", "Cannot open the file: " + file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    ui -> textEdit -> setText(text);
    file.close();
    setWindowTitle(tr("%1[*] - %2").arg(filename, QCoreApplication::applicationName()));
}

void MainWindow::on_actionSave_as_triggered()
{
    filename = QFileDialog::getSaveFileName(this, "Save as");
    on_actionSave_triggered();
}

void MainWindow::on_actionSave_triggered()
{
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, "Ooops", "Cannot save the file: " + file.errorString());
            return;
        }
        QTextStream out(&file);
        QString text = ui -> textEdit -> toPlainText();
        out << text;
        file.close();
        setWindowTitle(tr("%1[*] - %2").arg(filename, QCoreApplication::applicationName()));
    }
    else
        on_actionSave_as_triggered();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;

    QPrintDialog pDialog(&printer, this);
    if(pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot access printer");
        return;
    }
    ui->textEdit->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
    maybe_save();
    QApplication::quit();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    on_actionExit_triggered();
}

void MainWindow::maybe_save()
{
    if(ui->textEdit -> document() -> isModified())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Save?", "You did not save your changes. Do you want to save them now?",
                                                                   QMessageBox::Save|QMessageBox::Discard);
        if (reply == QMessageBox::Save)
            on_actionSave_triggered();
    }
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "This is a program that I have made to learn QT. <br> Author: Joanna",
                                                                QMessageBox::Ok);
}
