#!/usr/bin/python3

from smbus2 import SMBus
import struct
import datetime

import SM8crt.data as data
I2C_MEM = data.I2C_MEM
CHANNEL_NO = data.CHANNEL_NO
CALIB = data.CALIB

class SM8crt: 
    """Python class to control the 8crt Card for Raspberry Pi.

    Args:
        stack (int): Stack level/device number.
        i2c (int): i2c bus number
    """
    def __init__(self, stack=0, i2c=1):
        if stack < 0 or stack > data.STACK_LEVEL_MAX:
            raise ValueError("Invalid stack level!")
        self._hw_address_ = data.SLAVE_OWN_ADDRESS_BASE + stack
        self._i2c_bus_no = i2c
        self.bus = SMBus(self._i2c_bus_no)
        try:
            self._card_rev_major = self.bus.read_byte_data(self._hw_address_, I2C_MEM.REVISION_HW_MAJOR_ADD)
            self._card_rev_minor = self.bus.read_byte_data(self._hw_address_, I2C_MEM.REVISION_HW_MINOR_ADD)
        except Exception:
            print("{} not detected!".format(data.CARD_NAME))
            raise

    def _get_byte(self, address):
        return self.bus.read_byte_data(self._hw_address_, address)
    def _get_word(self, address):
        return self.bus.read_word_data(self._hw_address_, address)
    def _get_i16(self, address):
        buf = self.bus.read_i2c_block_data(self._hw_address_, address, 2)
        i16_value = struct.unpack("h", bytearray(buf))[0]
        return i16_value
    def _get_float(self, address):
        buf = self.bus.read_i2c_block_data(self._hw_address_, address, 4)
        float_value = struct.unpack("f", bytearray(buf))[0]
        return float_value
    def _get_i32(self, address):
        buf = self.bus.read_i2c_block_data(self._hw_address_, address, 4)
        i32_value = struct.unpack("i", bytearray(buf))[0]
        return i32_value
    def _get_u32(self, address):
        buf = self.bus.read_i2c_block_data(self._hw_address_, address, 4)
        u32_value = struct.unpack("I", bytearray(buf))[0]
        return u32_value
    def _get_block_data(self, address, byteno=4):
        return self.bus.read_i2c_block_data(self._hw_address_, address, byteno)
    def _set_byte(self, address, value):
        self.bus.write_byte_data(self._hw_address_, address, int(value))
    def _set_word(self, address, value):
        self.bus.write_word_data(self._hw_address_, address, int(value))
    def _set_float(self, address, value):
        ba = bytearray(struct.pack("f", value))
        self.bus.write_block_data(self._hw_address_, address, ba)
    def _set_i32(self, address, value):
        ba = bytearray(struct.pack("i", value))
        self.bus.write_block_data(self._hw_address_, address, ba)
    def _set_block(self, address, ba):
        self.bus.write_i2c_block_data(self._hw_address_, address, ba)

    @staticmethod
    def _check_channel(channel_type, channel):
        if not (0 < channel and channel <= CHANNEL_NO[channel_type]):
            raise ValueError("Invalid {} channel number. Must be [1..{}]!".format(channel_type, CHANNEL_NO[channel_type]))
    def _calib_set(self, channel, value):
        ba = bytearray(struct.pack("f", value))
        ba.extend([channel, data.CALIBRATION_KEY])
        self._set_block(I2C_MEM.CALIB_VALUE, ba)

    def _calib_reset(self, channel):
        ba = bytearray([channel, data.CALIBRATION_KEY])
        self._set_block(I2C_MEM.CALIB_CHANNEL, ba)

    def calib_status(self):
        """Get current calibration status of device.

        Returns:
            (int) Calib status
        """
        status = self._get_byte(I2C_MEM.CALIB_STATUS)
        return status

    def get_version(self):
        """Get firmware version.

        Returns: (int) Firmware version number
        """
        version_major = self._get_byte(I2C_MEM.REVISION_MAJOR_ADD)
        version_minor = self._get_byte(I2C_MEM.REVISION_MINOR_ADD)
        version = str(version_major) + "." + str(version_minor)
        return version

    ##################
    # Current inputs #
    ##################
    def get_crt(self, channel):
        """Read the instantenous current(A) of one input channel.

        Args:
            channel (int): Channel number [1..8]

        Returns:
            (float) Current value in amperes(A)
        """
        self._check_channel("crt_in", channel)
        value = self._get_i16(I2C_MEM.CRT_IN_VAL + (channel - 1) * 2)
        return float(value) / data.CRT_SCALE

    def get_crt_rms(self, channel):
        """Read the RMS current(A) of one input channel.

        Args:
            channel (int): Channel number [1..8]

        Returns:
            (float) RMS current value in amperes(A)
        """
        self._check_channel("crt_in", channel)
        value = self._get_i16(I2C_MEM.CRT_IN_RMS + (channel - 1) * 2)
        return float(value) / data.CRT_SCALE

    def cal_crt(self, channel, value):
        """Calibrate current readings.
        Calibration must be done in 2 points. The further the points are
        apart, the more accurate the calibration will be.

        Args:
            channel (int): Channel number
            value (int): Current(A) value
        """
        self._check_channel("crt_in", channel)
        self._calib_set(CALIB + channel, value)

    def get_crt_range(self, channel):
        """Get the full scale range for a current sensor.

        Args:
            channel (int): Channel number [1..8]

        Returns:
            (int) Full scale range in amperes(A)
        """
        self._check_channel("crt_in", channel)
        value = self._get_word(I2C_MEM.CRT_SNS_RANGE + (channel - 1) * 2)
        return value

    def set_crt_range(self, channel, value):
        """Set the full scale range for a current sensor.

        Args:
            channel (int): Channel number [1..8]
            value (int): Full scale range in amperes(A) [1..300]
        """
        self._check_channel("crt_in", channel)
        if not (1 <= value <= 300):
            raise ValueError("Invalid range value, must be 1..300")
        self._set_word(I2C_MEM.CRT_SNS_RANGE + (channel - 1) * 2, value)

    def get_crt_sensor_type(self, channel):
        """Get the type of current sensor.
        
        Type 0 = 2.5V +/- 0.625V
        Type 1 = 2.5V +/- 1V

        Args:
            channel (int): Channel number [1..8]

        Returns:
            (int) 0 or 1 indicating sensor type
        """
        self._check_channel("crt_in", channel)
        value = self._get_byte(I2C_MEM.CRT_SNS_TYPE)
        return 1 if (value & (1 << (channel - 1))) else 0

    def set_crt_sensor_type(self, channel, sensor_type):
        """Set the type of current sensor.
        
        Type 0 = 2.5V +/- 0.625V
        Type 1 = 2.5V +/- 1V

        Args:
            channel (int): Channel number [1..8]
            sensor_type (int): 0 or 1 indicating sensor type
        """
        self._check_channel("crt_in", channel)
        if sensor_type not in [0, 1]:
            raise ValueError("Sensor type must be 0 or 1")
        
        current_mask = self._get_byte(I2C_MEM.CRT_SNS_TYPE)
        if sensor_type == 1:
            new_mask = current_mask | (1 << (channel - 1))
        else:
            new_mask = current_mask & ~(1 << (channel - 1))
        self._set_byte(I2C_MEM.CRT_SNS_TYPE, new_mask)

    ########
    # Leds #
    ########
    def get_led(self, led):
        """Get led state.

        Args:
            led (int): Led number

        Returns:
            0(OFF) or 1(ON)
        """
        self._check_channel("led", led)
        val = self._get_byte(I2C_MEM.LEDS)
        if (val & (1 << (led - 1))) != 0:
            return 1
        return 0

    def get_all_leds(self):
        """Get all leds state as bitmask.

        Returns:
            (int) Leds state bitmask
        """
        return self._get_byte(I2C_MEM.LEDS)

    def set_led(self, led, val):
        """Set led state.

        Args:
            led (int): Led number
            val: 0(OFF) or 1(ON)
        """
        self._check_channel("led", led)
        if val != 0:
            self._set_byte(I2C_MEM.LED_SET, led)
        else:
            self._set_byte(I2C_MEM.LED_CLR, led)

    def set_all_leds(self, val):
        """Set all leds states as bitmask.

        Args:
            val (int): Led bitmask
        """
        if(not (0 <= val and val <= (1 << CHANNEL_NO["led"]) - 1)):
            raise ValueError("Invalid led mask!")
        self._set_byte(I2C_MEM.LEDS, val)

    ############
    # Watchdog #
    ############
    def wdt_reload(self):
        """Reload watchdog."""
        self._set_byte(I2C_MEM.WDT_RESET_ADD, data.WDT_RESET_SIGNATURE)

    def wdt_get_period(self):
        """Get watchdog period in seconds.

        Returns:
            (int) Watchdog period in seconds
        """
        return self._get_word(I2C_MEM.WDT_INTERVAL_GET_ADD)

    def wdt_set_period(self, period):
        """Set watchdog period.

        Args:
            period (int): Watchdog period in seconds
        """
        return self._set_word(I2C_MEM.WDT_INTERVAL_SET_ADD, period)

    def wdt_get_init_period(self):
        """Get watchdog initial period.

        Returns:
            (int) Initial watchdog period in seconds
        """
        return self._get_word(I2C_MEM.WDT_INIT_INTERVAL_GET_ADD)

    def wdt_set_init_period(self, period):
        """Set watchdog initial period.

        Args:
            period (int): Initial period in second
        """
        return self._set_word(I2C_MEM.WDT_INIT_INTERVAL_SET_ADD, period)

    def wdt_get_off_period(self):
        """Get watchdog off period in seconds.

        Returns:
            (int) Watchdog off period in seconds.
        """
        return self._get_i32(I2C_MEM.WDT_POWER_OFF_INTERVAL_GET_ADD)

    def wdt_set_off_period(self, period):
        """Set off period in seconds

        Args:
            period (int): Off period in seconds
        """
        return self._set_i32(I2C_MEM.WDT_POWER_OFF_INTERVAL_SET_ADD, period)

    def wdt_get_reset_count(self):
        """Get watchdog reset count.

        Returns:
            (int) Watchdog reset count
        """
        return self._get_word(I2C_MEM.WDT_RESET_COUNT_ADD)

    def wdt_clear_reset_count(self):
        """Clear watchdog counter. """
        return self._set_i32(I2C_MEM.WDT_CLEAR_RESET_COUNT_ADD, data.WDT_RESET_COUNT_SIGNATURE)

    #######
    # RTC #
    #######
    def get_rtc(self):
        """Get rtc time.

        Returns:
            (tuple) date(year, month, day, hour, minute, second)
        """
        buf = self._get_block_data(I2C_MEM.RTC_YEAR_ADD, 6)
        buf[0] += 2000
        return tuple(buf)

    def set_rtc(self, year, month, day, hour, minute, second):
        """Set rtc time.

        Args:
            year (int): current year
            month (int): current month
            day (int): current day
            hour (int): current hour
            minute (int): current minute
            second (int): current second
        """
        if year > 2000:
            year -= 2000
        if(not(0 <= year and year <= 255)):
            raise ValueError("Invalid year!")
        datetime.datetime(
                year=2000+year, month=month, day=day,
                hour=hour, minute=minute, second=second)
        ba = bytearray(struct.pack(
            "6B B",
            year, month, day, hour, minute, second,
            data.CALIBRATION_KEY))
        self._set_block(I2C_MEM.RTC_SET_YEAR_ADD, ba)

    ##########
    # Button #
    ##########
    def get_button(self):
        """Get button status.

        Returns:
            (bool) status
                True(ON)/False(OFF)
        """
        state = self._get_byte(I2C_MEM.BUTTON)
        if(state & 1):
            return True
        else:
            return False

    def get_button_latch(self):
        """Get button latch status.

        Returns:
            (bool) status
                True(ON)/False(OFF)
        """
        state = self._get_byte(I2C_MEM.BUTTON)
        if(state & 2):
            state &= ~2
            self._set_byte(I2C_MEM.BUTTON, state)
            return True
        else:
            return False