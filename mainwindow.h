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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLocale>
#include <QSettings>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionUse_line_triggered(bool checked);
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionLoad_Image_triggered();
    void on_actionSave_as_triggered();
    void on_actionSet_Scale_triggered();
    void on_actionEnglish_US_triggered();
    void on_actionPortuguese_BR_triggered();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    bool isSaved;
    QString savedFileName;
    QSettings settings;
    void save();
    void saveAs();
    void save(QString);
    QString imageFileName;
    void resetTitle();
    QLabel * scaleStatusBar;
    QLocale locale;
    QString csvSeparator;
    static const QString CSV_WC;
    static const QString CSV_WS;
    QString loadImageDefaultDir;
    QString saveDefaultDir;
    QString lastOpenedType;
    void saveDefaulDir(QString);
    void saveLoadImage(QString);
    QString removeFileName(QString);


};

#endif // MAINWINDOW_H
