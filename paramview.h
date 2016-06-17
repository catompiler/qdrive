#ifndef PARAMVIEW_H
#define PARAMVIEW_H

#include <QFrame>
#include <QString>
#include <QQueue>
#include "parameter.h"

class QPaintEvent;
class QMouseEvent;


class ParamView : public QFrame
{
    Q_OBJECT
public:
    explicit ParamView(QWidget *parent = 0);
    ~ParamView();

    void viewParam(const QString& name, Parameter* p);
    Parameter* parameter();

signals:

public slots:
    void updated();

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QString param_name;
    Parameter* param;

    QQueue<float>* buffer;
    float buffer_max;

    enum TextPosition {
        TEXT_POS_TOP_LEFT = 0,
        TEXT_POS_TOP_RIGHT,
        TEXT_POS_BOTTOM_LEFT,
        TEXT_POS_BOTTOM_RIGHT,
        TEXT_POS_TOP_CENTER,
        TEXT_POS_BOTTOM_CENTER,
        TEXT_POS_LEFT_CENTER,
        TEXT_POS_RIGHT_CENTER
    };

    void drawParameter(QPainter& painter);

    void drawTextAt(QPainter& painter, const QString& text, int x, int y, TextPosition pos);
    void scaleFont(QFont &scale_font, int needed_height, int min_size, int max_size);
    float getNearestMax(float value);
};

#endif // PARAMVIEW_H
