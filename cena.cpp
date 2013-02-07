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

#include "cena.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPen>
#include <QGraphicsLineItem>
#include <QKeyEvent>
#include <QPainter>
#include "circledline.h"
#include "normalline.h"
#include "graphicspixmapitem.h"
#include "scaleline.h"
#include <QFileDialog>


Cena::Cena(QObject *parent) :
    QGraphicsScene(parent)
{
    this->x1 = 0;
    this->y1 = 0;
    useCircledLine = false;
    tempLine = false;
    scaleLine = false;
    settingScale = false;
    isUpdated = false;
    scaleSize = 0;
    undo = QStack<Drawable *>();
    redo = QStack<Drawable *>();
}

void Cena::setScale(bool) {
    this->x1 = 0;
    this->y1 = 0;

    settingScale = true;
    if(scaleLine) {
        scaleLine->hide();
    }
}
float Cena::getScalePixelSize() {
    if(scaleLine)
        return scaleLine->size();
    return 0;
}
float Cena::getScaleSize() {
    return scaleSize;
}
void Cena::setScaleSize(float f) {
    isUpdated = true;
    scaleSize = f;
}

QStack<Drawable *> * Cena::getLines() {
    return &undo;
}
void Cena::setUseCircledLine(bool useCircledLine2) {
    useCircledLine = useCircledLine2;
}
void Cena::addObj(int t, float x, float y, float x1, float y1, QPen p) {
    qDebug() << "ADDOBJ";
    if(t == LINE) {
        NormalLine * c = new NormalLine(this, x, y, x1, y1, p);
        undo.append(c);
    } else if(t == CIRCLED) {
        CircledLine * c = new CircledLine(this, x, y, x1, y1, p);
        undo.append(c);
    } else if(t == SCALE) {
        scaleLine = new ScaleLine(this, x, y, x1, y1, p);
    }
}

void Cena::mousePressionado(qreal x, qreal y) {
    if(x1 == 0 && y1 == 0) {
        this->x1 = x;
        this->y1 = y;
    } else {
        if(tempLine)
            tempLine->hide();
        if(!settingScale) {
            QPen linePen = QPen(Qt::red);
            linePen.setWidth(1);
            if(useCircledLine) {
                addObj(CIRCLED, this->x1 + 0.5, this->y1, x - 0.5, y, linePen);
            } else {
                addObj(LINE, this->x1 + 0.5, this->y1, x - 0.5, y, linePen);
            }
        } else {
            QPen linePen = QPen(Qt::blue);
            linePen.setWidth(4);
            addObj(SCALE, this->x1 + 2, this->y1, x - 2, this->y1, linePen);
            settingScale = false;
        }
        isUpdated = true;
        this->x1 = 0;
        this->y1 = 0;
    }

}

void Cena::keyPressEvent(QKeyEvent * event) {
    if(event->modifiers() == Qt::CTRL && event->key() == 90 ) {
        if(!undo.isEmpty()) {
            Drawable * i = undo.pop();
            i->hide();
            redo.append(i);
            isUpdated = true;
        }
    } else if (event->modifiers() == Qt::CTRL && event->key() == 89)  {
        if(!redo.isEmpty()) {
            Drawable * i = redo.pop();
            i->show();
            undo.append(i);
            isUpdated = true;
        }
    }

}

void Cena::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        this->x1 = 0;
        this->y1 = 0;
        if(tempLine)
            tempLine->hide();
    } else {
        mousePressionado(event->scenePos().x(), event->scenePos().y());
    }
    QGraphicsScene::mousePressEvent(event);

}
void Cena::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if(this->x1 != 0 && this->y1 != 0) {
        if (tempLine) {
            tempLine->hide();
        }
        if(!settingScale) {
            QPen linePen = QPen(Qt::red);
            linePen.setWidth(1);
            tempLine = addLine(this->x1 + 0.5, this->y1, event->scenePos().x() - 0.5, event->scenePos().y(), linePen);
        } else {
            QPen linePen = QPen(Qt::blue);
            linePen.setWidth(4);
            tempLine = addLine(this->x1+ 2, this->y1, event->scenePos().x()- 2, this->y1, linePen);
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void Cena::loadBackground(QString bg) {
    clear();
    QPixmap pixMap = QPixmap(bg);
    GraphicsPixmapItem * i = new GraphicsPixmapItem(pixMap);
    i->setFlag(QGraphicsItem::ItemIsSelectable,true);
    addItem(i);
}

bool Cena::getUpdated() {
    bool r  = isUpdated;
    return r;
}

void Cena::resetUpdated() {
    isUpdated = false;
}

void Cena::clear() {
    this->x1 = 0;
    this->y1 = 0;
    useCircledLine = false;
    tempLine = false;
    scaleLine = false;
    settingScale = false;
    undo.clear();
    redo.clear();
    isUpdated = false;
    QGraphicsScene::clear();
}

Drawable * Cena::getScaleLine() {
    return scaleLine;
}
void Cena::setScaleLine(Drawable * s) {
    scaleLine = s;
}
