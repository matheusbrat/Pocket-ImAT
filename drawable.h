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

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <math.h>
#include <QGraphicsScene>
#include <QLocale>

#define LINE 0
#define CIRCLED 1
#define SCALE 2

class Drawable
{
public:
    Drawable(QGraphicsScene *);
protected:
    QGraphicsScene * scene;
    QPen linePen;
    double sizeNumber;
    int typeNumber;

public:
    void virtual hide();
    void virtual show();
    double virtual size();
    int type();
    QString virtual save(QLocale *locale, QString separator);
    void setColor(QColor);

};

#endif // DRAWABLE_H
