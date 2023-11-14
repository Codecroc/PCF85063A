#pragma once
#include <Wire.h>

#define PCF85063A_ADDRESS 0x51
#define PCF85063A_REG_CONTROL_1 0x00
#define PCF85063A_REG_CONTROL_2 0x01
#define PCF85063A_REG_OFFSET 0x02
#define PCF85063A_REG_RAM 0x03
#define PCF85063A_REG_SECONDS 0x04
#define PCF85063A_REG_MINUTES 0x05
#define PCF85063A_REG_HOURS 0x06
#define PCF85063A_REG_DAYS 0x07
#define PCF85063A_REG_WEEKDAYS 0x08
#define PCF85063A_REG_MONTHS 0x09
#define PCF85063A_REG_YEARS 0x0A
#define PCF85063A_REG_ALARM_SECONDS 0x0B
#define PCF85063A_REG_ALARM_MINUTES 0x0C
#define PCF85063A_REG_ALARM_HOURS 0x0D
#define PCF85063A_REG_ALARM_DAYS 0x0E
#define PCF85063A_REG_ALARM_WEEKDAYS 0x0F
#define PCF85063A_REG_TIMER_VALUE 0x10
#define PCF85063A_REG_TIMER_MODE 0x11

class PCF85063A
{
public:
    static PCF85063A& getInstance()
    {
        static PCF85063A instance;
        return instance;
    }

    typedef enum {
        PCF85063A_WEEKDAY_SUNDAY = 0b00000000,
        PCF85063A_WEEKDAY_MONDAY = 0b00000001,
        PCF85063A_WEEKDAY_TUESDAY = 0b00000010,
        PCF85063A_WEEKDAY_WEDNESDAY = 0b00000011,
        PCF85063A_WEEKDAY_THURSDAY = 0b00000100,
        PCF85063A_WEEKDAY_FRIDAY = 0b00000101,
        PCF85063A_WEEKDAY_SATURDAY = 0b00000110
    } PCF85063A_WEEKDAYS_t;
    
    /**
     * Setup I2C connection
    */
    void begin();

    /**
     * Setup I2C connection with custom pins and frequency
    */
    void begin(gpio_num_t sda, gpio_num_t scl, uint32_t frequency = 400000);

    /**
     * Set the time for the RTC
    */
    void setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, PCF85063A_WEEKDAYS_t weekdays, uint8_t months, uint8_t years);
    
    /**
     * Set the alarm for the RTC
     * The alarm is only set but not enabled
     * To enable use the enableAlarm() function
    */
    void setAlarm(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t days, PCF85063A_WEEKDAYS_t weekdays);
    
    /**
     * Set a timer value and mode
     * Mode Bit 0: timer interrupt mode -> 0: flag, 1: pulse
     * Mode Bit 1: timer interrupt enable -> 0: disable, 1: enable
     * Mode Bit 2: timer enable -> 0: disable, 1: enable
     * Mode Bit 3-4: time clock frequency -> 00: 4096Hz, 01: 64Hz, 10: 1Hz, 11: 1/60Hz
    */
    void setTimer(uint8_t timerValue, uint8_t timerMode);

    /**
     * * Mode Bit 0: timer interrupt mode -> 0: flag, 1: pulse
     * Mode Bit 1: timer interrupt enable -> 0: disable, 1: enable
     * Mode Bit 2: timer enable -> 0: disable, 1: enable
     * Mode Bit 3-4: time clock frequency -> 00: 4096Hz, 01: 64Hz, 10: 1Hz, 11: 1/60Hz
    */    
    void setTimerMode(uint8_t timerMode);

    /**
     * Set the offset register
     * Bit 7: Mode -> 0 normal mode: offset is made once every two hours
     *                1 course mode: offset is made every 4 minutes
     * Bit 6-0: Offset -> 2's complement value
    */
    void setOffset(byte mode, int8_t offset);

    /**
     * Set the control register 1
     * Bit 7: EXT_TEST ->  0: normale mode
     *                     1: external clock test mode
     * Bit 6: unused
     * Bit 5: STOP     ->  0: clock running
     *                     1: clock stopped
     * Bit 4: SR       ->  0: no software reset
     *                     1: software reset, returns always 0
     * Bit 3: unused
     * Bit 2: CIE     ->  0: correction interrupt disabled
     *                    1: correction interrupt enabled
     * Bit 1: 12_24   ->  0: 24 hour mode
     *                    1: 12 hour mode
     * Bit 0: CAP_SEL ->  0: 7pf
     *                    1: 12.5pf
    */
    void setControl1(uint8_t control1);

    /**
     * Set the control register 2
     * Bit 7: AIE   ->  0: alarm interrupt disabled
     *                  1: alarm interrupt enabled
     * Bit 6: AF    ->  alarm flag
     * Bit 5: MI    ->  0: minute interrupt disabled
     *                  1: minute interrupt enabled
     * Bit 4: HMI   ->  0: hour interrupt disabled
     *                  1: hour interrupt enabled
     * Bit 3: TF    ->  timer flag
     * Bit 2 to 0:  ->  CLKOUT frequency
    */
    void setControl2(uint8_t control2);

    /**
     * Set the ram register value
    */
    void setRam(uint8_t value);

    /**
     * Enable the alarm
     * LSB for seconds, ..., MSB for weekdays
    */
    void enableAlarm(uint8_t enable);

    void getTime(uint8_t &seconds, uint8_t &minutes, uint8_t &hours, uint8_t &days, PCF85063A_WEEKDAYS_t &weekdays, uint8_t &months, uint8_t &years);
    void getRam(uint8_t &value);
    void getControl1(uint8_t &value);
    void getControl2(uint8_t &value);
    void getOffset(byte &mode, int8_t &value);
    void getTimerMode(uint8_t &value);
    void getTimerValue(uint8_t &value);
    void getOS(byte &value);

    PCF85063A(PCF85063A const &) = delete;
    void operator=(PCF85063A const &) = delete;

private:
    PCF85063A(){};
    uint8_t decToBcd(uint8_t val);
    uint8_t bcdToDec(uint8_t val);
};