#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Wire.h>

#include <Adafruit_BMP280.h>
#include <SFE_BMP180.h>
#include <MPU6050_tockn.h>

#include <QMC5883L.h>



QMC5883L compass;

MPU6050 mpu6050(Wire);

Adafruit_BMP280 bmp;

SFE_BMP180 pressure;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
float data[7];
void setup() {

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  
  Serial.println(F("BMP280 test"));

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  compass.init();
  compass.setSamplingRate(50);

  
  Serial.println("QMC5883L Compass Demo");
  Serial.println("Turn compass in all directions to calibrate....");

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  
}

void loop() {
    mpu6050.update();
    
    
    Serial.println("=======================================================");
  
    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
    Serial.println(" ");
    
    Serial.println("=======================================================\n");


  
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure =    "));
    Serial.print((bmp.readPressure())+540);
    Serial.println(" Pa");

    Serial.print(("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); // Adjusted to local forecast! 
    Serial.println(" m");

      int heading = compass.readHeading();
    if(heading==0) {
    /* Still calibrating, so measure but don't print */
  } else {
    Serial.print("Heading Indicator:");
    Serial.print(heading);
    Serial.println("");
    
  }  

   char status;
  double T,P,p0,a;

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("Total pressure: ");
          Serial.print(P*100,2);
          Serial.print(" pa ");
          Serial.println();

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

         

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
   
    
    data[0] =((bmp.readPressure())+540);
    data[1] =(bmp.readTemperature());
    data[2] =(bmp.readAltitude(1013.25));
    data[3] =(mpu6050.getAccAngleX());
    data[4] =(mpu6050.getAccAngleY());
    data[5] =(heading);
    data[6] =(P*100);

    radio.write(data, sizeof(data));
    
}
