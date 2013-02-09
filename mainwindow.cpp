/* Copyright (c) 2013 - Matheus Bratfisch

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cena.h"
#include "myview.h"
#include <QFileDialog>
#include <QDebug>
#include "graphicspixmapitem.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>
#include <QLabel>
#include <QLocale>
#include <QSettings>
#include "about.h"

const QString MainWindow::CSV_WC = "Comma-Separated Values with \",\" divisor (*.csv)";
const QString MainWindow::CSV_WS = "Comma-Separated Values with \";\" divisor (*.csv)";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), settings(QSettings::IniFormat, QSettings::UserScope, "MatBra", "PocketImAT"),
    ui(new Ui::MainWindow)
{

    Cena * scene = new Cena();
    scene->setSceneRect(0,0,1280,1024);

    savedFileName = QString(" ");
    isSaved = true;

    ui->setupUi(this);


    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,1280,1024);

    QStatusBar * bar = this->statusBar();
    scaleStatusBar = new QLabel(bar);
    QString s = "Scale: ";
    csvSeparator = ",";
    s = s.leftJustified(20, ' ');
    scaleStatusBar->setText(s);
    bar->addPermanentWidget(scaleStatusBar);

    locale = QLocale(settings.value("Language", QLocale(QLocale::English, QLocale::UnitedStates)).toLocale());
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    if(locale.country() == QLocale::UnitedStates) {
        ui->actionEnglish_US->setChecked(true);
        ui->actionPortuguese_BR->setChecked(false);
        lastOpenedType = CSV_WC;
    } else {
        ui->actionEnglish_US->setChecked(false);
        ui->actionPortuguese_BR->setChecked(true);
        lastOpenedType = CSV_WS;
    }

    scene->setUseCircledLine(settings.value("useCircledLine", false).toBool());
    ui->actionUse_line->setChecked(settings.value("useCircledLine", false).toBool());

    loadImageDefaultDir = settings.value("loadImageDefaultDir", "").toString();
    saveDefaultDir  = settings.value("saveDefaultDir", "").toString();

    // ui->menuBar->addAction("About",this)
    QAction * actionAbout = new QAction(this);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(on_actionAbout_triggered()));

    ui->menuBar->addAction(actionAbout);

}

void MainWindow::on_actionAbout_triggered() {
    qDebug() << "OIEM";
    //QMessageBox::about(this, "Pocket ImAT - Pocket Image Analysis Tools", "Program created to measure images. \r\n Created by Matheus Bratfisch \r\n http://www.matbra.com");
    About * t = new About();

}

void MainWindow::saveDefaulDir(QString s) {
    QString a = removeFileName(s);
    saveDefaultDir = a;
    settings.setValue("saveDefaultDir", a);
}

void MainWindow::saveLoadImage(QString s) {
    QString a = removeFileName(s);
    loadImageDefaultDir = a;
    settings.setValue("loadImageDefaultDir", a);
}

QString MainWindow::removeFileName(QString s) {
    s = s.left(s.lastIndexOf("/"));
    return s;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionUse_line_triggered(bool checked)
{
    Cena * s = (Cena*) ui->graphicsView->scene();
    s->setUseCircledLine(checked);
    settings.setValue("useCircledLine", checked);
 }

void MainWindow::save() {
    QString filename;
    qDebug() << savedFileName;

    if(savedFileName.size() == 1) {
        filename = QFileDialog::getSaveFileName(0, "Save File", saveDefaultDir, "*.csv");
        if(filename.size() > 0) {
            if(!filename.endsWith(".csv"))
                filename.append(".csv");
            savedFileName = filename;
        }
    } else {
        filename = savedFileName;
    }

    save(filename);
    resetTitle();
}

void MainWindow::saveAs() {
    QString filename;
    filename = QFileDialog::getSaveFileName(0, "Save File", saveDefaultDir, "*.csv");
    if(filename.size() > 0) {
        if(!filename.endsWith(".csv"))
            filename.append(".csv");
        savedFileName = filename;
        save(filename);
        resetTitle();
    }
}

void MainWindow::save(QString filename) {   
    QFile f( filename );
    f.open( QIODevice::WriteOnly | QIODevice::Text);
    saveDefaulDir(filename);
    QTextStream out(&f);
    Cena * c  = (Cena*) ui->graphicsView->scene();
    MyView * view = (MyView*) ui->graphicsView;
    QStack<Drawable *> * lines = c->getLines();
    Drawable * scale = c->getScaleLine();
    out << "Size" << csvSeparator << "Type" << csvSeparator << "P0X" << csvSeparator << "P0Y" << csvSeparator << "P1X" << csvSeparator << "P1Y" << csvSeparator << "Scale Pixel Size" << endl;
    if(scale)
        out << scale->size() << csvSeparator << scale->save(&locale, csvSeparator) << csvSeparator << c->getScaleSize() << endl;
    else
        out << "0" << csvSeparator << "0" << csvSeparator << "0" << csvSeparator << "0"  << csvSeparator << "0" << csvSeparator << "0" << csvSeparator << c->getScaleSize() << endl;
    for(int x = 0; x < lines->size(); x++) {
        Drawable * d  = (Drawable*) lines->at(x);
        qDebug() << "D:" << d->size() << " ScaleSize " << c->getScaleSize() << " ScalePixelSize " << c->getScalePixelSize();
        double v = d->size() * c->getScaleSize() / c->getScalePixelSize();
        out << locale.toString(v);
        out << csvSeparator;
        out << d->save(&locale, csvSeparator);
        out << endl;
    }
    f.close();
    c->resetUpdated();
}

void MainWindow::on_actionSave_triggered()
{
    save();
}


void MainWindow::on_actionOpen_triggered()
{

    Cena * s = (Cena*) ui->graphicsView->scene();
    QPen linePen = QPen(Qt::blue);
    linePen.setWidth(4);
    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(0, "Open File", saveDefaultDir, lastOpenedType + ";;" + CSV_WC + ";;" + CSV_WS, &selectedFilter);
    if(selectedFilter == CSV_WC) {
        ui->actionEnglish_US->setChecked(true);
        ui->actionPortuguese_BR->setChecked(false);
        locale = QLocale(QLocale::English, QLocale::UnitedStates);
        locale.setNumberOptions(QLocale::OmitGroupSeparator);
        csvSeparator = ",";
    } else {
        ui->actionEnglish_US->setChecked(false);
        ui->actionPortuguese_BR->setChecked(true);
        locale = QLocale(QLocale::Portuguese, QLocale::Brazil);
        locale.setNumberOptions(QLocale::OmitGroupSeparator);
        csvSeparator = ";";
    }
    if(filename.size() > 0) {
        QFile f( filename );
        saveDefaulDir(filename);
        f.open( QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&f);
        QString line = in.readLine();

        while(!in.atEnd()) {
            line = in.readLine();
            QStringList fields = line.split(csvSeparator);
            if(fields.size() > 1) {
                float x = locale.toFloat(fields.at(2));
                float y = locale.toFloat(fields.at(3));
                float x1= locale.toFloat(fields.at(4));
                float y1= locale.toFloat(fields.at(5));

                if(!(x == 0 && y == 0 && x1 == 0 && y1 == 0)) {
                    s->addObj(locale.toInt(fields.at(1)), x, y, x1, y1, linePen);
                    if(fields.at(1).toInt() == 2) {
                        s->setScaleSize(locale.toFloat(fields.at(6)));
                    }
                }
                linePen.setColor(Qt::red);
                linePen.setWidth(1);
            } else {
                QMessageBox msgBox;
                msgBox.setText("It seems to be a wrong formated file. Verify if it's the right data file or if you're using the appropriate locale (Options > Locale).");
                msgBox.setInformativeText("Wrong file");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Warning);
                int ret = msgBox.exec();
                return;
            }
        }
        savedFileName = filename;
        resetTitle();
        s->resetUpdated();

    }


}

void MainWindow::resetTitle() {
    QString s = "Pocket ImAT - ";
    QString tempFileName = imageFileName;
    tempFileName = tempFileName.remove(0, tempFileName.lastIndexOf("/")+1);
    this->setWindowTitle(s.append(tempFileName).append(" @ ").append(savedFileName));
}

void MainWindow::on_actionLoad_Image_triggered()
{
    Cena * s = (Cena*) ui->graphicsView->scene();
    QString file;
    if(s->getUpdated()) {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
          case QMessageBox::Save:
              save();
              s->resetUpdated();
              file = QFileDialog::getOpenFileName(0,"Choose an image",loadImageDefaultDir);
              saveLoadImage(file);
              s->loadBackground(file);
              savedFileName = QString(" ");
              break;
          case QMessageBox::Discard:
              s->resetUpdated();
              file = QFileDialog::getOpenFileName(0,"Choose an image",loadImageDefaultDir);
              saveLoadImage(file);
              s->loadBackground(file);
              savedFileName = QString(" ");
              break;

          case QMessageBox::Cancel:
              // Cancel was clicked
              break;
          default:
              // should never be reached
              break;
        }
    } else {
        file = QFileDialog::getOpenFileName(0,"Choose an image",loadImageDefaultDir);
        saveLoadImage(file);
        s->loadBackground(file);
        savedFileName = QString(" ");
        isSaved = false;
    }
    imageFileName = file;
    resetTitle();
}

void MainWindow::on_actionSave_as_triggered()
{
    saveAs();
}

void MainWindow::on_actionSet_Scale_triggered()
{
    QString text = "";
    bool ok = true;
    bool f = false;
    while((text.compare("") == 0 && ok) || (ok && !f)) {
        text = QInputDialog::getText(this, "Enter Scale", "Scale:", QLineEdit::Normal,"", &ok);
        if(ok && (text.compare("") != 0)) {
            Cena * c =  (Cena*) ui->graphicsView->scene();
            c->setScale(true);
            float sc = text.toFloat(&f);
            if(f) {
                c->setScaleSize(text.toFloat());
                QString s = "Scale: " + text;
                s = s.leftJustified(20, ' ');
                scaleStatusBar->setText(s);

            }
       }
    }
}

void MainWindow::on_actionEnglish_US_triggered()
{
    ui->actionPortuguese_BR->setChecked(false);
    locale = QLocale(QLocale::English, QLocale::UnitedStates);
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    settings.setValue("Language", locale);
    lastOpenedType = CSV_WC;
    csvSeparator = ",";
}

void MainWindow::on_actionPortuguese_BR_triggered()
{
    ui->actionEnglish_US->setChecked(false);
    locale = QLocale(QLocale::Portuguese, QLocale::Brazil);
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    settings.setValue("Language", locale);
    lastOpenedType = CSV_WS;
    csvSeparator = ";";
}
