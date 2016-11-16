#include "paramview.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QToolTip>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QBrush>
#include <QRect>
#include <QSize>
#include <algorithm>
#include <utility>
#include "utils.h"
#include <QDebug>


using namespace utils::paint;


ParamView::ParamView(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    param_color = palette().mid().color();

    main_pixmap = new QPixmap();

    param = nullptr;

    buffer = new QQueue<float>();
    buffer_min = 0.0f;
    buffer_max = 0.0f;
}

ParamView::~ParamView()
{
    delete main_pixmap;
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

void ParamView::setColor(const QColor& c)
{
    param_color = c;

    updatePixmap();
    update();
}

void ParamView::updated()
{
    buffer->append(param->toFloat());
    while(buffer->length() > width()) buffer->removeFirst();

    auto buf_minmax = std::minmax_element(buffer->begin(), buffer->end());

    buffer_min = *buf_minmax.first;
    buffer_max = *buf_minmax.second;

    updatePixmap();
    update();
}

void ParamView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *main_pixmap);
}

void ParamView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    delete main_pixmap;
    main_pixmap = new QPixmap(event->size());

    updatePixmap();
}

void ParamView::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    int index = event->x();

    if(buffer->size() < width()) index -= width() - buffer->size();

    if(index < buffer->size() && index > 0){
        QToolTip::showText(event->globalPos(),Parameter::number(param->type(), buffer->at(index)), this);
    }else{
        QToolTip::hideText();
    }
}

void ParamView::updatePixmap()
{
    if(main_pixmap->width() == 0 || main_pixmap->height() == 0) return;

    QPainter painter(main_pixmap);

    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.fillRect(main_pixmap->rect(), palette().dark());

    float top_value = getNearestMax(buffer_max);
    float bottom_value = (buffer_min >= 0.0f) ? 0.0f : getNearestMin(buffer_min);

    if(top_value == bottom_value){
        if(top_value == 0.0f){
            top_value = 1.0f;
        }else{
            top_value += 0.5f;
            bottom_value -= 0.5f;
        }
    }

    QFont name_font(font());
    scaleFont(name_font, height()/5, 8, 12);

    QFontMetrics fm(name_font);

    int delta = fm.height() / 2;

    painter.setPen(param_color);

    if(!buffer->isEmpty()){

        auto calc_y = [top_value, bottom_value, this](float value) -> int{
            return static_cast<int>((1.0f - (value - bottom_value) / (top_value - bottom_value)) * height());
        };

        int x = width() - buffer->size(), y = 0;

        auto iter = buffer->begin();
        ++ iter;

        for(; iter != buffer->end(); ++ iter){
            x = x + 1;
            y = calc_y(*iter);
            painter.drawLine(x, y, x, height() - 1);
        }
    }

    QString value_str = param ? param->toString() : QString();
    QString top_str = param ? Parameter::number(param->type(), top_value) : QString();
    QString bottom_str = param ? Parameter::number(param->type(), bottom_value) : QString();

    painter.setPen(palette().light().color());
    painter.setFont(name_font);

    drawTextAt(&painter, param_name, delta, delta, TEXT_POS_BOTTOM_RIGHT);
    drawTextAt(&painter, value_str, delta, height() - delta, TEXT_POS_TOP_RIGHT);
    drawTextAt(&painter, top_str, width() - delta, delta, TEXT_POS_BOTTOM_LEFT);
    drawTextAt(&painter, bottom_str, width() - delta, height() - delta, TEXT_POS_TOP_LEFT);
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
    if(value == 0.0f) return 0.0f;

    float cur_base = pow(10.0f, floor(log10(fabs(value))));
    float n = ceil(value / cur_base);
    float res = cur_base * n;

    //qDebug() << value << cur_base << n << res;

    if(value >= res) res += cur_base;

    return res;
}

float ParamView::getNearestMin(float value)
{
    if(value == 0.0f) return 0.0f;

    float cur_base = pow(10.0f, floor(log10(fabs(value))));
    float n = floor(value / cur_base);
    float res = cur_base * n;

    //qDebug() << value << cur_base << n << res;

    if(value <= res) res -= cur_base;

    return res;
}

