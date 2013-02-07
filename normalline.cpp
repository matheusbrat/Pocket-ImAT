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

#include "normalline.h"
#include <QDebug>

NormalLine::NormalLine(QGraphicsScene * s, float x, float y, float x2, float y2, QPen p) : Drawable(s)
{
    linePen = p;
    draw(x,y,x2,y2);
    typeNumber = 0;
    x0 = x;
    y0 = y;
    x1 = x2;
    y1 = y2;
}

void NormalLine::draw(float x, float y, float x2, float y2) {
    line =  scene->addLine(x, y, x2, y2, linePen);
    sizeNumber = sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
    qDebug() << sizeNumber;
}

void NormalLine::show() {
    line->show();
}

void NormalLine::hide() {
    line->hide();
}

QString NormalLine::save(QLocale * locale, QString separator) {
    QString result;
    if(locale) {
        QTextStream(&result) << locale->toString(typeNumber) << separator << locale->toString(x0) << separator << locale->toString(y0) << separator << locale->toString(x1) << separator << locale->toString(y1);
    } else {
        //QTextStream(&result) << (typeNumber) << "," << (x0) << "," << (y0) << "," << (x1) << "," << (y1);
    }
    return result;
}
