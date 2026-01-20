////////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: SPI protocol experiment. Communicates with the TI TMP121 temperature sensor.

AUTHOR: Justus Downing

COMMENTS: The program reads a 16-bit temperature value every 100-250ms from the device using SPI protocol. 
The sensor is read only, so the MASTER need only assert the SCLK pin and then clock out the data on the SLAVE 
MISO pin. The data is 2-bytes long, 16-bit, where the MSB is shifted first of the upper byte then the lower byte
is shifted. The lower 3-bits of the data are 0. So the value read is really 13-bit two's complement. Therefore, 
a bit of shifting needs to be done to get the data into proper position. Also, the value is in Celcius and a value 
of 1 is equal to 0.0625, so the data must be scaled as well.

HARDWARE SETUP:

Used the TMP121 IC and mounted it myself.



Arduinio UNO              TMP121 Sensor

Signal Name     Pin#      Signal Name     Pin #
____________________________________________________________

Power Connections
VCC (5V)        ANY       V+              3
GND             ANY       GND             2

SPI Connections from Arduino to TMP121
MOSI            11        NA              NA (TMP121 is read-only, so MOSI from Arduino is not needed).  
MISO            12        SO              6  (This is called "serial out", it's the data from the SLAVE device to the MASTER).
SCK             13        SCK             4  (This is the SPI clock, the MASTER drives the clock, simply connect to SLAVE).
/CS             7         /CS             5  (This is the chip select line, notice it is active LOW, /CS).

*/


////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES 
////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SPI.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// DEFINES AND MACROS
////////////////////////////////////////////////////////////////////////////////////////

// SPI chip select, active LOW, we could use any pin for this, but according to the R3 pinout, the default pin usually
// is pin 10, but I use pin 7.
const uint8_t SPI_nCS = 7; // default is 10 usually  

////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////

char gStringBuffer[ 80 ];

////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
// the setup function runs once when you press reset or power the board  

// initialize serial port
Serial.begin(115200);
Serial.write( "\n\rTMP121_Temperature_Sensor\n\r" );

// setup SPI CS pin, and de-assert
pinMode( SPI_nCS, OUTPUT );
digitalWrite( SPI_nCS, HIGH );

// initial SPI driver, "SPI" object created in SPI.h/cpp files for us, so no need to instantiate an object
SPI.begin();

} // end setup

////////////////////////////////////////////////////////////////////////////////////////

void loop() 
{
int16_t tempData;   // holds the 16-bit temperature data read from TMP121 (13 bits valid)

// begin SPI transactions...
SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

// manually assert CS, very important
digitalWrite( SPI_nCS, LOW );

#if 1

// read high byte
tempData = (SPI.transfer( 0x00 ) << 8);
tempData = tempData | ( SPI.transfer( 0x00 ) << 0 ); 


// lastly mask off lower 3-bits make sure they are 0s (Remeber from TMP121 datasheet)
tempData = tempData & (0xFFF8);
#endif

// tempData is shifted 3 bits to left, we need to adjust to right taking into consideration sign bit

// positive case, shift, do nothing to sign
if ( tempData > 0 )
  tempData = tempData >> 3;
else
// negative case, shift, and then sign extend upper bits to maintain two's complement
  tempData = (tempData >> 3) | 0xE000;

// manually de-assert CS
digitalWrite( SPI_nCS, HIGH );

// end SPI transactions...
SPI.endTransaction();

// convert temp to float, scale by 0.0625, convert C to F, then print temperature
sprintf( gStringBuffer, "\n%d", (int)(0.5 + ((float)tempData) * 0.0625f * 1.8f + 32.0f ) );
Serial.print( gStringBuffer );

// pause a moment, spec requires about 300mS between chip selects, else IC won't have time to sample temp and convert
delay( 250 );

} // end loop

////////////////////////////////////////////////////////////////////////////////////////
