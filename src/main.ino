#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "SPI.h"
#include "TFT_eSPI.h"

#define sea_level_pressure 1013.25
#define WAIT 500
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
Adafruit_BMP280 bmp;
TFT_eSPI tft = TFT_eSPI();

void Clearscreen(){
  tft.setTextSize(2);
  tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLACK);
  tft.setCursor(0, 35);

  tft.setTextColor(TFT_RED);
  tft.println("BNO055 + BMP280 BFF");
  tft.setTextColor(TFT_GREEN);
}

void setup(void)
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GOLD);

  Serial.println("BNO055 + BMP280 BFF Demo");
  tft.println("BNO055 + BMP280 BFF Demo");
  
  tft.setCursor(0, round(tft.height())/2);
  tft.setTextColor(TFT_RED);

  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    tft.println("BNO055 is undetected");
    while (1);
  }

  if (!bmp.begin()) {
    Serial.print("Ooops, no BMP280 detected ... Check your wiring or I2C ADDR!");
    tft.println("BMP280 is undetected");
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  Serial.println("Found BNO055 and BMP280 sensors!\n");
  delay(1500);
}

void loop(void)
{

  Serial.println("---------\nBMP280 data:");
  Serial.printf("Temperature = %.2f C | Pressure = %.2f Pa |  Approx altitude = %.2f m\n", bmp.readTemperature(), bmp.readPressure(), bmp.readAltitude(sea_level_pressure));
  Clearscreen();
  
  tft.setTextColor(TFT_YELLOW);
  tft.println("\nBMP280 data:");
  tft.setTextColor(TFT_GREEN);
  tft.printf("Temp: %.2f C\nPres: %.2f Pa\nApproxAlt: %.2f m\n", bmp.readTemperature(), bmp.readPressure(), bmp.readAltitude(sea_level_pressure));

  sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  Serial.println("\nBNO055 data:");
  tft.setTextColor(TFT_YELLOW);
  tft.println("\nBNO055 data:");
  tft.setTextColor(TFT_GREEN);
  printEvent(&orientationData);
  printEvent(&angVelocityData);
  printEvent(&linearAccelData);
  printEvent(&magnetometerData);
  printEvent(&accelerometerData);
  printEvent(&gravityData);

  delay(WAIT);

}


void printEvent(sensors_event_t* event) {
  float x = 0, y = 0, z = 0;
  const char* label = "Unk";

  switch (event->type) {
    case SENSOR_TYPE_ACCELEROMETER: label = "Accl"; x = event->acceleration.x; y = event->acceleration.y; z = event->acceleration.z; break;
    case SENSOR_TYPE_ORIENTATION:   label = "Orient"; x = event->orientation.x; y = event->orientation.y; z = event->orientation.z; break;
    case SENSOR_TYPE_MAGNETIC_FIELD:label = "Mag"; x = event->magnetic.x; y = event->magnetic.y; z = event->magnetic.z; break;
    case SENSOR_TYPE_GYROSCOPE:     label = "Gyro"; x = event->gyro.x; y = event->gyro.y; z = event->gyro.z; break;
    case SENSOR_TYPE_ROTATION_VECTOR: label = "Rot"; x = event->gyro.x; y = event->gyro.y; z = event->gyro.z; break;
    case SENSOR_TYPE_LINEAR_ACCELERATION: label = "Linear"; x = event->acceleration.x; y = event->acceleration.y; z = event->acceleration.z; break;
    case SENSOR_TYPE_GRAVITY:       label = "Gravity"; x = event->acceleration.x; y = event->acceleration.y; z = event->acceleration.z; break;
  }
  tft.setTextSize(1);
  Serial.printf("%-8s: x= %6.2f | y= %6.2f | z= %6.2f\n", label, x, y, z);
  // tft.printf("%s: x= %.2f | y= %.2f | z= %.2f\n", label, x, y, z);
  //
  tft.setTextColor(TFT_GREEN);
  tft.printf("%s: x=", label);

  tft.setTextColor(TFT_BLUE);
  tft.printf("%.2f ", x);

  tft.setTextColor(TFT_GREEN);
  tft.print("| y=");

  // y=... (blue)
  tft.setTextColor(TFT_BLUE);
  tft.printf("%.2f ", y);

  // Separator | (green)
  tft.setTextColor(TFT_GREEN);
  tft.print("| z=");

  // z=... (blue)
  tft.setTextColor(TFT_BLUE);
  tft.printf("%.2f\n", z);

}

