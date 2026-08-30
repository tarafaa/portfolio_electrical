#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdlib.h>

/*
 * Robotic Hand Glove Controller
 * Target: Arduino Uno (ATmega328P)
 *
 * Flex Sensors:
 * A0 / ADC0 - Thumb
 * A1 / ADC1 - Index
 * A2 / ADC2 - Middle
 * A3 / ADC3 - Ring
 * A4 / ADC4 - Pinky
 *
 * Servos:
 * D2 - Thumb
 * D3 - Index
 * D4 - Pinky
 * D5 - Middle
 * D6 - Ring
 */


/* ---------------- Servo Pins ---------------- */

#define SERVO_THUMB   PD2
#define SERVO_INDEX   PD3
#define SERVO_PINKY   PD4
#define SERVO_MIDDLE  PD5
#define SERVO_RING    PD6

#define SERVO_MASK ((1 << SERVO_THUMB)  | \
                    (1 << SERVO_INDEX)  | \
                    (1 << SERVO_PINKY)  | \
                    (1 << SERVO_MIDDLE) | \
                    (1 << SERVO_RING))


/* ---------------- Servo Limits ---------------- */

#define ANGLE_MIN 0
#define ANGLE_MAX 90

#define SMOOTH_SAMPLES 4
#define DEADBAND_DEG   1


/* ---------------- Flex Sensor Calibration ---------------- */

/*
 * Raw ADC values measured with the fingers
 * fully extended and fully closed.
 */

static const int16_t raw_extended[5] = {
    256,    /* Thumb  */
    256,    /* Index  */
    256,    /* Middle */
    256,    /* Ring   */
    256     /* Pinky  */
};

static const int16_t raw_closed[5] = {
    59,     /* Thumb  */
    59,     /* Index  */
    59,     /* Middle */
    59,     /* Ring   */
    59      /* Pinky  */
};


/* ---------------- Servo State ---------------- */

/*
 * Timer interrupt runs every 20 us.
 *
 * 1000 us pulse = 50 ticks
 * 1500 us pulse = 75 ticks
 * 2000 us pulse = 100 ticks
 */

volatile uint16_t servo_pulse[5] = {
    50, 50, 50, 50, 50
};

volatile uint16_t frame_tick = 0;


/* ---------------- ADC ---------------- */

void adc_init(void)
{
    /*
     * AVcc used as ADC reference.
     */
    ADMUX = (1 << REFS0);

    /*
     * Enable ADC.
     *
     * Prescaler = 128
     * 16 MHz / 128 = 125 kHz ADC clock
     */
    ADCSRA = (1 << ADEN)  |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);

    /*
     * Disable digital input buffers on A0-A4.
     */
    DIDR0 = (1 << ADC0D) |
            (1 << ADC1D) |
            (1 << ADC2D) |
            (1 << ADC3D) |
            (1 << ADC4D);
}


uint16_t adc_read(uint8_t channel)
{
    /* Select ADC0-ADC4 while keeping AVcc reference. */
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    /* Start ADC conversion. */
    ADCSRA |= (1 << ADSC);

    /* Wait for conversion to finish. */
    while (ADCSRA & (1 << ADSC))
    {
    }

    return ADC;
}


uint16_t adc_read_smoothed(uint8_t channel)
{
    uint32_t total = 0;

    for (uint8_t i = 0; i < SMOOTH_SAMPLES; i++)
    {
        total += adc_read(channel);
    }

    return (uint16_t)(total / SMOOTH_SAMPLES);
}


/* ---------------- Sensor Conversion ---------------- */

int16_t raw_to_angle(int16_t raw,
                     int16_t raw_ext,
                     int16_t raw_clo)
{
    int32_t angle;

    angle =
        ((int32_t)(raw - raw_ext) *
        (ANGLE_MAX - ANGLE_MIN)) /
        (raw_clo - raw_ext);

    angle += ANGLE_MIN;

    if (angle < ANGLE_MIN)
    {
        angle = ANGLE_MIN;
    }

    if (angle > ANGLE_MAX)
    {
        angle = ANGLE_MAX;
    }

    return (int16_t)angle;
}


/*
 * Converts an angle from 0-90 degrees to
 * a servo pulse between approximately
 * 1000-2000 us.
 */
uint16_t angle_to_servo_ticks(int16_t angle)
{
    uint32_t pulse_us;

    pulse_us =
        1000UL + ((uint32_t)angle * 1000UL) / ANGLE_MAX;

    return (uint16_t)(pulse_us / 20UL);
}


/* ---------------- Servo Timer ---------------- */

void servo_timer_init(void)
{
    /* Configure D2-D6 as outputs. */
    DDRD |= SERVO_MASK;

    /* Start all servo outputs LOW. */
    PORTD &= ~SERVO_MASK;

    /*
     * Timer1 CTC mode
     *
     * CPU clock   = 16 MHz
     * Prescaler   = 8
     * Timer clock = 2 MHz
     *
     * OCR1A = 39 produces an interrupt
     * every 20 us.
     */
    TCCR1A = 0;

    TCCR1B =
        (1 << WGM12) |
        (1 << CS11);

    OCR1A = 39;

    /* Enable Timer1 compare interrupt. */
    TIMSK1 |= (1 << OCIE1A);
}


/*
 * Software-generated servo PWM.
 *
 * One servo frame:
 * 20 ms = 1000 timer ticks.
 */
ISR(TIMER1_COMPA_vect)
{
    if (frame_tick == 0)
    {
        /* Start a new pulse for all five servos. */
        PORTD |= SERVO_MASK;
    }

    if (frame_tick >= servo_pulse[0])
    {
        PORTD &= ~(1 << SERVO_THUMB);
    }

    if (frame_tick >= servo_pulse[1])
    {
        PORTD &= ~(1 << SERVO_INDEX);
    }

    if (frame_tick >= servo_pulse[2])
    {
        PORTD &= ~(1 << SERVO_MIDDLE);
    }

    if (frame_tick >= servo_pulse[3])
    {
        PORTD &= ~(1 << SERVO_RING);
    }

    if (frame_tick >= servo_pulse[4])
    {
        PORTD &= ~(1 << SERVO_PINKY);
    }

    frame_tick++;

    if (frame_tick >= 1000)
    {
        frame_tick = 0;
    }
}


/* ---------------- Main Program ---------------- */

int main(void)
{
    uint16_t raw[5];
    int16_t angle[5];

    int16_t previous_angle[5] = {
        0, 0, 0, 0, 0
    };

    adc_init();
    servo_timer_init();

    /* Enable global interrupts. */
    sei();

    while (1)
    {
        /*
         * Read each flex sensor and calculate
         * the corresponding finger position.
         */
        for (uint8_t i = 0; i < 5; i++)
        {
            raw[i] = adc_read_smoothed(i);

            angle[i] = raw_to_angle(
                raw[i],
                raw_extended[i],
                raw_closed[i]
            );

            /*
             * Ignore very small angle changes
             * to reduce servo jitter.
             */
            if (abs(angle[i] - previous_angle[i])
                >= DEADBAND_DEG)
            {
                previous_angle[i] = angle[i];

                uint16_t new_pulse =
                    angle_to_servo_ticks(angle[i]);

                /*
                 * ATmega328P is an 8-bit MCU.
                 * Protect the 16-bit value while
                 * it is being updated.
                 */
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                {
                    servo_pulse[i] = new_pulse;
                }
            }
        }

        _delay_ms(15);
    }

    return 0;
}
