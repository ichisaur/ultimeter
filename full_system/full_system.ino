//Datalogging
double lowgX, lowgY, lowgZ;
double altitudeReading, temperatureReading;

//Printing to website
long latitude_mdeg;
long longitude_mdeg;

//Low Power
#include <ArduinoLowPower.h>

// Blink sequence number
// Declare it volatile since it's incremented inside an interrupt
volatile int repetitions = 1;

// Pin used to trigger a wakeup
const int pin = 8;

//LTE chip
#include "Adafruit_FONA.h"
#define FONA_RX 0
#define FONA_TX 1
#define FONA_RST 4
char replybuffer[255];
HardwareSerial *fonaSerial = &Serial1;
Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
uint8_t type;
char content[255];
char packet[255];
char sendcommand[255];

//BNO
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//BMP
#include <SPI.h>
#include "Adafruit_BMP3XX.h"
#define BMP_SCK 17
#define BMP_SDA 16
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp;

//H3LIS
#include "SparkFun_LIS331.h"
LIS331 xl;

//GPS
#include "SparkFun_Ublox_Arduino_Library.h"
SFE_UBLOX_GPS myGPS;
#include <MicroNMEA.h>
char nmeaBuffer[100];
MicroNMEA nmea(nmeaBuffer, sizeof(nmeaBuffer));

//Battery Babysitter
#include <SparkFunBQ27441.h>
const unsigned int BATTERY_CAPACITY = 1200; // 1200 mAh battery

//SD Card
#include <SD.h>
File myFile;
const int chipSelect = 7;


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //Low Power
  pinMode(LED_BUILTIN, OUTPUT);
  // Attach a wakeup interrupt on pin 8, calling repetitionsIncrease when the device is woken up
  LowPower.attachInterruptWakeup(pin, repetitionsIncrease, CHANGE);
  
  //Datalogger
  if (!SD.begin(chipSelect)){
    Serial.println("initialization failed!");
    while (1);
  }


  fonaSerial->begin(4800);
  fona.begin(*fonaSerial);
  
  Serial.println("initialization done.");
  
  myFile = SD.open("data.csv", FILE_WRITE);

  if(myFile){
    Serial.print("Writing to data.csv...");
    myFile.println("Time, X High G Accel, Y High G Accel, Z High G Accel, X Low G Accel, Y Low G Accel, Z Low G Accel, X Orientation, Y Orientation, Z Orientation, Altitude, Temperature, Battery Percentage, GPS Latitude, GPS Longitude");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening data.csv");
  }
  
  Serial.println("Full System Sensor Test"); Serial.println("");
  if (!bno.begin())
    {
      Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }

  if (!bmp.begin((uint8_t)118)) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  
  //H3LIS
  pinMode(9,INPUT);       // Interrupt pin input
  Wire.begin();
  xl.setI2CAddr(0x19);    // This MUST be called BEFORE .begin() so 
                          //  .begin() can communicate with the chip
  xl.begin(LIS331::USE_I2C); // Selects the bus to be used and sets
                          //  the power up bit on the accelerometer.
                          //  Also zeroes out all accelerometer
                          //  registers that are user writable.
  xl.intSrcConfig(LIS331::INT_SRC, 1); // Select the source of the
                          //  signal which appears on pin INT1. In
                          //  this case, we want the corresponding
                          //  interrupt's status to appear. 
  xl.setIntDuration(1, 1); // Number of samples a value must meet
                          //  the interrupt condition before an
                          //  interrupt signal is issued. At the
                          //  default rate of 50Hz, this is one sec.
  xl.setIntThreshold(2, 1); // Threshold for an interrupt. This is
                          //  not actual counts, but rather, actual
                          //  counts divided by 16.
  xl.enableInterrupt(LIS331::Y_AXIS, LIS331::TRIG_ON_HIGH, 1, true);
                          // Enable the interrupt. Parameters indicate
                          //  which axis to sample, when to trigger
                          //  (in this case, when the absolute mag
                          //  of the signal exceeds the threshold),
                          //  which interrupt source we're configuring,
                          //  and whether to enable (true) or disable
                          //  (false) the interrupt.
  
  //BMP
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

  //GPS
  if (myGPS.begin() == false)
  {
    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  //Battery Babysitter
  setupBQ27441();

  //BNO
  delay(1000);
  bno.setExtCrystalUse(true);

  while(fonaSerial->read() >= 0 ) {
    
  }

  fonaSerial->write("AT\r\n");
  echoFona();
  fonaSerial->write("AT+CGDCONT=1, \"IP\", \"NXTGENPHONE\",\"0.0.0.0\",0,0\r\n");
  delay(500);
  echoFona();

  fonaSerial->write("AT+CGSOCKCONT=1,\"IP\",\"NXTGENPHONE\",\"0.0.0.0\",0,0\r\n");
  delay(500);
  echoFona();

  fonaSerial->write("AT+CSOCKSETPN=1\r\n");
  delay(500);
  echoFona();

  fonaSerial->write("AT+CHTTPSSTART\r\n");
  delay(500);
  echoFona();

}

