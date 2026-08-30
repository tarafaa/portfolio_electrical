#include <SPI.h>
#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>

// ============================================================
// Pin Definitions
// ============================================================

// OLED
#define OLED_CS   9
#define OLED_DC   8
#define OLED_RES  7

// BME680
#define BME_CS   10


// ============================================================
// Hardware Objects
// ============================================================

U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(
    U8G2_R0,
    OLED_CS,
    OLED_DC,
    OLED_RES
);

Adafruit_BME680 bme(BME_CS);


// ============================================================
// Sensor Data Structure
// ============================================================

typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float gasResistance;
    uint16_t readingNumber;

} SensorData;


// ============================================================
// FreeRTOS Objects
// ============================================================

QueueHandle_t sensorQueue;

SemaphoreHandle_t spiMutex;


// ============================================================
// Task Prototypes
// ============================================================

void TaskSensor(void *pvParameters);

void TaskDisplay(void *pvParameters);

void TaskSerial(void *pvParameters);


// ============================================================
// Setup
// ============================================================

void setup()
{
    Serial.begin(9600);

    pinMode(OLED_CS, OUTPUT);
    pinMode(BME_CS, OUTPUT);

    digitalWrite(OLED_CS, HIGH);
    digitalWrite(BME_CS, HIGH);


    // --------------------------------------------------------
    // Initialize OLED
    // --------------------------------------------------------

    u8g2.begin();


    // --------------------------------------------------------
    // Initialize BME680
    // --------------------------------------------------------

    if (!bme.begin())
    {
        Serial.println("BME680 initialization failed.");

        while (1);
    }


    // --------------------------------------------------------
    // Configure BME680
    // --------------------------------------------------------

    bme.setTemperatureOversampling(BME680_OS_2X);

    bme.setHumidityOversampling(BME680_OS_2X);

    bme.setPressureOversampling(BME680_OS_4X);

    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);


    // --------------------------------------------------------
    // Create FreeRTOS Queue
    // --------------------------------------------------------

    sensorQueue = xQueueCreate(
        5,
        sizeof(SensorData)
    );

    if (sensorQueue == NULL)
    {
        Serial.println("Queue creation failed.");

        while (1);
    }


    // --------------------------------------------------------
    // Create SPI Mutex
    // --------------------------------------------------------

    spiMutex = xSemaphoreCreateMutex();

    if (spiMutex == NULL)
    {
        Serial.println("SPI mutex creation failed.");

        while (1);
    }


    // --------------------------------------------------------
    // Create Tasks
    // --------------------------------------------------------

    xTaskCreate(
        TaskSensor,
        "Sensor",
        256,
        NULL,
        2,
        NULL
    );


    xTaskCreate(
        TaskDisplay,
        "Display",
        256,
        NULL,
        1,
        NULL
    );


    xTaskCreate(
        TaskSerial,
        "Serial",
        192,
        NULL,
        1,
        NULL
    );


    Serial.println("FreeRTOS Environmental Monitor Started");
}


// ============================================================
// Arduino Loop
// ============================================================

void loop()
{
    /*
     * Empty.
     *
     * FreeRTOS scheduler manages execution.
     */
}


// ============================================================
// Sensor Task
// ============================================================

void TaskSensor(void *pvParameters)
{
    SensorData data;

    uint16_t readingCounter = 0;

    TickType_t previousWakeTime;

    previousWakeTime = xTaskGetTickCount();


    while (1)
    {
        /*
         * Lock the SPI bus before communicating
         * with the BME680.
         */

        if (xSemaphoreTake(
                spiMutex,
                portMAX_DELAY) == pdTRUE)
        {

            digitalWrite(OLED_CS, HIGH);

            digitalWrite(BME_CS, LOW);


            // ----------------------------------------------
            // Read Sensor
            // ----------------------------------------------

            if (bme.performReading())
            {

                data.temperature =
                    bme.temperature;


                data.humidity =
                    bme.humidity;


                // Pa → hPa

                data.pressure =
                    bme.pressure / 100.0;


                // Ohms → kOhms

                data.gasResistance =
                    bme.gas_resistance / 1000.0;


                data.readingNumber =
                    readingCounter++;


                /*
                 * Send sensor structure into
                 * the FreeRTOS queue.
                 */

                xQueueSend(
                    sensorQueue,
                    &data,
                    0
                );
            }


            digitalWrite(BME_CS, HIGH);


            /*
             * Release SPI bus.
             */

            xSemaphoreGive(spiMutex);
        }


        /*
         * Run this task every 500 ms.
         *
         * vTaskDelayUntil provides more consistent
         * periodic scheduling than delay().
         */

        vTaskDelayUntil(
            &previousWakeTime,
            pdMS_TO_TICKS(500)
        );
    }
}


// ============================================================
// Display Task
// ============================================================

void TaskDisplay(void *pvParameters)
{
    SensorData data;


    while (1)
    {
        /*
         * Wait until a sensor measurement arrives.
         */

        if (xQueueReceive(
                sensorQueue,
                &data,
                portMAX_DELAY) == pdTRUE)
        {

            /*
             * Lock SPI because the OLED and BME680
             * share the SPI bus.
             */

            if (xSemaphoreTake(
                    spiMutex,
                    portMAX_DELAY) == pdTRUE)
            {

                digitalWrite(BME_CS, HIGH);

                digitalWrite(OLED_CS, LOW);


                // ------------------------------------------
                // Draw OLED Screen
                // ------------------------------------------

                u8g2.firstPage();

                do
                {
                    u8g2.setFont(
                        u8g2_font_6x10_tr
                    );


                    // Reading Number

                    u8g2.setCursor(0, 10);

                    u8g2.print("Reading #");

                    u8g2.print(
                        data.readingNumber
                    );


                    // Temperature

                    u8g2.setCursor(0, 22);

                    u8g2.print("Temp: ");

                    u8g2.print(
                        data.temperature,
                        1
                    );

                    u8g2.print(" C");


                    // Humidity

                    u8g2.setCursor(0, 34);

                    u8g2.print("Humidity: ");

                    u8g2.print(
                        data.humidity,
                        1
                    );

                    u8g2.print(" %");


                    // Pressure

                    u8g2.setCursor(0, 46);

                    u8g2.print("Pressure: ");

                    u8g2.print(
                        data.pressure,
                        0
                    );

                    u8g2.print(" hPa");


                    // Gas Resistance

                    u8g2.setCursor(0, 58);

                    u8g2.print("Gas: ");

                    u8g2.print(
                        data.gasResistance,
                        1
                    );

                    u8g2.print(" kOhm");


                } while (u8g2.nextPage());


                digitalWrite(OLED_CS, HIGH);


                /*
                 * Release SPI bus.
                 */

                xSemaphoreGive(spiMutex);
            }
        }
    }
}


// ============================================================
// Serial Monitoring Task
// ============================================================

void TaskSerial(void *pvParameters)
{
    while (1)
    {
        Serial.print("RTOS Tick: ");

        Serial.println(
            xTaskGetTickCount()
        );


        /*
         * Serial status message every 2 seconds.
         */

        vTaskDelay(
            pdMS_TO_TICKS(2000)
        );
    }
}
