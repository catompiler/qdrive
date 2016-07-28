#include "settings.h"
#include <QSettings>
#include <QVariant>


static const char* company_name = "PPTDev";
static const char* app_name = "QDrive";


Settings &Settings::get()
{
    static Settings settings;

    return settings;
}

bool Settings::read()
{
    QSettings settings(company_name, app_name);

#ifdef Q_OS_WIN
    io_dev_name = settings.value("io_dev_name", "COM1").toString();
#else
    io_dev_name = settings.value("io_dev_name", "/dev/ttyS0").toString();
#endif
    io_baud = settings.value("io_baud", 9600).toUInt();
    io_parity = static_cast<Settings::Parity>(settings.value("io_parity", 0).toUInt());
    io_stop_bits = static_cast<Settings::StopBits>(settings.value("io_stop_bits", 0).toUInt());
    dev_address = static_cast<uint8_t>(settings.value("dev_address", 0x1).toUInt());
    dev_timeout = settings.value("dev_timeout", 100).toUInt();
    dev_byte_timeout = settings.value("dev_byte_timeout", 100).toUInt();
    dev_period = settings.value("dev_period", 100).toUInt();

    return true;
}

bool Settings::write()
{
    QSettings settings(company_name, app_name);

    settings.setValue("io_dev_name", io_dev_name);
    settings.setValue("io_baud", io_baud);
    settings.setValue("io_parity", static_cast<unsigned int>(io_parity));
    settings.setValue("io_stop_bits", static_cast<unsigned int>(io_stop_bits));
    settings.setValue("dev_address", static_cast<unsigned int>(dev_address));
    settings.setValue("dev_timeout", dev_timeout);
    settings.setValue("dev_byte_timeout", dev_byte_timeout);
    settings.setValue("dev_period", dev_period);

    return true;
}

const QString &Settings::ioDeviceName() const
{
    return io_dev_name;
}

void Settings::setIODeviceName(const QString &name)
{
    io_dev_name = name;
}

unsigned int Settings::baud() const
{
    return io_baud;
}

void Settings::setBaud(unsigned int value)
{
    io_baud = value;
}

Settings::Parity Settings::parity() const
{
    return io_parity;
}

void Settings::setParity(Settings::Parity value)
{
    io_parity = value;
}

Settings::StopBits Settings::stopBits() const
{
    return io_stop_bits;
}

void Settings::setStopBits(Settings::StopBits value)
{
    io_stop_bits = value;
}

uint8_t Settings::deviceAddress() const
{
    return dev_address;
}

void Settings::setDeviceAddress(uint8_t address)
{
    dev_address = address;
}

unsigned int Settings::deviceTimeout() const
{
    return dev_timeout;
}

void Settings::setDeviceTimeout(unsigned int value)
{
    dev_timeout = value;
}

unsigned int Settings::byteTimeout() const
{
    return dev_byte_timeout;
}

void Settings::setByteTimeout(unsigned int value)
{
    dev_byte_timeout = value;
}

unsigned int Settings::devicePeriod() const
{
    return dev_period;
}

void Settings::setDevicePeriod(unsigned int value)
{
    dev_period = value;
}

Settings::Settings() : QObject()
{
}
