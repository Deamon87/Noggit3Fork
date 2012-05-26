#ifndef TEXTURESELECTER_H
#define TEXTURESELECTER_H

#include "noggit/blp_texture.h"

#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QPainter>

#define WIDTH 150
#define HEIGHT 150

class QResizeEvent;

namespace noggit
{

class textureSelecter : public QWidget
{
    Q_OBJECT
public:
    textureSelecter(QGLWidget *shared, QWidget *parent = 0);

signals:

public slots:

private:
    QGLWidget *sharedWidget;

};

class textureView : public QGraphicsView
{
    Q_OBJECT
public:
    textureView(QStringList textures, QGLWidget *shared, QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event);
    QSize sizeHint() const;
signals:

public slots:

private:
    int num;

};

class textureScene : public QGraphicsScene
{
    Q_OBJECT
public:
    textureScene(QStringList textures, QRectF rect , QWidget *parent= 0);
    void drawBackground(QPainter *painter, const QRectF &);

signals:

public slots:
    void resized(QRectF);
private:
    QStringList textureList;
    int rows, columns;

};

class textureItem : public QGraphicsItem
{

public:
    textureItem(QString texturename);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/);
    QRectF boundingRect() const;

signals:

public slots:

private:
    noggit::blp_texture *blptexture;

};

}

#endif // TEXTURESELECTER_H