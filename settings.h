#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <stdint.h>

/**
 * @brief Класс настроек приложения.
 */
class Settings : public QObject
{
    Q_OBJECT
public:

    typedef enum _Parity {
        PARITY_NONE = 0,
        PARITY_EVEN = 1,
        PARITY_ODD = 2
    } Parity;

    typedef enum _StopBist {
        STOP_BITS_1 = 0,
        STOP_BITS_2 = 1
    } StopBits;

    /**
     * @brief Получения объекта синглтона настроек.
     * @return Объект настроек.
     */
    static Settings& get();

    /**
     * @brief Считывает настройки.
     * @return флаг успеха.
     */
    bool read();
    /**
     * @brief Записывает настройки.
     * @return флаг успеха.
     */
    bool write();

    const QString& lastPath() const;
    void setLastPath(const QString& path);

    const QString& ioDeviceName() const;
    void setIODeviceName(const QString& name);

    unsigned int baud() const;
    void setBaud(unsigned int value);

    Parity parity() const;
    void setParity(Parity value);

    StopBits stopBits() const;
    void setStopBits(StopBits value);

    uint8_t deviceAddress() const;
    void setDeviceAddress(uint8_t address);

    unsigned int deviceTimeout() const;
    void setDeviceTimeout(unsigned int value);

    unsigned int byteTimeout() const;
    void setByteTimeout(unsigned int value);

    unsigned int devicePeriod() const;
    void setDevicePeriod(unsigned int value);

signals:

public slots:

private:
    Settings();

    QString last_path;
    QString io_dev_name;
    unsigned int io_baud;
    Parity io_parity;
    StopBits io_stop_bits;
    uint8_t dev_address;
    unsigned int dev_timeout;
    unsigned int dev_byte_timeout;
    unsigned int dev_period;
};

#endif // SETTINGS_H
