#ifndef PARAMVIEW_H
#define PARAMVIEW_H

#include <QWidget>
#include <QString>
#include <QQueue>
#include <QColor>
#include "parameter.h"

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QPixmap;


class ParamView : public QWidget
{
    Q_OBJECT
public:
    explicit ParamView(QWidget *parent = 0);
    ~ParamView();

    void viewParam(const QString& name, Parameter* p);
    Parameter* parameter();

    void setColor(const QColor& c);

signals:

public slots:
    void updated();

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QString param_name;
    Parameter* param;

    QPixmap* main_pixmap;

    QQueue<float>* buffer;
    float buffer_max;
    float buffer_min;

    QColor param_color;

    void updatePixmap();

    void scaleFont(QFont &scale_font, int needed_height, int min_size, int max_size);
    float getNearestMax(float value);
    float getNearestMin(float value);
};

#endif // PARAMVIEW_H