void loop() {
  if (repetitions < 3)
  {
  LowPower.sleep();
  bno.enterSuspendMode();
  }
  bno.enterNormalMode();
  static long loopTimer = 0;
  int16_t x, y, z;
  int currenttime = millis();
  if (millis() - loopTimer > 1000)
  {
    loopTimer = millis();
    xl.readAxes(x, y, z);  // The readAxes() function transfers the
                           //  current axis readings into the three
                           //  parameter variables passed to it.
    Serial.print("X High G Accel: ");
    Serial.println(xl.convertToG(100,x)); // The convertToG() function
    Serial.print("Y High G Accel: ");
    Serial.println(xl.convertToG(100,y));
    Serial.print("Z High G Accel: ");     // accepts as parameters the
    Serial.println(xl.convertToG(100,z)); // raw value and the current maximum g-rating.
  }
  if (digitalRead(8) == HIGH)
  {
    Serial.println("Interrupt");
  }
  // Triggers an infinite sleep (the device will be woken up only by the registered wakeup sources)
  // The power consumption of the chip will drop consistently
  sensors_event_t linearAccelData;
  sensors_event_t event;
  bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  bno.getEvent(&event);
  printEvent(&linearAccelData);
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  if (! bmp.performReading()) {
    Serial.println("Failed to perform BMP reading :(");
    return;
  }
  Serial.print("Approx. Altitude = ");
  altitudeReading = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print(altitudeReading);
  Serial.println(" m");
  Serial.print("Temperature = ");
  temperatureReading = bmp.temperature;
  Serial.print(temperatureReading);
  Serial.println(" *C");
  printBatteryStats();
  myGPS.checkUblox();
  if(nmea.isValid() == true)
  {
    latitude_mdeg = nmea.getLatitude();
    longitude_mdeg = nmea.getLongitude();

    Serial.print("Latitude (deg): ");
    Serial.println(latitude_mdeg / 1000000., 6);
    Serial.print("Longitude (deg): ");
    Serial.println(longitude_mdeg / 1000000., 6);
  }
  else
  {
    Serial.print("No Fix - ");
    Serial.print("Num. satellites: ");
    Serial.println(nmea.getNumSatellites());
  }
  Serial.println("");
  String dataString = "";

  // read all the sensors and append to the string:

  dataString += String(currenttime);
  dataString += ",";
  dataString += String(xl.convertToG(100,x));
  dataString += ",";
  dataString += String(xl.convertToG(100,y));
  dataString += ",";
  dataString += String(xl.convertToG(100,z));
  dataString += ",";
  dataString += String(lowgX);
  dataString += ",";
  dataString += String(lowgY);
  dataString += ",";
  dataString += String(lowgZ);
  dataString += ",";
  dataString += String(event.orientation.x, 4);
  dataString += ",";
  dataString += String(event.orientation.y, 4);
  dataString += ",";
  dataString += String(event.orientation.z, 4);
  dataString += ",";
  dataString += String(altitudeReading);
  dataString += ",";
  dataString += String(temperatureReading);
  dataString += ",";
  dataString += String(lipo.soc());
  dataString += ",";
  if (nmea.isValid() == true)
  {
    long latitude_mdeg = nmea.getLatitude();
    long longitude_mdeg = nmea.getLongitude();

    dataString += String((latitude_mdeg / 1000000., 6));
    dataString += ",";
    dataString += String((longitude_mdeg / 1000000., 6));
  }
  else
  {
    dataString += "No fix";
    dataString += ",";
    dataString += "No fix";
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("data.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening data.csv");
  }

  fonaSerial->write("AT+CHTTPSOPSE=\"ultimeter-altimeter.firebaseio.com\",443,2\r\n");
  delay(500);
  echoFona();

  int clength = sprintf(content, "{\"Latitude\":%d,\"Longitude\":%d", latitude_mdeg, longitude_mdeg);
  int totallength = sprintf(packet, "POST /data.json HTP/1.1\r\nHOST: ultimeter-altimeter.firebaseio.com:443\r\nContent-Length: %d\r\n\r\n %s\r\n", clength, content);
  sprintf(sendcommand, "AT+CHTTPSSEND=%d", totallength);
  
  fonaSerial->write(sendcommand);
  delay(500);
  echoFona();
  
  fonaSerial->write(packet);
  
  delay(1000);
}

