#include "paramview.h"
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QSize>
#include <QFontMetrics>
#include <algorithm>
#include <QDebug>


ParamView::ParamView(QWidget *parent) : QFrame(parent)
{
    param = nullptr;
    setFrameShadow(QFrame::Sunken);
    setFrameShape(QFrame::StyledPanel);

    buffer = new QQueue<float>();
    buffer_max = 0.0f;
}

ParamView::~ParamView()
{
    delete buffer;
}

void ParamView::viewParam(const QString &name, Parameter *p)
{
    param_name = name;
    param = p;
}

Parameter *ParamView::parameter()
{
    return param;
}

void ParamView::updated()
{
    buffer->append(param->toFloat());
    while(buffer->length() > width()) buffer->removeFirst();

    buffer_max = *std::max_element(buffer->begin(), buffer->end());

    update();
}

void ParamView::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);

    //painter.drawRect(QRect(0, 0, width()-1, height()-1));

    drawParameter(painter);
}

void ParamView::drawParameter(QPainter &painter)
{
    QFont name_font(font());
    scaleFont(name_font, height()/5, 8, 12);

    QFontMetrics fm(name_font);

    int delta = fm.height() / 2;

    QString value_str = param ? param->toString() : QString();

    painter.setFont(name_font);
    drawTextAt(painter, param_name, delta, delta, TEXT_POS_BOTTOM_RIGHT);

    painter.setFont(name_font);
    drawTextAt(painter, value_str, delta, height() - delta, TEXT_POS_TOP_RIGHT);

    if(!buffer->isEmpty()){
        float top_value = getNearestMax(buffer_max);

        auto calc_y = [top_value, this](float value) -> int{
            return static_cast<int>((1.0f - value / top_value) * height());
        };

        int x_old = width() - buffer->size();
        int y_old = calc_y(buffer->first());

        int x = 0, y = 0;

        auto iter = buffer->begin();
        ++ iter;

        for(; iter != buffer->end(); ++ iter){
            x = x_old + 1;
            y = calc_y(*iter);
            painter.drawLine(x_old, y_old, x, y);
            x_old = x;
            y_old = y;
        }
    }
}

void ParamView::drawTextAt(QPainter &painter, const QString &text, int x, int y, ParamView::TextPosition pos)
{
    QFontMetrics fm(painter.font());
    QSize bound = fm.size(0, text);

    //qDebug() << fm.leading() << fm.ascent() << fm.descent() << fm.lineSpacing() << fm.lineWidth() << fm.strikeOutPos() << fm.overlinePos() << fm.underlinePos();

    int dx = fm.leading() - fm.minLeftBearing();
    int dy = fm.lineWidth() + fm.descent();

    switch(pos){
    case TEXT_POS_TOP_LEFT:
        painter.drawText(x - bound.width() - dx, y - dy, text);
        break;
    case TEXT_POS_TOP_RIGHT:
        painter.drawText(x + dx, y - dy, text);
        break;
    case TEXT_POS_BOTTOM_LEFT:
        painter.drawText(x - bound.width() - dx, y + bound.height() - dy, text);
        break;
    case TEXT_POS_BOTTOM_RIGHT:
        painter.drawText(x + dx, y + bound.height() - dy, text);
        break;
    case TEXT_POS_TOP_CENTER:
        painter.drawText(x + (dx - bound.width()) / 2, y - dy, text);
        break;
    case TEXT_POS_BOTTOM_CENTER:
        painter.drawText(x + (dx - bound.width()) / 2, y + bound.height() - dy, text);
        break;
    case TEXT_POS_LEFT_CENTER:
        painter.drawText(x - bound.width() - dx, y + (bound.height() - dy) / 2, text);
        break;
    case TEXT_POS_RIGHT_CENTER:
        painter.drawText(x + dx, y + (bound.height() - dy) / 2, text);
        break;
    }
}

void ParamView::scaleFont(QFont& scale_font, int needed_height, int min_size, int max_size)
{
    int dpi = logicalDpiY();

    if(dpi == 0) return;

    float font_size = static_cast<float>(needed_height) * 72 / dpi;

    font_size = std::max<float>(std::min<float>(font_size, max_size), min_size);

    //qDebug() << font_size;

    scale_font.setPointSizeF(font_size);
}

float ParamView::getNearestMax(float value)
{
    if(value == 0.0f) return 1;
    //const int base = 10;
    //return static_cast<int>(ceil(1.618f / base * value)) * base;
    //return static_cast<int>(pow(10.0f, floor(log10(value)) + 1.0f));

    const float min_upper_bound = 10.0f;

    float cur_base = pow(10.0f, floor(log10(value)));
    float n = ceil(value / cur_base);
    float res = cur_base * n;

    if(value >= res) res += cur_base;

    if(res < min_upper_bound) res = min_upper_bound;

    return res;
}

