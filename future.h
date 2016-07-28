#ifndef FUTURE_H
#define FUTURE_H

#include <QObject>
#include <QVariant>

/**
 * @brief Класс будущего с возможностью управления.
 */
class Future : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор.
     * @param parent Родитель.
     */
    explicit Future(QObject *parent = 0);
    //Future(const Future& future);
    ~Future();


    /**
     * @brief Получает флаг запущенности операции.
     * @return Флаг запущенности операции.
     */
    bool isRunning() const;

    /**
     * @brief Получает флаг завершения операции.
     * @return Флаг завершения операции.
     */
    bool isDone() const;

    /**
     * @brief Получает флаг необходимости отмены операции.
     * @return Флаг необходимости отмены операции.
     */
    bool needCancel() const;

    /**
     * @brief Получает прогресс операции.
     * @return Прогресс операции.
     */
    int progress() const;

    /**
     * @brief Получает минимальный прогресс операции.
     * @return Минимальный прогресс операции.
     */
    int progressMinimum() const;

    /**
     * @brief Получает минимальный прогресс операции.
     * @return Минимальный прогресс операции.
     */
    int progressMaximum() const;

    /**
     * @brief Ждёт завершения операции.
     */
    void wait() const;

signals:
    /**
     * @brief Сигнал начала операции.
     */
    void started();
    /**
     * @brief Сигнал завершения операции.
     */
    void finished();
    /**
     * @brief Сигнал изменения минимального прогресса.
     */
    void progressMinimumChanged(int);
    /**
     * @brief Сигнал изменения максимального прогресса.
     */
    void progressMaximumChanged(int);
    /**
     * @brief Сигнал изменения пределов изменения прогресса.
     */
    void progressRangeChanged(int, int);
    /**
     * @brief Сигнал изменения прогресса операции.
     */
    void progressChanged(int);

public slots:
    /**
     * @brief Запускает будущее.
     */
    void start();

    /**
     * @brief Устанавливает минимальный прогресс.
     * @param progress_min Минимальный прогресс.
     */
    void setProgressMinimum(int progress_min);

    /**
     * @brief Устанавливает максимальный прогресс.
     * @param progress_max Максимальный прогресс.
     */
    void setProgressMaximum(int progress_max);

    /**
     * @brief Устанавливает пределы изменения прогресса.
     * @param progress_min Минимальный прогресс.
     * @param progress_max Максимальный прогресс.
     */
    void setProgressRange(int progress_min, int progress_max);

    /**
     * @brief Устанавливает прогресс операции.
     * @param progress_val Прогресс операции.
     */
    void setProgress(int progress_val);

    /**
     * @brief Останавливает будущее.
     */
    void finish();

    /**
     * @brief Останавливает будущее.
     * @param res Результат выполнения операции.
     */
    void finish(const QVariant& res);

    /**
     * @brief Отмечает флаг необходимости отмены операции.
     */
    void cancel();

private:
    /**
     * @brief Флаг запуска операции.
     */
    volatile bool op_run;
    /**
     * @brief Флаг завершения операции.
     */
    volatile bool op_done;
    /**
     * @brief Флаг отмены операции.
     */
    volatile bool op_cancel;
    /**
     * @brief Прогресс операции.
     */
    int op_progress;
    /**
     * @brief Минимальный прогресс операции.
     */
    int op_progress_min;
    /**
     * @brief Максимальный прогресс операции.
     */
    int op_progress_max;
    /**
     * @brief Результат операции.
     */
    QVariant op_res;
};

//Q_DECLARE_METATYPE(Future)

#endif // FUTURE_H
