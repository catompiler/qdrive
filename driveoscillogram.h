#ifndef DRIVEOSCILLOGRAM_H
#define DRIVEOSCILLOGRAM_H


#include "drive_types.h"
#include <stddef.h>


/**
 * @brief Класс осциллограммы привода.
 */
class DriveOscillogram
{
public:
    DriveOscillogram();
    DriveOscillogram(const DriveOscillogram& osc);
    ~DriveOscillogram();

    /**
     * @brief Класс канала осциллограммы.
     */
    class Channel{
    public:
        Channel();
        Channel(const Channel& channel);
        Channel(osc_value_t* values, size_t count);
        ~Channel();

        /**
         * @brief Устанавливает данные канала осциллограммы.
         * @param index Позиция в данных канала осциллограммы.
         * @param values Значения канала осциллограммы.
         * @param count Число данных для установки.
         */
        void setData(size_t index, const osc_value_t* values, size_t count);

        /**
         * @brief Устанавливает данные канала осциллограммы.
         * @param index Позиция в данных канала осциллограммы.
         * @param values Значения канала осциллограммы.
         * @param count Число данных для установки.
         */
        void setData(size_t index, const float* values, size_t count);

        /**
         * @brief Получает размер данных канала осциллограммы.
         * @return Размер данных канала осциллограммы.
         */
        size_t size() const;

        /**
         * @brief Получает продолжительность
         * канала осциллограммы в мс.
         * @return Продолжительность.
         */
        float timeMs() const;

        /**
         * @brief Получает значение канала осциллограммы.
         * index должен быть валидным.
         * @param index Индекс данных.
         * @return Значение.
         */
        const float& value(size_t index) const;

        /**
         * @brief Получает значение канала осциллограммы.
         * index должен быть валидным.
         * @param index Индекс данных.
         * @return Значение.
         */
        float& value(size_t index);

        /**
         * @brief Устанавливает значение канала осциллограммы.
         * @param index Индекс данных.
         * @param val Значение.
         */
        void setValue(size_t index, float val);

        const float& operator[](size_t index) const;
        float& operator[](size_t index);

        /**
         * @brief Распаковывает упакованные данные
         * канала осциллограммы во float.
         * @param val Упакованные данные.
         * @return Распакованные данные.
         */
        static float unpack(osc_value_t val);

    private:
        //! Данные канала осциллограммы.
        float* data_ptr;//[DRIVE_POWER_OSC_CHANNEL_LEN];
    };

    /**
     * @brief Получает идентификатор события осциллограммы.
     * @return Идентификатор события.
     */
    drive_event_id_t eventId() const;

    /**
     * @brief Устанавливает идентификатор события осциллограммы.
     * @param id Идентификатор события осциллограммы.
     */
    void setEventId(drive_event_id_t id);

    /**
     * @brief Получает количество каналов осциллограммы.
     * @return Количество каналов осциллограммы.
     */
    size_t channelsCount() const;

    /**
     * @brief Получает канал осциллограммы
     * с заданным индексом.
     * @param index Индекс канала осциллограммы.
     * @return Канал осциллограммы.
     */
    const Channel* channel(size_t index) const;

    /**
     * @brief Получает канал осциллограммы
     * с заданным индексом.
     * @param index Индекс канала осциллограммы.
     * @return Канал осциллограммы.
     */
    Channel* channel(size_t index);

private:
    //! Идентификатор события осциллограммы.
    drive_event_id_t event_id;
    //! Каналы осциллограммы.
    Channel* channels;//[DRIVE_POWER_OSC_CHANNELS_COUNT];
};

#endif // DRIVEOSCILLOGRAM_H
