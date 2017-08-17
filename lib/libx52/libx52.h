/*
 * Saitek X52 Pro MFD & LED driver
 *
 * Copyright (C) 2012-2015 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2.
 *
 */

#ifndef LIBX52_H
#define LIBX52_H

#include <time.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque structure used by libx52
 */
struct libx52_device;

/**
 * @brief Opaque structure used by libx52
 */
typedef struct libx52_device libx52_device;

/**
 * @brief List of supported clocks on the MFD
 */
typedef enum {
    /** Primary clock on the MFD, indicated by the number 1 */
    LIBX52_CLOCK_1,

    /** Secondary clock on the MFD, indicated by the number 2 */
    LIBX52_CLOCK_2,

    /** Tertiary clock on the MFD, indicated by the number 3 */
    LIBX52_CLOCK_3,
} libx52_clock_id;

/**
 * @brief Supported clock formats
 */
typedef enum {
    /** 12-hour display on the MFD clock */
    LIBX52_CLOCK_FORMAT_12HR,

    /** 24-hour display on the MFD clock */
    LIBX52_CLOCK_FORMAT_24HR,
} libx52_clock_format;

/**
 * @brief Supported date formats
 */
typedef enum {
    /** Date format DD-MM-YY */
    LIBX52_DATE_FORMAT_DDMMYY,

    /** Date format MM-DD-YY */
    LIBX52_DATE_FORMAT_MMDDYY,

    /** Date format YY-MM-DD */
    LIBX52_DATE_FORMAT_YYMMDD,
} libx52_date_format;

/**
 * @brief Supported LED identifiers
 */
typedef enum {
    /** Fire indicator LED */
    LIBX52_LED_FIRE = 0x01,

    /** LED on button A */
    LIBX52_LED_A = 0x02,

    /** LED on button B */
    LIBX52_LED_B = 0x04,

    /** LED on button D */
    LIBX52_LED_D = 0x06,

    /** LED on button E */
    LIBX52_LED_E = 0x08,

    /** LED on toggle T1 */
    LIBX52_LED_T1 = 0x0a,

    /** LED on toggle T2 */
    LIBX52_LED_T2 = 0x0c,

    /** LED on toggle T3 */
    LIBX52_LED_T3 = 0x0e,

    /** LED on POV Hat */
    LIBX52_LED_POV = 0x10,

    /** LED on clutch button */
    LIBX52_LED_CLUTCH = 0x12,

    /** Throttle LED */
    LIBX52_LED_THROTTLE = 0x14,
} libx52_led_id;

/**
 * @brief Supported LED states
 *
 * Not all LEDs support all states
 */
typedef enum {
    /** LED is off */
    LIBX52_LED_STATE_OFF,

    /**
     * LED is on. This is not supported by those LEDs which can display
     * multiple colors
     */
    LIBX52_LED_STATE_ON,

    /** LED displays red color. This is not supported by single color LEDs */
    LIBX52_LED_STATE_RED,

    /** LED displays amber color. This is not supported by single color LEDs */
    LIBX52_LED_STATE_AMBER,

    /** LED displays green color. This is not supported by single color LEDs */
    LIBX52_LED_STATE_GREEN,
} libx52_led_state;

/**
 * @brief Initialize the X52 library
 *
 * This function initializes the libx52 library, sets up any internal data
 * structures to access the joystick, and returns a \ref libx52_device pointer.
 * All calls to libx52 use the returned pointer to control the device.
 *
 * If no joystick is found `libx52_init()` returns _NULL_.
 *
 * @par Limitations
 * This function does not support hotplugging. The joystick must be plugged in
 * before calling this function.
 *
 * @returns a pointer to the detected \ref libx52_device
 */
libx52_device * libx52_init(void);

/**
 * @brief Exit the library and free up any resources used
 *
 * This function releases any resources allocated by \ref libx52_init and
 * terminates the library. Using the freed device now is invalid and can
 * cause errors.
 *
 * @param[in]   dev     Pointer to the device
 * @returns None
 */
