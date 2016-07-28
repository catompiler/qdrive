#ifndef OSCVIEW_H
#define OSCVIEW_H

#include <QWidget>
#include <QPointF>
#include <QPoint>
#include <QVector>
#include <QHash>
#include <QStack>
#include <QSize>
#include <QPair>
#include <QColor>
#include "driveoscillogram.h"

class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class QWheelEvent;
class QPainter;
class QPixmap;


class OscView : public QWidget
{
    Q_OBJECT
public:
    explicit OscView(QWidget *parent = 0);
    ~OscView();

    class Waveform{
    public:
        Waveform();
        Waveform(const Waveform& wave);
        Waveform(const DriveOscillogram::Channel* channel);
        ~Waveform();

        /**
         * @brief Получает количество точек в осциллограмме.
         * @return Количество точек.
         */
        size_t size() const;

        /**
         * @brief Устанавливает новый размер осциллограммы.
         * @param s Размер осциллограммы.
         */
        void resize(size_t s);

        /**
         * @brief Получает точку с заданным индексом.
         * @param index Индекс точки.
         * @return Точка с заданным индексом.
         */
        const QPointF& point(size_t index) const;

        /**
         * @brief Получает точку с заданным индексом.
         * @param index Индекс точки.
         * @return Точка с заданным индексом.
         */
        QPointF& point(size_t index);

        /**
         * @brief Получает прямоугольник - границу осциллограммы.
         * @return Граница осциллограммы.
         */
        QRectF boundRect() const;

    private:
        QVector<QPointF>* points;
    };

    /**
     * @brief Получает количество осциллограмм.
     * @return Количество осциллограмм.
     */
    size_t count() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    /**
     * @brief Устанавливает осциллограмму для просмотра.
     * Осциллограмма будет удалена просмотрщиком.
     * @param id Идентификатор осциллограммы.
     * @param wf Осциллограмма.
     * @param color Цвет осциллограммы.
     */
    void setWaveform(int id, Waveform* wf, const QColor& color);

    /**
     * @brief Устанавливает осциллограмму для просмотра.
     * Осциллограмма будет удалена просмотрщиком.
     * @param id Идентификатор осциллограммы.
     * @param wf Осциллограмма.
     */
    void setWaveform(int id, Waveform* wf);

    /**
     * @brief Устанавливает цвет осциллограммы.
     * @param id Идентификатор осциллограммы.
     * @param color Цвет осциллограммы.
     */
    void setWaveformColor(int id, const QColor& color);

    /**
     * @brief Удаляет осциллограмму.
     * @param id Идентификатор осциллограммы.
     */
    void delWaveform(int id);

    /**
     * @brief Удаляет все осциллограммы.
     */
    void clearWaveforms();

    /**
     * @brief Приводит окно просмотра к наилучшему виду на осциллограммы.
     */
    void adjustView();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    class ViewSettings {
    public:
        ViewSettings();
        ViewSettings(const ViewSettings& settings);
        ~ViewSettings();

        QRectF& rect();
        const QRectF& rect() const;

        qreal dx() const;
        qreal dy() const;

        qreal left() const;
        qreal bottom() const;
        qreal top() const;
        qreal right() const;

        void adjustMarks(size_t ideal_x_count, size_t ideal_y_count);

        const QVector<qreal>& marksX() const;
        const QVector<qreal>& marksY() const;

        void stepX(qreal steps);
        void stepY(qreal steps);
        void step(QPointF steps);

        void zoomInX(qreal mult);
        void zoomInY(qreal mult);
        void zoomIn(qreal mult);

        void zoomOutX(qreal mult);
        void zoomOutY(qreal mult);
        void zoomOut(qreal mult);

        void view(const QRectF& rect_view);

    private:
        QRectF view_rect;
        QVector<qreal> x_marks;
        QVector<qreal> y_marks;

        qreal adjustAxisStep(qreal ideal_step);
    };

    typedef QHash<int, QPair<Waveform*, QColor>> WaveformsList;

    WaveformsList* waveforms;

    QStack<ViewSettings>* view_settings;

    QPixmap* main_pixmap;

    bool is_dragging;
    bool is_selecting;

    QPoint old_pos;
    QRect sel_rect;

    const int MARK_LEN = 5;
    const int XMARGIN = 55;
    const int YMARGIN = 25;

    void updatePixmap();
    void drawAxes(QPainter* painter);
    void drawXAxis(QPainter* painter);
    void drawYAxis(QPainter* painter);
    void drawWaveform(QPainter* painter, Waveform* wf);

    int mapX(qreal point_x);
    int mapY(qreal point_y);
    QPoint map(const QPointF& point);

    qreal unmapX(int point_x);
    qreal unmapY(int point_y);
    QPointF unmap(const QPoint& point);
};

#endif // OSCVIEW_H
