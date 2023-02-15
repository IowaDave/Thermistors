/*
  Copyright (c) 2023 David G Sparks.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Contact the author at github.com/IowaDave
*/

/* This program compute coefficients for the Steinhart-Hart Equation,
 * given resistance values at three different temeratures
 * from data sheet, at intervals of 10 degrees C.
 * 
 * Then it works the Equation in reverse to estimate
 * expected resistance values at 1-degree intervals
 * on the range -40C to +50C.
 * 
 * The specific device referenced here is an NTC
 * thermistor by Jameco, model "103", part number 207037
 * having 10K-Ohm resistance at 25 degrees Celsius.
 * https://www.jameco.com/z/NTC-103-R-Jameco-Valuepro-Thermistor-NTC-K-10-10k-Ohm_207037.html
 * 
 * Code written for and tested on a Seeed XIAO SAMD21 
 * Arduino-compatible development board. 
 */

char fltStr[20]; // for debugging use with dtostrf()

// variables for the Steinhart-Hart coefficients
  double A_coefficient;
  double B_coefficient;
  double C_coefficient;
  double z2; // an intermediary value
  double z3; // an intermediary value

// struct for data points
 struct DATAPOINT {
    int32_t Ctemp; // temperature in Celsius
    int32_t R; // resistance in Ohms
    double K; // temperature in Kelvin
    double Y; // 1/K
    double L; // log(R)

    DATAPOINT () {}; // default constructor
    DATAPOINT (double tempCelsius, double resistance = 0) 
      : Ctemp {tempCelsius}
      , R {resistance}
      , K { Ctemp + 273.15 }
      , Y { 1/K }
      , L { log (R) } 
      { };
 };

// Given: three measured value pairs
DATAPOINT T1 (-30,  188200);
DATAPOINT T2 (-20,  102300);
DATAPOINT T3 (-10, 57670);

void setup() {
  
  // calculate the z2 and z3 values
  z2 = (T1.Y - T2.Y) / (T1.L - T2.L);
  z3 = (T1.Y - T3.Y) / (T1.L - T3.L);
  
  // calculate the C, B and A coefficients
  
  C_coefficient = 
    ( (z3 - z2) / (T3.L - T2.L) )
    / (T1.L + T2.L + T3.L);
  
  B_coefficient = 
    z2
    - (
        C_coefficient // that's the coefficient, not a parenthesis
        * (
            (T1.L * T1.L) // L1^2
          + (T1.L * T2.L) // L1 x L2
          + (T2.L * T2.L) // L2^2
        )
      );

  A_coefficient = 
    T1.Y
    - (
        T1.L
        * (
            B_coefficient 
            + ( 
                T1.L * T1.L * C_coefficient 
              )
          )
      );

  Serial.begin(9600);
  while (!Serial);

/* some debugging output
  // print the coefficients
  Serial.print("C: ");
  Serial.println(dtostrf (C, 17, 15, fltStr) );
  Serial.print("B: ");
  Serial.println(dtostrf (B, 17, 15, fltStr) );
  Serial.print("A: ");
  Serial.println(dtostrf (A, 17, 15, fltStr) );
*/

  // process temperatures in the range -40 to +50 C
  for (int32_t celsius = -40; celsius < 51; celsius++) {
    DATAPOINT Tn = DATAPOINT (celsius);
    double x = ( 1/C_coefficient ) * ( A_coefficient - Tn.Y );
    double y = sqrt(
      pow((B_coefficient / (3 * C_coefficient )), 3)
      + ( pow(x, 2) / 4)
    );

/* some debugging output
    Serial.print(Tn.C);
    Serial.println(": ");
    Serial.print("x: ");
    Serial.println(dtostrf (x, 17, 15, fltStr));
    Serial.print("y: ");
    Serial.println(dtostrf (y, 17, 15, fltStr));
*/    
    Tn.R = (uint32_t) exp (
      cbrt(y - (x/2)) - cbrt(y + (x/2))
    );
    Serial.print(celsius);
    Serial.print(": ");
    Serial.println(Tn.R);
  }
  
} // setup

void loop() {
  // no activity here
}