void libx52_exit(libx52_device *dev);

/**
 * @brief Set the text on an MFD line
 *
 * The Multifunction display (MFD) supports 3 lines of up to 16 characters.
 * This function will set the text into the internal data structures.
 *
 * \p text must be a pointer to an array of bytes, each byte represents a code
 * point on the MFD's internal character map. \p length is the length of this
 * array.
 *
 * \p line must be \b 0, \b 1, or \b 2 and refers to the first, second or third
 * line of the MFD respectively.
 *
 * @par Limitations
 * This function can only store a maximum of 16 characters per line. Any
 * additional characters are silently discarded.
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   line    Line to be updated (0, 1 or 2)
 * @param[in]   text    Pointer to the text string. The text must be mapped to
 *                      the code page of the X52 display.
 * @param[in]   length  Length of the text to display
 *
 * @returns 0 on success, \c -EINVAL if either \p x52 is invalid, or \p line is
 * outside the accepted range.
 */
int libx52_set_text(libx52_device *x52, uint8_t line, const char *text, uint8_t length);

/**
 * @brief Set the LED state
 *
 * The X52 pro has several LEDs that can be individually controlled. This
 * function allows you to set the state of each LED in the internal data
 * structures.
 *
 * Note that the LEDs referred to by \ref LIBX52_LED_FIRE and \ref
 * LIBX52_LED_THROTTLE support only the states \ref LIBX52_LED_STATE_OFF and
 * \ref LIBX52_LED_STATE_ON. The remaining LEDs support all the states with
 * the exception of \ref LIBX52_LED_STATE_ON.
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   led     LED identifier (refer \ref libx52_led_id)
 * @param[in]   state   State of the LED (refer \ref libx52_led_state)
 *
 * @returns 0 on success, \c -EINVAL if the \p x52 parameter is not valid, and
 * \c -ENOTSUP if the \p led and \p state combination is not a supported one.
 * The API also returns \c -ENOTSUP if the probed joystick is not an X52 Pro,
 * but the non-Pro X52 variant.
 */
int libx52_set_led_state(libx52_device *x52,
                         libx52_led_id led,
                         libx52_led_state state);

/**
 * @brief Set the clock
 *
 * This function sets the primary clock's date and time with the specified Unix
 * time value.  \p time can be obtained from \c time(2). \p local controls
 * whether the primary clock displays local time or GMT.
 *
 * If this function is called again within the same minute as calculated by
 * \c localtime(3) or \c gmtime(3), it will return \c -EAGAIN, as it does not
 * require any updates to be written to the joystick. However, if the call
 * changes the timezone from local time to GMT or vice-versa, then the function
 * will return 0, since it requires a write to the device to update the clock
 * with the new timezone.
 *
 * The secondary and tertiary clocks are driven off the primary clock and set
 * using \ref libx52_set_clock_timezone.
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   time    Time value from \c time(3)
 * @param[in]   local   0 for GM time, non-zero for localtime
 *
 * @returns 0 on success, \c -EAGAIN if no change from previous time,
 *          \c -EINVAL if \p x52 is not valid.
 */
int libx52_set_clock(libx52_device *x52, time_t time, int local);

/**
 * @brief Set the timezone for the secondary and tertiary clocks.
 *
 * The X52 Pro has a total of 3 clock displays. The secondary and tertiary
 * clocks are controlled as an offset from the primary clock in minutes.
 * However, for convenience, the X52 library calculates this offset internally
 * and only requires you to set the timezone as the number of minutes offset
 * from GMT. \p offset is limited to +/- 1440 minutes, and any offset outside
 * this range will result in a return value of \c -EDOM
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   clock   \ref libx52_clock_id, cannot be \ref
 *                      LIBX52_CLOCK_1
 * @param[in]   offset  Offset in minutes from GMT (east is positive, west
 *                      is negative)
 *
 * @returns 0 on success, \c -EINVAL if \p x52 is invalid, \c -ENOTSUP if
 * \p clock is \ref LIBX52_CLOCK_1, \c -EDOM if \p offset is more than +/-
 * 24 hours.
 */
