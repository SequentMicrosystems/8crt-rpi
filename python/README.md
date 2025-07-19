# Welcome to SM8crt’s documentation!

# Install

```bash
sudo pip install SM8crt
```

or

```bash
sudo pip3 install SM8crt
```

# Update

```bash
sudo pip install SM8crt -U
```

or

```bash
sudo pip3 install SM8crt -U
```

# Initiate class

```console
$ python
Python 3.11.8 (main, Feb 12 2024, 14:50:05) [GCC 13.2.1 20230801] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import SM8crt
>>> sm = SM8crt.SM8crt()
>>> sm.set_led(2, 1) # Set LED 2 to ON
>>>
```

# Documentation

<a id="module-SM8crt"></a>

### *class* SM8crt.SM8crt(stack=0, i2c=1)

Bases: `object`

Python class to control the 8crt Card for Raspberry Pi.

* **Parameters:**
  * **stack** (*int*) – Stack level/device number.
  * **i2c** (*int*) – i2c bus number

#### cal_crt(channel, value)

Calibrate current readings.
Calibration must be done in 2 points. The further the points are
apart, the more accurate the calibration will be.

* **Parameters:**
  * **channel** (*int*) – Channel number
  * **value** (*int*) – Current(A) value

#### calib_status()

Get current calibration status of device.

* **Returns:**
  (int) Calib status

#### get_all_leds()

Get all leds state as bitmask.

* **Returns:**
  (int) Leds state bitmask

#### get_button()

Get button status.

* **Returns:**
  (bool) status
  : True(ON)/False(OFF)

#### get_button_latch()

Get button latch status.

* **Returns:**
  (bool) status
  : True(ON)/False(OFF)

#### get_crt(channel)

Read the instantenous current(A) of one input channel.

* **Parameters:**
  **channel** (*int*) – Channel number [1..8]
* **Returns:**
  (float) Current value in amperes(A)

#### get_crt_range(channel)

Get the full scale range for a current sensor.

* **Parameters:**
  **channel** (*int*) – Channel number [1..8]
* **Returns:**
  (int) Full scale range in amperes(A)

#### get_crt_rms(channel)

Read the RMS current(A) of one input channel.

* **Parameters:**
  **channel** (*int*) – Channel number [1..8]
* **Returns:**
  (float) RMS current value in amperes(A)

#### get_crt_sensor_type(channel)

Get the type of current sensor.

Type 0 = 2.5V +/- 0.625V
Type 1 = 2.5V +/- 1V

* **Parameters:**
  **channel** (*int*) – Channel number [1..8]
* **Returns:**
  (int) 0 or 1 indicating sensor type

#### get_led(led)

Get led state.

* **Parameters:**
  **led** (*int*) – Led number
* **Returns:**
  0(OFF) or 1(ON)

#### get_rtc()

Get rtc time.

* **Returns:**
  (tuple) date(year, month, day, hour, minute, second)

#### get_version()

Get firmware version.

Returns: (int) Firmware version number

#### set_all_leds(val)

Set all leds states as bitmask.

* **Parameters:**
  **val** (*int*) – Led bitmask

#### set_crt_range(channel, value)

Set the full scale range for a current sensor.

* **Parameters:**
  * **channel** (*int*) – Channel number [1..8]
  * **value** (*int*) – Full scale range in amperes(A) [1..300]

#### set_crt_sensor_type(channel, sensor_type)

Set the type of current sensor.

Type 0 = 2.5V +/- 0.625V
Type 1 = 2.5V +/- 1V

* **Parameters:**
  * **channel** (*int*) – Channel number [1..8]
  * **sensor_type** (*int*) – 0 or 1 indicating sensor type

#### set_led(led, val)

Set led state.

* **Parameters:**
  * **led** (*int*) – Led number
  * **val** – 0(OFF) or 1(ON)

#### set_rtc(year, month, day, hour, minute, second)

Set rtc time.

* **Parameters:**
  * **year** (*int*) – current year
  * **month** (*int*) – current month
  * **day** (*int*) – current day
  * **hour** (*int*) – current hour
  * **minute** (*int*) – current minute
  * **second** (*int*) – current second

#### wdt_clear_reset_count()

Clear watchdog counter.

#### wdt_get_init_period()

Get watchdog initial period.

* **Returns:**
  (int) Initial watchdog period in seconds

#### wdt_get_off_period()

Get watchdog off period in seconds.

* **Returns:**
  (int) Watchdog off period in seconds.

#### wdt_get_period()

Get watchdog period in seconds.

* **Returns:**
  (int) Watchdog period in seconds

#### wdt_get_reset_count()

Get watchdog reset count.

* **Returns:**
  (int) Watchdog reset count

#### wdt_reload()

Reload watchdog.

#### wdt_set_init_period(period)

Set watchdog initial period.

* **Parameters:**
  **period** (*int*) – Initial period in second

#### wdt_set_off_period(period)

Set off period in seconds

* **Parameters:**
  **period** (*int*) – Off period in seconds

#### wdt_set_period(period)

Set watchdog period.

* **Parameters:**
  **period** (*int*) – Watchdog period in seconds

<!-- vi:se ts=4 sw=4 et: -->
