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

#include "circledline.h"

#include <QString>
#include <QDebug>

CircledLine::CircledLine(QGraphicsScene * s, float x, float y, float x2, float y2, QPen p) : Drawable(s)
{
    draw(x,y,x2,y2);
    typeNumber = 1;
    x0 = x;
    y0 = y;
    x1 = x2;
    y1 = y2;
    linePen = p;

}
void CircledLine::draw(float x, float y, float x2, float y2) {
    line =  scene->addLine(x, y, x2, y2, linePen);
    float dist = sqrt(line->line().dx() * line->line().dx() + line->line().dy() * line->line().dy());
    sizeNumber = sqrt(line->line().dx() * line->line().dx() + line->line().dy() * line->line().dy());
    ellipse = scene->addEllipse((x+x2)/2-(dist/2), (y+y2)/2-(dist/2), dist, dist, linePen);

}

void CircledLine::show() {
    line->show();
    ellipse->show();
}

void CircledLine::hide() {
    line->hide();
    ellipse->hide();
}

QString CircledLine::save(QLocale * locale, QString separator) {
    QString result;
    if(locale) {
        QTextStream(&result) << locale->toString(typeNumber) << separator << locale->toString(x0) << separator << locale->toString(y0) << separator << locale->toString(x1) << separator << locale->toString(y1);
    } else {
       // QTextStream(&result) << (typeNumber) << separator << (x0) << separator << (y0) << separator << (x1) << separator << (y1);
    }
    return result;
}
