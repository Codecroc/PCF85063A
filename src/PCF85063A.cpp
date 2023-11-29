#include "PCF85063A.hpp"

uint8_t PCF85063A::begin()
{
    return Wire.begin();
}

uint8_t PCF85063A::begin(gpio_num_t sda, gpio_num_t scl, uint32_t frequency)
{
    return Wire.begin(sda, scl, frequency);
}

void PCF85063A::end()
{
    Wire.end();
}

uint8_t PCF85063A::_decToBcd(uint8_t val)
{
    return ( (val/10*16) + (val%10) );
}

uint8_t PCF85063A::_bcdToDec(uint8_t val)
{
    return ( (val/16*10) + (val%16) );
}

uint8_t PCF85063A::setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, PCF85063A_WEEKDAYS_t weekdays, uint8_t months, uint8_t years)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_SECONDS);
    if (seconds > 59) seconds = 0;
    Wire.write(this->decToBcd(seconds));
    if (minutes > 59) minutes = 0;
    Wire.write(this->decToBcd(minutes));
    if (hours > 23) hours = 0;
    Wire.write(this->decToBcd(hours));
    if (days > 31) days = 0;
    Wire.write(this->decToBcd(days));
    if (weekdays > 6) weekdays = PCF85063A_WEEKDAY_SUNDAY;
    Wire.write(weekdays);
    if (months > 12) months = 0;
    Wire.write(this->decToBcd(months));
    if (years > 99) years = 0;
    Wire.write(this->decToBcd(years));
    return Wire.endTransmission();
}

uint8_t PCF85063A::setAlarm(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, PCF85063A_WEEKDAYS_t weekdays)
{
    // Alarm is enabled by setting the MSB of the registers to 0
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_ALARM_SECONDS);
    if (seconds > 59) seconds = 0;
    Wire.write(this->decToBcd(seconds) | 0b10000000);
    if (minutes > 59) minutes = 0;
    Wire.write(this->decToBcd(minutes) | 0b10000000);
    if (hours > 23) hours = 0;
    Wire.write(this->decToBcd(hours) | 0b10000000);
    if (days > 31) days = 0;
    Wire.write(this->decToBcd(days));
    if (weekdays > 6) weekdays = PCF85063A_WEEKDAY_SUNDAY;
    Wire.write(weekdays | 0b10000000);
    return Wire.endTransmission();
}

uint8_t PCF85063A::enableAlarm(uint8_t enable)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_ALARM_SECONDS);
    Wire.write((0b00000001 & enable) << 7); // Set enable bit for seconds
    Wire.write((0b00000010 & enable) << 6); // Set enable bit for minutes
    Wire.write((0b00000100 & enable) << 5); // Set enable bit for hours
    Wire.write((0b00001000 & enable) << 4); // Set enable bit for days
    Wire.write((0b00010000 & enable) << 3); // Set enable bit for weekdays
    return Wire.endTransmission();
}

uint8_t PCF85063A::setTimer(uint8_t value, uint8_t mode)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_TIMER_VALUE);
    Wire.write(value);
    Wire.write(mode);
    return Wire.endTransmission();
}

uint8_t PCF85063A::setTimerMode(uint8_t mode)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_TIMER_MODE);
    Wire.write(mode);
    return Wire.endTransmission();
}

uint8_t PCF85063A::setTimerValue(uint8_t value)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_TIMER_VALUE);
    Wire.write(value);
    return Wire.endTransmission();
}

uint8_t PCF85063A::setOffset(byte mode, int8_t offset)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_OFFSET);
    Wire.write(mode << 7);
    Wire.write(offset & 0b00111111);
    return Wire.endTransmission();
}

uint8_t PCF85063A::setControl1(uint8_t control1)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_CONTROL_1);
    Wire.write(control1);
    return Wire.endTransmission();
}

uint8_t PCF85063A::setControl2(uint8_t control2)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_CONTROL_2);
    Wire.write(control2);
    return Wire.endTransmission();
}

uint8_t PCF85063A::setRam(uint8_t value)
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_RAM);
    Wire.write(value);
    return Wire.endTransmission();
}

uint8_t PCF85063A::getRam(uint8_t &value)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_RAM);
    error = Wire.endTransmission();
    Wire.requestFrom(PCF85063A_ADDRESS, 1);
    value = Wire.read();
    return error;
}

uint8_t PCF85063A::getControl1(uint8_t &value)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_CONTROL_1);
    error = Wire.endTransmission();
    Wire.requestFrom(PCF85063A_ADDRESS, 1);
    value = Wire.read();
    return error;
}

uint8_t PCF85063A::getControl2(uint8_t &value)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_CONTROL_2);
    error = Wire.endTransmission();
    Wire.requestFrom(PCF85063A_ADDRESS, 1);
    value = Wire.read();
    return error;
}

uint8_t PCF85063A::getOffset(uint8_t &mode, int8_t &value)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_OFFSET);
    error = Wire.endTransmission();

    Wire.requestFrom(PCF85063A_ADDRESS, 1);
    uint8_t reg = Wire.read();
    mode = (reg & 0b10000000) >> 7;
    value = reg & 0b00111111;
    return error;
}

uint8_t PCF85063A::getTimerMode(uint8_t &value)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_TIMER_MODE);
    error = Wire.endTransmission();
    Wire.requestFrom(PCF85063A_ADDRESS, 1);
    value = Wire.read();
    return error;
}

uint8_t PCF85063A::getTimerValue(uint8_t &value)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_TIMER_VALUE);
    error = Wire.endTransmission();

    Wire.requestFrom(PCF85063A_ADDRESS, 1);
    value = Wire.read();
    return error;
}

uint8_t PCF85063A::getOS(uint8_t &value)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_SECONDS);

    error = Wire.endTransmission();
    Wire.requestFrom(PCF85063A_ADDRESS, 1);
    value = ((Wire.read() & 0b10000000) >> 7);
    return error;
}

uint8_t PCF85063A::getTime(uint8_t &seconds, uint8_t &minutes, uint8_t &hours, uint8_t &days, PCF85063A_WEEKDAYS_t &weekdays, uint8_t &months, uint8_t &years)
{
    uint8_t error;
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_SECONDS);

    error = Wire.endTransmission();
    Wire.requestFrom(PCF85063A_ADDRESS, 7);
    seconds = this->bcdToDec(Wire.read() & 0b01111111);
    minutes = this->bcdToDec(Wire.read() & 0b01111111);
    hours = this->bcdToDec(Wire.read() & 0b00111111);
    days = this->bcdToDec(Wire.read() & 0b00111111);
    weekdays = (PCF85063A_WEEKDAYS_t)(Wire.read() & 0b00000111);
    months = this->bcdToDec(Wire.read() & 0b00011111);
    years = this->bcdToDec(Wire.read() & 0b01111111);
    return error;
}

uint8_t PCF85063A::clearOS()
{
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_SECONDS);
    Wire.endTransmission();
    uint8_t reg = Wire.read();
    Wire.beginTransmission(PCF85063A_ADDRESS);
    Wire.write(PCF85063A_REG_SECONDS);
    Wire.write(reg & 0b01111111);
    return Wire.endTransmission();
}