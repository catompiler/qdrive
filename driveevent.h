#ifndef DRIVEEVENT_H
#define DRIVEEVENT_H


#include <QSharedPointer>
#include "drive_types.h"

/**
 * @brief Класс события привода.
 */
class DriveEvent
{
public:
    DriveEvent();
    DriveEvent(const DriveEvent& event);
    DriveEvent(const drive_event_t& drive_event);

    /**
     * @brief Получает флаг валидности данных события.
     * @return Валидность данных события.
     */
    bool isValid() const;

    /**
     * @brief Получает идентификатор события.
     * @return Идентификатор события.
     */
    drive_event_id_t id() const;

    /**
     * @brief Получает тип события.
     * @return Тип события.
     */
    drive_event_type_t type() const;

    /**
     * @brief Получает состояние привода.
     * @return Состояние привода.
     */
    drive_state_t state() const;

    /**
     * @brief Получает направление вращения.
     * @return Направление вращения.
     */
    drive_dir_t direction() const;

    /**
     * @brief Получает состояние калибровки привода.
     * @return Состояние калибровки привода.
     */
    drive_power_calibration_t calibrationState() const;

    /**
     * @brief Получает состояние запуска привода.
     * @return Состояние запуска привода.
     */
    drive_starting_t startingState() const;

    /**
     * @brief Получает состояние останова привода.
     * @return Состояние останова привода.
     */
    drive_stopping_t stoppingState() const;

    /**
     * @brief Получает состояние останова привода при ошибке.
     * @return Состояние останова привода при ошибке.
     */
    drive_err_stopping_t errStoppingState() const;

    /**
     * @brief Получает задание привода.
     * @return Задание привода.
     */
    reference_t reference() const;

    /**
     * @brief Получает флаги привода.
     * @return Флаги привода.
     */
    drive_flags_t flags() const;

    /**
     * @brief Получает предупреждения привода.
     * @return Предупреждения привода.
     */
    drive_warnings_t warnings() const;

    /**
     * @brief Получает ошибки привода.
     * @return Ошибки привода.
     */
    drive_errors_t errors() const;

    /**
     * @brief Получает предупреждения питания привода.
     * @return Предупреждения питания привода.
     */
    drive_power_warnings_t powerWarnings() const;

    /**
     * @brief Получает ошибки питания привода.
     * @return Ошибки питания привода.
     */
    drive_power_errors_t powerErrors() const;

    /**
     * @brief Получает ошибки фаз привода.
     * @return Ошибки фаз привода.
     */
    drive_phase_errors_t phaseErrors() const;

    /**
     * @brief Получает время события.
     * @return Время события.
     */
    uint32_t time() const;

private:
    QSharedPointer<drive_event_t> event_impl;
};

#endif // DRIVEEVENT_H