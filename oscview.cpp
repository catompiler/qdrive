#include "oscview.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QBrush>
#include "utils.h"
#include <math.h>
#include <QDebug>


using namespace utils::paint;



OscView::OscView(QWidget *parent) : QWidget(parent)
{
    waveforms = new WaveformsList();
    main_pixmap = new QPixmap();

    view_settings = new QStack<ViewSettings>();
    view_settings->resize(1);

    is_dragging = false;
    is_selecting = false;

    /*Waveform* wf_a = new Waveform();
    Waveform* wf_b = new Waveform();
    Waveform* wf_c = new Waveform();

    wf_a->resize(21);
    wf_b->resize(21);
    wf_c->resize(21);

    for(int i = 0; i <= 20; i ++){
        QPointF& pa = wf_a->point(i);
        pa.setX(-10 + i);
        pa.setY(310.0 * sin((2.0 * M_PI) * i / 20));

        QPointF& pb = wf_b->point(i);
        pb.setX(-10 + i);
        pb.setY(310.0 * sin((2.0 * M_PI) * i / 20 + 2.0 * M_PI / 3.0));

        QPointF& pc = wf_c->point(i);
        pc.setX(-10 + i);
        pc.setY(310.0 * sin((2.0 * M_PI) * i / 20 + 4.0 * M_PI / 3.0));
    }

    waveforms->insert(0, qMakePair(wf_a, Qt::yellow));
    waveforms->insert(1, qMakePair(wf_b, Qt::green));
    waveforms->insert(2, qMakePair(wf_c, Qt::red));*/
}

OscView::~OscView()
{
    clearWaveforms();
    delete view_settings;
    delete main_pixmap;
    delete waveforms;
}

size_t OscView::count() const
{
    return static_cast<size_t>(waveforms->size());
}

QSize OscView::sizeHint() const
{
    return QSize(XMARGIN * 3, YMARGIN * 3);
}

QSize OscView::minimumSizeHint() const
{
    return QSize(XMARGIN * 6, YMARGIN * 5);
}

void OscView::setWaveform(int id, OscView::Waveform* wf, const QColor& color)
{
    if(waveforms->contains(id)){
        auto pair = (*waveforms)[id];
        delete pair.first;
        pair.first = wf;
        pair.second = color;
    }else{
        waveforms->insert(id, qMakePair(wf, color));
    }

    updatePixmap();
    update();
}

void OscView::setWaveform(int id, OscView::Waveform* wf)
{
    if(waveforms->contains(id)){
        auto pair = (*waveforms)[id];
        delete pair.first;
        pair.first = wf;
    }else{
        waveforms->insert(id, qMakePair(wf, QColor()));
    }

    updatePixmap();
    update();
}

void OscView::setWaveformColor(int id, const QColor& color)
{
    if(waveforms->contains(id)){
        (*waveforms)[id].second = color;

        updatePixmap();
        update();
    }
}

void OscView::delWaveform(int id)
{
    waveforms->remove(id);

    updatePixmap();
    update();
}

void OscView::clearWaveforms()
{
    for(auto it = waveforms->begin(); it != waveforms->end(); ++ it) delete it.value().first;
    waveforms->clear();

    updatePixmap();
    update();
}

void OscView::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *main_pixmap);

    if(is_selecting){
        painter.setPen(Qt::white);
        painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        painter.drawRect(sel_rect);
    }
}

void OscView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    delete main_pixmap;
    main_pixmap = new QPixmap(event->size());

    updatePixmap();
}

void OscView::mouseDoubleClickEvent(QMouseEvent* /*event*/)
{
    adjustView();
}

void OscView::mouseMoveEvent(QMouseEvent* event)
{
    if(is_dragging){
        QPointF diff_pos = unmap(event->pos()) - unmap(old_pos);
        old_pos = event->pos();

        ViewSettings& vs = view_settings->top();
        vs.step(diff_pos);

        updatePixmap();
        update();
    }else if(is_selecting){
        sel_rect.setBottomRight(event->pos());
        update();
    }
}

void OscView::mousePressEvent(QMouseEvent* event)
{
    switch(event->button()){
    case Qt::LeftButton:
        is_dragging = true;
        old_pos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        break;
    case Qt::RightButton:
        if(is_dragging) break;
        is_selecting = true;
        sel_rect = QRect(event->pos(), event->pos());
        setCursor(Qt::CrossCursor);
        break;
    default:
        break;
    }
}