int libx52_set_clock_timezone(libx52_device *x52,
                              libx52_clock_id clock,
                              int offset);

/**
 * @brief Set whether the clock is displayed in 12 hour or 24 hour format.
 *
 * The clocks on the X52 Pro MFD are all displayed as HH:MM, but can be set to
 * display it in 12-hour (AM/PM) or 24-hour format. The default format if not
 * specified is 12-hour.
 *
 * @par Limitations
 * The hardware has a limitation that it cannot display 12:00 am in 12 hour
 * mode - instead it will display as 00:00 am
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   clock   \ref libx52_clock_id
 * @param[in]   format  \ref libx52_clock_format
 *
 * @returns 0 on success, \c -EINVAL if \p x52 is not valid, or if either of
 * \p clock or \p format are outside their respective ranges.
 */
int libx52_set_clock_format(libx52_device *x52,
                            libx52_clock_id clock,
                            libx52_clock_format format);

/**
 * @brief Set the date format for the MFD date display
 *
 * If not set, the date format defaults to DD-MM-YY
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   format  \ref libx52_date_format
 *
 * @returns 0 on success, \c -EINVAL if \p x52 is not valid
 */
int libx52_set_date_format(libx52_device *x52, libx52_date_format format);

/**
 * @brief Set the MFD or LED brightness
 *
 * The brightness of the MFD display and the button LEDs can be controlled
 * individually. \p brightness should be a value between 0 and 128. While
 * the library does not fail on values higher than 128, the effect may not
 * be what is intended.
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   mfd     0 for LED brightness, 1 for MFD brightness
 * @param[in]   brightness  Brightness level to set
 *
 * @returns 0 on success, \c -EINVAL if \p x52 is not valid.
 */
int libx52_set_brightness(libx52_device *x52, uint8_t mfd, uint16_t brightness);

/**
 * @brief Set the state of the shift indicator
 *
 * The X52 Pro MFD has a single shift indicator that indicates when the
 * "shift" button is pressed.
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   state   0 for off, 1 for on
 *
 * @returns 0 on success, \c -EINVAL if \p x52 is not valid
 */
int libx52_set_shift(libx52_device *x52, uint8_t state);

/**
 * @brief Set the blinking state
 *
 * The X52 Pro has a "blink" state where it blinks the clutch and hat LEDs.
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   state   0 for off, 1 for on
 *
 * @returns 0 on success, \c -EINVAL if \p x52 is not valid
 */
int libx52_set_blink(libx52_device *x52, uint8_t state);

/**
 * @brief Update the X52
 *
 * All the libx52_set functions only set the internal data structures, but do
 * not actually write anything to the joystick. This function writes the saved
 * data to the joystick and updates the internal data structures as necessary.
 *
 * @param[in]   x52     Pointer to the device
 *
 * @returns 0 on success, non-zero error number on failure
 */
int libx52_update(libx52_device *x52);

/**
 * @brief Write a raw vendor control packet
 *
 * This function sends the control packet immediately to the hardware, without
 * having to wait for a call to \ref libx52_update.
 *
 * This can be used to debug issues seen on the hardware, however, it is \b NOT
 * recommended for use by end users, as it can potentially damage the hardware.
 *
 * @param[in]   x52     Pointer to the device
 * @param[in]   index   wIndex in the USB packet
 * @param[in]   value   wValue in the USB packet
 *
 * @returns 0 on success, non-zero error number on failure
 */
int libx52_vendor_command(libx52_device *x52, uint16_t index, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif /* !defined LIBX52_H */