void printEvent(sensors_event_t* event) {
  //Serial.println();
  Serial.print(event->type);
  double x = -1000000, y = -1000000 , z = -1000000; //dumb values, easy to spot problem
  if (event->type == SENSOR_TYPE_ACCELEROMETER) {
    lowgX = event->acceleration.x;
    lowgY = event->acceleration.y;
    lowgZ = event->acceleration.z;
  }
  
  Serial.print(": x= ");
  Serial.print(lowgX);
  Serial.print(" | y= ");
  Serial.print(lowgY);
  Serial.print(" | z= ");
  Serial.println(lowgZ);
}

void SFE_UBLOX_GPS::processNMEA(char incoming)
{
  //Take the incoming char from the Ublox I2C port and pass it on to the MicroNMEA lib
  //for sentence cracking
  nmea.process(incoming);
}

void setupBQ27441(void)
{
  // Use lipo.begin() to initialize the BQ27441-G1A and confirm that it's
  // connected and communicating.
  /*if (!lipo.begin()) // begin() will return true if communication is successful
  {
  // If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    while (1) ;
  }*/
  
  // Uset lipo.setCapacity(BATTERY_CAPACITY) to set the design capacity
  // of your battery.
  lipo.setCapacity(BATTERY_CAPACITY);
}

void printBatteryStats()
{
  // Read battery stats from the BQ27441-G1A
  unsigned int soc = lipo.soc();  // Read state-of-charge (%)
  unsigned int volts = lipo.voltage(); // Read battery voltage (mV)
  int current = lipo.current(AVG); // Read average current (mA)
  unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
  int power = lipo.power(); // Read average power draw (mW)
  int health = lipo.soh(); // Read state-of-health (%)

  // Now print out those values:
  String toPrint = String(soc) + "% | ";
  toPrint += String(volts) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(fullCapacity) + " mAh | ";
  toPrint += String(power) + " mW | ";
  toPrint += String(health) + "%";
  Serial.println(toPrint);
}

void repetitionsIncrease() {
  // This function will be called once on device wakeup
  // You can do some little operations here (like changing variables which will be used in the loop)
  // Remember to avoid calling delay() and long running functions since this functions executes in interrupt context
  repetitions ++;
}

void echoFona() {
    while (!fonaSerial->available()) {
  }
  while (fonaSerial->available()) {
    Serial.write(fonaSerial->read());
  }
}