void OscView::mouseReleaseEvent(QMouseEvent* event)
{
    QRectF rect_view;

    switch(event->button()){
    case Qt::LeftButton:
        is_dragging = false;
        setCursor(Qt::ArrowCursor);
        break;
    case Qt::RightButton:
        if(is_dragging) break;

        is_selecting = false;
        setCursor(Qt::ArrowCursor);

        sel_rect = sel_rect.normalized();

        rect_view = QRectF(unmap(sel_rect.topLeft()), unmap(sel_rect.bottomRight()));

        view_settings->top().view(rect_view);

        updatePixmap();
        update();
        break;
    default:
        break;
    }
}

void OscView::wheelEvent(QWheelEvent* event)
{
    QRect graph_rect = QRect(XMARGIN, YMARGIN, width() - 2 * XMARGIN, height() - 2 * YMARGIN);
    if(graph_rect.contains(event->pos())){
        ViewSettings& vs = view_settings->top();
        if(event->delta() > 0)
            vs.zoomIn(2.0);
        else
            vs.zoomOut(2.0);

        //view_settings->append(vs);

        updatePixmap();
        update();
    }else if(event->pos().x() < XMARGIN || event->pos().x() > (width() - XMARGIN)){
        ViewSettings& vs = view_settings->top();
        if(event->delta() > 0)
            vs.zoomInY(2.0);
        else
            vs.zoomOutY(2.0);

        //view_settings->append(vs);

        updatePixmap();
        update();
    }else if(event->pos().y() < YMARGIN || event->pos().y() > (height() - YMARGIN)){
        ViewSettings& vs = view_settings->top();
        if(event->delta() > 0)
            vs.zoomInX(2.0);
        else
            vs.zoomOutX(2.0);

        //view_settings->append(vs);

        updatePixmap();
        update();
    }
}

void OscView::updatePixmap()
{
    QPainter painter(main_pixmap);

    view_settings->top().adjustMarks(width() / XMARGIN, height() / YMARGIN);
    painter.fillRect(main_pixmap->rect(), palette().dark());

    drawAxes(&painter);

    for(auto it = waveforms->begin(); it != waveforms->end(); ++ it){
        painter.setPen(it.value().second);
        drawWaveform(&painter, it.value().first);
    }
}

void OscView::drawAxes(QPainter* painter)
{
    painter->setPen(palette().midlight().color());
    painter->drawRect(XMARGIN, YMARGIN, width() - 2 * XMARGIN, height() - 2 * YMARGIN);

    drawXAxis(painter);
    drawYAxis(painter);
}

void OscView::drawXAxis(QPainter* painter)
{
    qreal delta = MARK_LEN  + MARK_LEN / 2;

    ViewSettings& vs = view_settings->top();

    for(const qreal& mark_pos: vs.marksX()){
        int map_x = mapX(mark_pos);

        int x = map_x;
        int y = height() - YMARGIN;

        painter->setPen(palette().midlight().color());
        painter->drawLine(x, y, x, y + MARK_LEN);
        painter->setPen(QPen(palette().midlight(), 1, Qt::DotLine));
        painter->drawLine(x, y, x, YMARGIN);

        painter->setPen(palette().light().color());
        drawTextAt(painter, QString::number(mark_pos, 'f', 2), x, y + delta, TEXT_POS_BOTTOM_CENTER);
    }
}

void OscView::drawYAxis(QPainter* painter)
{
    qreal delta = MARK_LEN + MARK_LEN / 2;

    ViewSettings& vs = view_settings->top();

    for(const qreal& mark_pos: vs.marksY()){
        int map_y = mapY(mark_pos);

        int x = XMARGIN;
        int y = map_y;

        painter->setPen(palette().midlight().color());
        painter->drawLine(x, y, XMARGIN - MARK_LEN, y);
        painter->setPen(QPen(palette().midlight(), 1, Qt::DotLine));
        painter->drawLine(x, y, width() - XMARGIN, y);

        painter->setPen(palette().light().color());
        drawTextAt(painter, QString::number(mark_pos, 'f', 2), x - delta, y, TEXT_POS_LEFT_CENTER);
    }
}

