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

#ifndef NORMALLINE_H
#define NORMALLINE_H

#include "drawable.h"
#include <QGraphicsLineItem>


class NormalLine : public Drawable
{
public:
    NormalLine(QGraphicsScene *, float, float, float, float, QPen);
    void hide();
    void show();
    QString save(QLocale *locale, QString separator);


private:
    void draw(float, float, float, float);
    QGraphicsLineItem * line;
    float x0, y0, x1, y1;
};

#endif // NORMALLINE_H
