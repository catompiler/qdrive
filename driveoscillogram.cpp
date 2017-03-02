#include "driveoscillogram.h"
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QLocale>
#include <algorithm>
#include <string.h>
#include <new>


//#define DRIVE_OSC_CSV_EXPORT_SIZE


const char* DriveOscillogram::data_file_magic_csv = "OSC";
const char* DriveOscillogram::data_file_version_csv = "100";


DriveOscillogram::DriveOscillogram()
{
    channels = reinterpret_cast<Channel*>(
                    operator new(DRIVE_POWER_OSC_CHANNELS_COUNT * sizeof(Channel))
                );
    for(size_t i = 0; i < DRIVE_POWER_OSC_CHANNELS_COUNT; i ++){
        new (&channels[i]) Channel();
    }

    event_id = 0;
}

DriveOscillogram::DriveOscillogram(const DriveOscillogram &osc)
{
    channels = reinterpret_cast<Channel*>(
                    operator new(DRIVE_POWER_OSC_CHANNELS_COUNT * sizeof(Channel))
                );
    for(size_t i = 0; i < DRIVE_POWER_OSC_CHANNELS_COUNT; i ++){
        new (&channels[i]) Channel(osc.channels[i]);
    }
    event_id = osc.event_id;
}

DriveOscillogram::~DriveOscillogram()
{
    for(size_t i = 0; i < DRIVE_POWER_OSC_CHANNELS_COUNT; i ++){
        channels[i].~Channel();
    }
    operator delete(reinterpret_cast<void*>(channels));
}

drive_event_id_t DriveOscillogram::eventId() const
{
    return event_id;
}

void DriveOscillogram::setEventId(drive_event_id_t id)
{
    event_id = id;
}

size_t DriveOscillogram::channelsCount() const
{
    return DRIVE_POWER_OSC_CHANNELS_COUNT;
}

const DriveOscillogram::Channel *DriveOscillogram::channel(size_t index) const
{
    if(index >= DRIVE_POWER_OSC_CHANNELS_COUNT) return nullptr;

    return &channels[index];
}

DriveOscillogram::Channel *DriveOscillogram::channel(size_t index)
{
    if(index >= DRIVE_POWER_OSC_CHANNELS_COUNT) return nullptr;

    return &channels[index];
}

bool DriveOscillogram::save(const QString& filename) const
{
    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly)) return false;

    QDataStream ds(&file);

    ds.setVersion(QDataStream::Qt_5_0);

    ds << data_file_magic << data_file_version
       << (uint8_t)event_id << (uint8_t)channelsCount();

    if(ds.status() != QDataStream::Ok){
        file.close();
        return false;
    }

    size_t size_to_write;
    const Channel* ch;
    for(size_t i = 0; i < channelsCount(); i ++){
        ch = channel(i);

        ds << (uint32_t)ch->size();

        size_to_write = ch->size() * sizeof(float);
        if(ds.writeRawData(reinterpret_cast<const char*>(ch->data()), size_to_write)
                != static_cast<int>(size_to_write)){
            file.close();
            return false;
        }
    }

    file.close();

    return ds.status() == QDataStream::Ok;
}

bool DriveOscillogram::load(const QString& filename)
{
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)) return false;

    QDataStream ds(&file);

    ds.setVersion(QDataStream::Qt_5_0);

    uint32_t magic, version;
    uint8_t evid, count;

    ds >> magic >> version >> evid >> count;

    if(ds.status() != QDataStream::Ok){
        file.close();
        return false;
    }

    if(magic != data_file_magic) return false;
    if(version != data_file_version) return false;
    if(static_cast<size_t>(count) != channelsCount()) return false;

    event_id = evid;

    uint32_t ch_size;
    size_t size_to_read;
    Channel* ch;
    for(size_t i = 0; i < channelsCount(); i ++){
        ch = channel(i);

        ds >> ch_size;
        if(ch_size > ch->size()){
            file.close();
            return false;
        }
        size_to_read = ch_size * sizeof(float);
        if(ds.readRawData(reinterpret_cast<char*>(ch->data()), size_to_read)
                != static_cast<int>(size_to_read)){
            file.close();
            return false;
        }
    }

    file.close();

    return ds.status() == QDataStream::Ok;
}