void OscView::drawWaveform(QPainter* painter, OscView::Waveform* wf)
{
    if(wf->size() == 0) return;

    painter->setClipRect(XMARGIN, YMARGIN, width() - 2 * XMARGIN, height() - 2 * YMARGIN);

    QPoint old_p;

    for(size_t i = 0; i < wf->size(); i ++){
        const QPointF& p = wf->point(i);

        int x = mapX(p.x());
        int y = mapY(p.y());

        if(i != 0){
            painter->drawLine(old_p.x(), old_p.y(), x, y);
        }
        old_p.setX(x);
        old_p.setY(y);
    }
}

int OscView::mapX(qreal point_x)
{
    int max_size = width() - 2 * XMARGIN;
    ViewSettings& vs = view_settings->top();

    point_x -= vs.left();

    return static_cast<int>(point_x * max_size / vs.dx()) + XMARGIN;
}

int OscView::mapY(qreal point_y)
{
    int max_size = height() - 2 * YMARGIN;
    ViewSettings& vs = view_settings->top();

    point_y -= vs.bottom();

    return height() - (static_cast<int>(point_y * max_size / vs.dy()) + YMARGIN);
}

QPoint OscView::map(const QPointF& point)
{
    return QPoint(mapX(point.x()), mapY(point.y()));
}

qreal OscView::unmapX(int point_x)
{
    int max_size = width() - 2 * XMARGIN;
    ViewSettings& vs = view_settings->top();

    point_x -= XMARGIN;

    return static_cast<qreal>(point_x) * vs.dx() / max_size + vs.left();
}

qreal OscView::unmapY(int point_y)
{
    int max_size = height() - 2 * YMARGIN;
    ViewSettings& vs = view_settings->top();

    point_y = height() - point_y - YMARGIN;

    return static_cast<qreal>(point_y) * vs.dy() / max_size + vs.bottom();
}

QPointF OscView::unmap(const QPoint& point)
{
    return QPointF(unmapX(point.x()), unmapY(point.y()));
}

void OscView::adjustView()
{
    if(waveforms->empty()) return;

    QRectF rect_view;

    for(auto it = waveforms->begin(); it != waveforms->end(); ++ it){
        if(rect_view.isNull())
            rect_view = it.value().first->boundRect();
        else
            rect_view = rect_view.united(it.value().first->boundRect());
    }

    //qDebug() << rect_view;

    if(rect_view.height() > 0){
        qreal h = rect_view.height();
        rect_view.setTop(rect_view.top() + h);
        rect_view.setHeight(h);
    }

    view_settings->top().view(rect_view);
    //view_settings->top().zoomOut(1.1);

    updatePixmap();
    update();
}

OscView::Waveform::Waveform()
{
    points = nullptr;
}

OscView::Waveform::Waveform(const OscView::Waveform& wave)
{
    if(wave.size()){
        points = new QVector<QPointF>(wave.size());
        for(size_t i = 0; i < wave.size(); i ++){
            QPointF& dst = (*points)[i];
            const QPointF& src = (*wave.points)[i];
            dst.setX(src.x());
            dst.setY(src.y());
        }
    }else{
        points = nullptr;
    }
}

OscView::Waveform::Waveform(const DriveOscillogram::Channel* channel)
{
    if(channel->size()){
        points = new QVector<QPointF>(channel->size());
        float ms_per_point = 0.0f;
        if(channel->size() > 1){
            ms_per_point = static_cast<float>(channel->timeMs()) / (channel->size() - 1);
        }
        for(size_t i = 0; i < channel->size(); i ++){
            QPointF& p = (*points)[i];
            p.setX(ms_per_point * i);
            p.setY(channel->value(i));
        }
    }else{
        points = nullptr;
    }
}

OscView::Waveform::~Waveform()
{
    delete points;
}

size_t OscView::Waveform::size() const
{
    if(points) return static_cast<size_t>(points->size());
    return 0;
}

void OscView::Waveform::resize(size_t s)
{
    if(!points) points = new QVector<QPointF>(s);
    else points->resize(s);
}

const QPointF& OscView::Waveform::point(size_t index) const
{
    return (*points)[index];
}

QPointF& OscView::Waveform::point(size_t index)
{
    return (*points)[index];
}

QRectF OscView::Waveform::boundRect() const
{
    if(!points || points->empty()) return QRectF();

    QRectF res(point(0), point(size() - 1));

    for(size_t i = 0; i < size(); i ++){
        const QPointF& p = point(i);

        if(res.left() > p.x()) res.setLeft(p.x());
        else if(res.right() < p.x()) res.setRight(p.x());
        if(res.top() < p.y()) res.setTop(p.y());
        else if(res.bottom() > p.y()) res.setBottom(p.y());
    }

    return res;
}

