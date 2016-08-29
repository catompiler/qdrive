#include "driveevent.h"

DriveEvent::DriveEvent()
{
}

DriveEvent::DriveEvent(const DriveEvent &event)
{
    event_impl = event.event_impl;
}

DriveEvent::DriveEvent(const drive_event_t &drive_event)
{
    event_impl = QSharedPointer<drive_event_t>(new drive_event_t());
    memcpy(event_impl.data(), &drive_event, sizeof(drive_event_t));
}

bool DriveEvent::isValid() const
{
    return !event_impl.isNull();
}

drive_event_id_t DriveEvent::id() const
{
    if(!isValid()) return 0;

    return event_impl->id;
}

drive_event_type_t DriveEvent::type() const
{
    if(!isValid()) return static_cast<drive_event_type_t>(0);

    return static_cast<drive_event_type_t>(event_impl->type);
}

drive_state_t DriveEvent::state() const
{
    if(!isValid()) return static_cast<drive_state_t>(0);

    return static_cast<drive_state_t>(event_impl->state);
}

drive_dir_t DriveEvent::direction() const
{
    if(!isValid()) return static_cast<drive_dir_t>(0);

    return static_cast<drive_dir_t>(event_impl->direction);
}

drive_init_state_t DriveEvent::initState() const
{
    if(!isValid()) return static_cast<drive_init_state_t>(0);

    return static_cast<drive_init_state_t>(event_impl->init_state);
}

drive_power_calibration_t DriveEvent::calibrationState() const
{
    if(!isValid()) return static_cast<drive_power_calibration_t>(0);

    return static_cast<drive_power_calibration_t>(event_impl->calibration_state);
}

drive_starting_t DriveEvent::startingState() const
{
    if(!isValid()) return static_cast<drive_starting_t>(0);

    return static_cast<drive_starting_t>(event_impl->starting_state);
}

drive_stopping_t DriveEvent::stoppingState() const
{
    if(!isValid()) return static_cast<drive_stopping_t>(0);

    return static_cast<drive_stopping_t>(event_impl->stopping_state);
}

drive_err_stopping_t DriveEvent::errStoppingState() const
{
    if(!isValid()) return static_cast<drive_err_stopping_t>(0);

    return static_cast<drive_err_stopping_t>(event_impl->err_stopping_state);
}

reference_t DriveEvent::reference() const
{
    if(!isValid()) return static_cast<reference_t>(0);

    return event_impl->reference;
}

drive_flags_t DriveEvent::flags() const
{
    if(!isValid()) return static_cast<drive_flags_t>(0);

    return event_impl->flags;
}

drive_warnings_t DriveEvent::warnings() const
{
    if(!isValid()) return static_cast<drive_warnings_t>(0);

    return event_impl->warnings;
}

drive_errors_t DriveEvent::errors() const
{
    if(!isValid()) return static_cast<drive_errors_t>(0);

    return event_impl->errors;
}

drive_power_warnings_t DriveEvent::powerWarnings() const
{
    if(!isValid()) return static_cast<drive_power_warnings_t>(0);

    return event_impl->power_warnings;
}

drive_power_errors_t DriveEvent::powerErrors() const
{
    if(!isValid()) return static_cast<drive_power_errors_t>(0);

    return event_impl->power_errors;
}

drive_phase_errors_t DriveEvent::phaseErrors() const
{
    if(!isValid()) return static_cast<drive_phase_errors_t>(0);

    return event_impl->phase_errors;
}

phase_time_t DriveEvent::phaseTimeA() const
{
    if(!isValid()) return static_cast<phase_time_t>(0);

    return event_impl->phase_a_time;
}

phase_time_t DriveEvent::phaseTimeB() const
{
    if(!isValid()) return static_cast<phase_time_t>(0);

    return event_impl->phase_b_time;
}

phase_time_t DriveEvent::phaseTimeC() const
{
    if(!isValid()) return static_cast<phase_time_t>(0);

    return event_impl->phase_c_time;
}

uint32_t DriveEvent::time() const
{
    if(!isValid()) return static_cast<uint32_t>(0);

    return event_impl->time;
}
