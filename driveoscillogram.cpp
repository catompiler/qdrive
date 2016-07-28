#include "driveoscillogram.h"
#include <string.h>
#include <new>



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