OscView::ViewSettings::ViewSettings()
{
    view_rect = QRectF(-10.0, 400.0, 20.0, 800.0);
}

OscView::ViewSettings::ViewSettings(const OscView::ViewSettings& settings)
{
    view_rect = settings.view_rect;
    x_marks = settings.x_marks;
    y_marks = settings.y_marks;
}

OscView::ViewSettings::~ViewSettings()
{
}

QRectF&OscView::ViewSettings::rect()
{
    return view_rect;
}

const QRectF&OscView::ViewSettings::rect() const
{
    return view_rect;
}

qreal OscView::ViewSettings::dx() const
{
    return view_rect.width();
}

qreal OscView::ViewSettings::dy() const
{
    return view_rect.height();
}

qreal OscView::ViewSettings::left() const
{
    return view_rect.left();
}

qreal OscView::ViewSettings::bottom() const
{
    return view_rect.top() - view_rect.height();
}

qreal OscView::ViewSettings::top() const
{
    return view_rect.top();
}

qreal OscView::ViewSettings::right() const
{
    return view_rect.right();
}

void OscView::ViewSettings::adjustMarks(size_t ideal_x_count, size_t ideal_y_count)
{
    qreal x_step = adjustAxisStep(dx() / ideal_x_count);
    qreal y_step = adjustAxisStep(dy() / ideal_y_count);

    qreal x_first = ceil(left() / x_step) * x_step;
    qreal y_first = ceil(bottom() / y_step) * y_step;

    x_marks.clear();
    while(x_first <= right()){
        x_marks.append(x_first);
        x_first += x_step;
    }

    y_marks.clear();
    while(y_first <= top()){
        y_marks.append(y_first);
        y_first += y_step;
    }
}

const QVector<qreal>& OscView::ViewSettings::marksX() const
{
    return x_marks;
}

const QVector<qreal>& OscView::ViewSettings::marksY() const
{
    return y_marks;
}

void OscView::ViewSettings::stepX(qreal steps)
{
    view_rect.moveLeft(view_rect.left() - steps);
}

void OscView::ViewSettings::stepY(qreal steps)
{
    view_rect.moveTop(view_rect.top() - steps);
}

void OscView::ViewSettings::step(QPointF steps)
{
    stepX(steps.x());
    stepY(steps.y());
}

void OscView::ViewSettings::zoomInX(qreal mult)
{
    if(mult == 0.0) return;

    qreal new_width = view_rect.width() / mult;
    qreal delta = (view_rect.width() - new_width) / 2.0;

    view_rect.setLeft(view_rect.left() + delta);
    view_rect.setWidth(new_width);
}

void OscView::ViewSettings::zoomInY(qreal mult)
{
    if(mult == 0.0) return;

    qreal new_height = view_rect.height() / mult;
    qreal delta = (view_rect.height() - new_height) / 2.0;

    view_rect.setTop(view_rect.top() - delta);
    view_rect.setHeight(new_height);
}

void OscView::ViewSettings::zoomIn(qreal mult)
{
    zoomInX(mult);
    zoomInY(mult);
}

void OscView::ViewSettings::zoomOutX(qreal mult)
{
    if(mult == 0.0) return;

    qreal new_width = view_rect.width() * mult;
    qreal delta = (new_width - view_rect.width()) / 2.0;

    view_rect.setLeft(view_rect.left() - delta);
    view_rect.setWidth(new_width);
}

void OscView::ViewSettings::zoomOutY(qreal mult)
{
    if(mult == 0.0) return;

    qreal new_height = view_rect.height() * mult;
    qreal delta = (new_height - view_rect.height()) / 2.0;

    view_rect.setTop(view_rect.top() + delta);
    view_rect.setHeight(new_height);
}

void OscView::ViewSettings::zoomOut(qreal mult)
{
    zoomOutX(mult);
    zoomOutY(mult);
}

void OscView::ViewSettings::view(const QRectF& rect_view)
{
    view_rect.setTopLeft(rect_view.topLeft());
    view_rect.setSize(rect_view.normalized().size());
}

qreal OscView::ViewSettings::adjustAxisStep(qreal ideal_step)
{
    qreal step_exp = floor(log10(ideal_step));

    qreal step_pow = pow(10.0, step_exp);

    qreal step_mul = ceil(ideal_step / step_pow);

    qreal step = step_pow * step_mul;

    return step;
}