bool DriveOscillogram::saveCsv(const QString& filename) const
{
    static const char* channels_names[DRIVE_POWER_OSC_CHANNELS_COUNT] = {
        "Ua", "Ub", "Uc", "Ia", "Ib", "Ic", "Urot", "Irot", "Iexc"
    };

    QFile file(filename);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream ts(&file);

    ts.setLocale(QLocale());

    ts << data_file_magic_csv << ";" << data_file_version_csv << ";"
       << (uint8_t)event_id << ";" << (uint8_t)channelsCount();
    ts << "\n";

    if(ts.status() != QTextStream::Ok){
        file.close();
        return false;
    }

    size_t max_size = 0;
    const Channel* ch = nullptr;

    for(size_t i = 0; i < channelsCount(); i ++){

        if(i != 0) ts << ";";

        if(i < DRIVE_POWER_OSC_CHANNELS_COUNT) ts << channels_names[i];
    }
    ts << "\n";

    for(size_t i = 0; i < channelsCount(); i ++){
#ifdef DRIVE_OSC_CSV_EXPORT_SIZE
        if(i != 0) ts << ";";

        ch = channel(i);

        ts << (uint32_t)ch->size();
#endif
        max_size = std::max(max_size, ch->size());
    }
#ifdef DRIVE_OSC_CSV_EXPORT_SIZE
    ts << "\n";
#endif

    for(size_t n = 0; n < max_size; n ++){

        if(n != 0) ts << "\n";

        for(size_t i = 0; i < channelsCount(); i ++){

            if(i != 0) ts << ";";

            ch = channel(i);

            if(n < ch->size()) ts << ch->value(n);
        }
    }
    ts << "\n";

    file.close();

    return ts.status() == QTextStream::Ok;
}

DriveOscillogram::Channel::Channel()
{
    data_ptr = new float[DRIVE_POWER_OSC_CHANNEL_LEN];
    memset(data_ptr, 0x0, DRIVE_POWER_OSC_CHANNEL_LEN * sizeof(float));
}

DriveOscillogram::Channel::Channel(const DriveOscillogram::Channel &channel)
{
    data_ptr = new float[DRIVE_POWER_OSC_CHANNEL_LEN];
    memcpy(data_ptr, channel.data_ptr, DRIVE_POWER_OSC_CHANNEL_LEN * sizeof(float));
}

DriveOscillogram::Channel::Channel(osc_value_t* values, size_t count)
    :Channel()
{
    setData(0, values, count);
}

DriveOscillogram::Channel::~Channel()
{
    delete[] data_ptr;
}

void DriveOscillogram::Channel::setData(size_t index, const osc_value_t* values, size_t count)
{
    if(index >= DRIVE_POWER_OSC_CHANNEL_LEN) return;

    size_t end = count + index;
    if(end > DRIVE_POWER_OSC_CHANNEL_LEN) end = DRIVE_POWER_OSC_CHANNEL_LEN;

    while(index != end){
        data_ptr[index ++] = unpack(*values ++);
    }
}

void DriveOscillogram::Channel::setData(size_t index, const float* values, size_t count)
{
    if(index >= DRIVE_POWER_OSC_CHANNEL_LEN) return;

    size_t end = count + index;
    if(end > DRIVE_POWER_OSC_CHANNEL_LEN) end = DRIVE_POWER_OSC_CHANNEL_LEN;

    while(index != end){
        data_ptr[index ++] = *values ++;
    }
}

float* DriveOscillogram::Channel::data()
{
    return data_ptr;
}

const float* DriveOscillogram::Channel::data() const
{
    return data_ptr;
}

size_t DriveOscillogram::Channel::size() const
{
    return DRIVE_POWER_OSC_CHANNEL_LEN;
}

float DriveOscillogram::Channel::timeMs() const
{
    return static_cast<float>(DRIVE_POWER_OSC_CHANNEL_TIME_MS);
}

const float& DriveOscillogram::Channel::value(size_t index) const
{
    return data_ptr[index];
}

float& DriveOscillogram::Channel::value(size_t index)
{
    return data_ptr[index];
}

void DriveOscillogram::Channel::setValue(size_t index, float val)
{
    if(index <= DRIVE_POWER_OSC_CHANNEL_LEN)
        data_ptr[index] = val;
}

const float& DriveOscillogram::Channel::operator[](size_t index) const
{
    return data_ptr[index];
}

float& DriveOscillogram::Channel::operator[](size_t index)
{
    return data_ptr[index];
}

float DriveOscillogram::Channel::unpack(osc_value_t val)
{
    return static_cast<float>(val) / (1 << (OSC_VALUE_FRACT_BITS)); // 1 bit sign + 10 bits int part + 5 bits fract part.
}
