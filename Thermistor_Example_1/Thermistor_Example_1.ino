/*
 * thermistor-example-1.ino
 * Obtain an accurate temperature from a thermistor
 *
 * Copyright (c) 2023 David G Sparks
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * This code was written for and tested on 
 * an Arduino Nano development board based on the
 * ATmega328P microcontroller.
 * 
 * 05June2023 changed ADC top value constant to 1024
 * to conform with datasheet. Added a test to avoid
 * a zero result from the ADC to prevent a runtime error.
 */

// Nano accepts analog input on pin A7.
// Modify as project requires for other boards.
#define ANALOG_INPUT 7

/*
 * 10 bits for older AVR-based MPUs and ESP8266
 * 12 bits for ESP32 and the new Arduino Uno R4
 * 12 bits also for the new AVR Dx series of MPUs
 */
#define ADC_RESOLUTION 10
// #define ADC_RESOLUTION 12
#define ADC_MAX ( pow(2,ADC_RESOLUTION) )

// Constants determined from the data sheet
// targeting range of -10 C to +30 C
#define REF_TEMP    263.15 // - 10 C + 273.15 degrees K
#define REF_RESIST  57670  // Ohms
#define BETA        3936 // 3936 for -10 to 30

// Fixed Resistor in series after the thermistor
#define R_FIXED     30000 // Ohms 

char vbgString[12];       // to format floating-point output

// function prototypes, defined below after loop()
double getTempK (void);
double getTempC (void);
double getTempF (void);
uint16_t readAnalogPin( uint8_t );

void setup() {

  delay(2000); 
  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.println("Serial started.");
}

void loop() {
  delay(1000);
  Serial.print("tempF: ");
  Serial.println(dtostrf(getTempF(), 6, 1, vbgString));
}


uint16_t readAnalogPin( uint8_t pin)
{
  #define NUM_SAMPLES 10
  uint16_t adcAverage = 0;
  pinMode(pin, INPUT);
  for (int i = 0; i < NUM_SAMPLES; i++)
  {
    adcAverage += analogRead(pin);
    delay(5);
  }
//  Serial.println(adcAverage);
  adcAverage /= NUM_SAMPLES;
  // ensure ADC reading > zero
  if (adcAverage == 0) adcAverage = 1;
  return adcAverage;
}

double getTempK ()
{
  double temp = // a scratchpad variable
  ( 
    (ADC_MAX / readAnalogPin ( ANALOG_INPUT ) )
     - 1)
    * R_FIXED; // begin with resistance in the thermistor
    
//  Serial.print("Resistance: ");
//  Serial.println(temp);   

// Device error could be as much as 10% above or below.
// Adjust for a measured resistance 6% below actual.
   temp *= 1.07; // apply correction factor

  temp = // temperature derived from resistance
  ( 
    1 / 
    ( (1.0/REF_TEMP) - log(REF_RESIST/temp)/BETA )
  ); // temperature in degrees K
//  Serial.print("Temp K: ");
//  Serial.println(temp);
  return temp;
}

double getTempC () {
  return getTempK () - 273.15;
}

double getTempF () {
  return getTempC () * 1.8 + 32;
}
