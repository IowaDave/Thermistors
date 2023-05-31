# Thermistor-Thermometer
Build an accurate digital thermometer with a simple, inexpensive sensor.

![Thermistor with Arduino Nano](https://github.com/IowaDave/Thermistors/blob/main/images/Thermistor.jpg)<br>
**Thermistor with an Arduino Nano-compatible controller**

This article puts a 39-cent thermistor to work as a digital thermometer. It needs just a few lines of code.

I target my example project to the range of outside air temperatures likely to be encountered during April and May in the upper midwestern U.S., &ndash;10&#8451; to 30&#8451;. This target will shape the code development.

## Contents

#### [Introduction](#introduction)
#### [How It Works](#how-it-works)
#### [The Circuit](#the-circuit)
#### [Data Sheet](#data-sheet-information)
#### [Simple Usage](#simple-usage)
#### [A Better Approach?](#a-better-approach?)
#### [The Characteristic Curve](#the-characteristic-curve)
#### [How I Could Be Wrong](#how-i-could-be-wrong-1)
#### [Wonky Calculations](#wonky-calculations-1)
#### [Links](#links-1)

## Introduction
Many web pages exist to describe or demonstrate using thermistors for temperature measurement. For example, this writer reviewed seven Arduino-style libraries. See the [links](#links-1) provided below. 

A reader can stop now and go sift through all those how-tos online. Or stay here and delve into it a little farther with me. 

Because the articles I reviewed left me wondering: how does one measure a temperature *accurately* using a thermistor?

What attracts me to thermistors is their simplicity compared to &ldquo;intelligent&rdquo; sensors marketed to hobbyists, such as the popular DS3231, DHT11 and DHT22 devices. 

Those products cost money and report their readings through serial communications that require specialized code libraries.

By contrast, an inexpensive thermistor can be read directly by a microcontroller without resort to a library.

I'm curious to know more than merely how to cobble together an apparatus: how does a thermistor work, and how can one be calibrated to obtain reasonable accuracy?

## How It Works
A thermistor is a kind of resistor having a special property: its resistance varies across a very wide range with high sensitivity to changes in temperature.

Pairing a thermistor with a fixed-value resistor gives a voltage divider. For a certain, constant voltage input, $V_I$, the voltage output, $V_O$, increases or decreases in response to a decrease or increase in temperature.

Code can evaluate the voltage output using the analog-to-digital converter (ADC) peripheral in a microcontroller. The value returned by the ADC can be interpreted as the resistance value in the thermistor. 

The resistance, by the nature of the materials in the thermistor, corresponds predictably to a temperature with remarkable precision.

However...

The change of resistance for a given change of temperature is not linear. This poses a challenge for code writers seeking to determine the temperature that corresponds to the measured resistance.  

There is more than one way to go about it. Accuracy of the temperature may be influenced by the method applied to interpret the ADC reading.

This article presents the least challenging approach to a reasonably useful result. Then it goes on to examine other methods that may be worth the effort in situations calling for greater accuracy.

## The Circuit

![Schematic](https://github.com/IowaDave/Thermistors/blob/main/images/schematic.svg)

The input voltage, ```Vcc```, is supplied to both the MCU and the voltage divider. The MCU will use this voltage as the ADC reference. The voltage divider will supply a reduced voltage to the analog input pin marked ```A0```. The MCU and the voltage divider share a common ground.

$R1$ in the diagram is the thermistor. The project described here used a Jameco LM05-103&nbsp;K part. In the photo it appears in the upper-right quadrant, a red disc on two, tall leads.

$R2$ is a fixed-impedence resistor. The example project uses one having impedance of 30K Ohms. This value was selected after studying the data sheet for the LM05-103 thermistor.

## Data Sheet Information

Here is a link to the Jameco LM05-103&nbsp;K part used in this project: [https://www.jameco.com/Jameco/Products/ProdDS/207037.pdf](https://www.jameco.com/Jameco/Products/ProdDS/207037.pdf).

There are many different thermistors having widely varying specifications. The information needed to use a particular thermistor is found in its manufacturer's data sheet. 

At a minimum, one needs to know the Big Three quantities about a thermistor. The following excerpt provides them for the LM05-103.

![Data Sheet excerpt #1](https://github.com/IowaDave/Thermistors/blob/main/images/DS_table_1.png)<br>
**Table 1 Reference Values and Beta**<br>
*Source: LM05-103&nbsp;K data sheet*

The first line gives two of the quantities: a nominal impedance of 10K Ohms at a temperature of 25 degrees Celsius. &ldquo;Nominal&rdquo; means &ldquo;intended&rdquo;. 

The third line gives the third quantity. It states that something called &ldquo;B Constant 25&#8451;/50&#8451;&rdquo; has a value near 4038. Another name for this number is &ldquo;Beta&rdquo;.

A reference temperature, the resistance at that temperature and the relevant Beta value are enough information to calculate a temperature with some of the more rudimentary open-source thermistor libraries available for the Arduino IDE. 

## Simple Usage
See the example program that accompanies a "Thermistor" library in the following github repository: [https://github.com/panStamp/thermistor](https://github.com/panStamp/thermistor).

**Know Your Thermistor!** That library expects the user to wire up an "NTC" type of thermistor that was designed for temperature measurement. (Those designed to deal with "inrush current" may be less suitable.) Wiring should follow the circuit diagram shown above. Finally, the code writer must supply the reference values given in the manufacturer's data sheet for the device being used.

Using such a library with a particular thermistor, given only the Big Three quantities from the data sheet, or (worse) from a product description on a mass-merchant's web site, will give a result that *looks like* a temperature. (He shrugs.) It's a place to start.

But how accurate is it?

### Errors of Approximation 
At best, the Big Three quantities can serve *to estimate* the temperature. The math involved will result in some difference between the actual and the estimated temperatures. 

Why that happens and ways to improve the estimate are discussed in the sections that follow.

Also, the Big Three numbers describe an ideal example of the device. Individual devices will vary from the ideal, introducing another source of error.
 
For example, the second line of Table 1 states that the actual resistance of a a Jameco LM05-103&nbsp;K part may differ by as much as 10% above or below the ideal level. In this case, it could be as low as 10K &ndash; 10% = 9,000 Ohms or as high as 10K + 10% = 11,000 Ohms. 

That works out to accuracy within about 4 degrees at room temperature. There are two ways to improve it.

Thermistors accurate to within 1% are available at higher prices. Now you're talking accuracy to within a half of a degree or less. Maybe the extra cost makes sense if you are mass-producing digital thermometers and wish to omit testing each part.

Fortunately for us hobbyists who build things one at a time, simply calibrating a single, inexpensive thermistor may restore it to near ideal performance. How to do this will be explained below, in [The Code](#the-code) section.


## A Better Approach?

This article does not use a library like that, for two reasons. Firstly, the code is really short. Secondly, a good data sheet will provide more complete information that may promote better accuracy. The following shows part of such a table for the LM05-103.

![Data Sheet excerpt #2](https://github.com/IowaDave/Thermistors/blob/main/images/DS_table_2.png)<br>
**Table 2 Resistance at Different Temperatures**<br>
*Source: LM05-103&nbsp;K data sheet*

Look twice at Table 2. The values for resistance follow a curve, rather than a straight slope. The numbers change by different amounts from one temperature to the next. Going down ten degrees from 30&#8451; to 20&#8451; the resistance increases by about 2,500 Ohms, from 20&#8451; down to 10&#8451; by almost 8,700 Ohms, from 10&#8451; down to 0&#8451; by more than 13,000 Ohms, and so forth.

The best a single number can do, such as the Beta in Table 1, is to approximate what actually happens to the thermistor's resistance as temperature fluctuates close to the reference temperature.

There are ways to approach a temperature estimate using data from Table 2 that may come closer. I used a different value for Beta in my project, 3936, for reasons described below in [The Characteristic Curve](#the-characteristic-curve) section.

The math for this project triggered a flashback to 9th grade algebra. If you really must see it, visit the [Wonky Calculations](#wonky-calculations-1) section at the end of this article.

## The Code

The example code incorporates a set of Big Three constants calculated beforehand. Then, just two code statements do the work. They are found in the subroutine named ```getTempK()```.

### Assess the Output of the Voltage Divider
The output of the voltage divider is led to an analog input pin on the Arduino. The ```analogRead()``` function activates the analog-to-digital converter (ADC) peripheral inside the microcontroller.

The ADC calculates a ratio of the analog input voltage compared to a reference voltage. Most MCUs use their main power input voltage as the reference by default. 

Actually the ADC returns an integer representing that ratio multiplied by the &ldquo;resolution&rdquo; of the ADC, that is, the maximum value that the ADC can return. 

What is that maximum value? It depends on the MCU.

* Processors found on older Arduino boards and on the ESP8266 device give 10-bit resolution, making the maximum 1023. 
* SAMD21-based Arduinos including the Zero, Nano 33 IoT and MKR-series boards feature 12-bit ADCs, increasing the ADC maximum value to 8191. 
* The new Arduino Uno R4 and the Expressif ESP32 boards support 12-bit resolution, also.

The following code segment from the example program deals with this by defining the ADC_RESOLUTION to be either 10 bits or 12 bits. The code example below specifies 10 bits. Your code should specify the one that corresponds to your device. The macro ADC_MAX then represents the ADC resolution.

```
#define ADC_RESOLUTION 10
// #define ADC_RESOLUTION 12
#define ADC_MAX ( pow(2,ADC_RESOLUTION) - 1 )
```

The following defines the ADC measurement, $M_{ADC}$.

```math
M_{ADC} = ADC\_MAX \times  \frac{V_O}{V_I}
```
where:<br>
&nbsp;&nbsp; $V_I$ is the input voltage applied to the thermistor, the same voltage that also powers the MCU.<br>
&nbsp;&nbsp; $V_O$ is the output voltage at the junction of $R1$ with $R2$.

This measurement, $0 <= M_{ADC} <=$ ADC_MAX, can be reinterpreted as the output voltage, $V_O$. However, we do not really need to know the voltage. We need only to know the resistance in the thermistor. 

### Infer a Resistance
First make sure that the voltage applied to the voltage divider, $V_I$ is the same as that powering the microcontroller. Then the ADC measurement can be converted directly into the resistance of the thermistor, knowing the values of $M_{ADC}$ and of the fixed-value resistor that follows it in series.

With the thermistor in the $R1$ position of the voltage divider, the following equation solves for its resistance:

```math
R1 = \left(\frac{ADC\_MAX}{M_{ADC}} - 1\right) \times R2
```
where $M_{ADC} > 0$.

This calculation occupies one code statement:

```
  // calculate thermistor resistance 
  // into a temporary variable
  double temp =
    ( (ADC_MAX / readAnalogPin ( ANALOG_INPUT ) ) - 1 )
    * R_FIXED;
```

The equation can be rearranged when the thermistor is in the R2 position:

```math
R2 = \frac{M_{ADC} \times R1}{ADC\_MAX - M_{ADC}}
```
where $M_{ADC} <$ ADC_MAX.

### Calculate a Temperature
The example code operates with a customized set of Big Three constants, for reasons explained below. Their values are:

1. $T_{REF}$, the selected reference temperature, &ndash;10&#8451;,
2. $R_{REF}$, the resistance at the reference temperature, 57670 Ohms, from Table 2,
3. Beta, calculated to be 3936 for the range &ndash;10&#8451; to 30&#8451;,

The temperature $T_{TEMP}$ being sensed by the thermistor is calculated from the measured resistance, $R_{TEMP}$, as follows. 

```math
\frac{1}{T_{TEMP}} = \left(\frac{1}{T_{REF}} 
- \frac{\ln(R_{REF})}{Beta}\right) 
+ \frac{ln(R_{TEMP})}{Beta}
```

Keep in mind that the temperatures need to be in degrees Kelvin, where 0&#8451; = 273.15&#730;&#8490;. The reference temperature becomes $273.15 - 10 = 263.15$.

```math
\frac{1}{T_{TEMP}} =  \left(\frac{1}{263.15}
 - \frac{ln(57670)}{3936}\right) 
 + \frac{ln(R_{TEMP})}{3936}
```

Again, the calculation consumes just one code statement

```
  // re-use the temporary variable , "temp",
  // to calculate temperature in degrees K,
  // based on the value for resistance
  // that is already contained in the variable
  temp = 
  ( 
    1 / 
    ( (1.0/REF_TEMP) - log(REF_RESIST/temp)/BETA )
  );
```

Invert the result then subtract 273.15 to arrive at the temperature in degrees Celsius.

### Calibrate to Improve the Accuracy
OK, now we're reading a temperature from our apparatus. How close is it to the expected value?

For example, the one in the photo indicated 73.2&deg;F when my room temperature was 70&deg;F. The difference was 3.2&deg;F. 

One way to resolve this discrepancy is to calibrate the resistance value before calculating the temperature. 

A short narrative can describe the general idea and yield a good approximation.

Look at Table 2 again. 70&deg;F is near 21&deg;C. At 25&deg;C we expect to see a resistance of 10,000 Ohms. At 20&deg;C it rises to 12,560 Ohms, an increase of about 500 Ohms per degree of *decrease* in temperature.

So, at 70&deg;F the resistance would be near $12,560 - 600 \approx 12,000$ Ohms. What does our voltage divider tell us? Print it out. The apparatus in the photo reported about 11,200 Ohms.

The expected resistance of 12,000 is about 1.07 times the measured value of 11,200 near 70&deg;F because $\frac{12000}{11200}  \approx 1.07$. 

Calibration is now simple: multiply the measured resistance times the ratio computed *for this particular thermistor*, 1.07. Insert this step to adjust the resistance value before calculating the temperature.

```
   temp *= 1.07; // apply calibration ratio
```

The ratio found at one temperature for one device may work well across the whole range of temperatures for that same device.

However, the ratio is likely to be different for each device you might find in a bagful of the same part number. Simply re-perform the calibration calculation for each device.

## The Characteristic Curve

Table 2 helps us to understand that business about &ldquo;25&#8451;/50&#8451;&rdquo; in Table 1. The Beta value given there, 4038, was calculated using data from resistances at the temperatures 25&#8451; and 50&#8451;. The calculation was:

```math
\beta = \frac{\ln\left(\frac{R_{25}}{R_{50}}\right)}{\frac{1}{T_{25}} - \frac{1}{T_{50}}} 
```

The temperatures need to be converted to the Kelvin scale by adding 273.15. Substituting actual values, the calculation becomes:
```math
\beta =  \frac{\ln\left(\frac{10000}{3507}\right)}{\frac{1}{298.15} - \frac{1}{323.15}} \approx 4038
```

The formula for deriving the Beta can be rearranged to estimate temperatures. It works by interpolating along a straight line that passes through two data points.

Figure 1 illustrates a Characteristic Curve of a thermistor. It plots the actual resistance at each temperature. The plot follows a curve. This means the resistance value found on the straight &ldquo;Beta&rdquo; line will not be exactly right. It is only an estimate. 

Figure 1 illustrates the gap between the Beta line estimates and the actual values found on the Characteristic Curve.

The Curve bends in such a way that estimating with Beta comes closest for resistance values between the two data points that were used to compute the Beta.

Notice that the Beta value given in Table 1 relates to the range of temperatures 25&#8451; to 50&#8451;. 

The graph in Figure 1 illustrates the limitations of using those two data points to calculate a value for Beta.

![Characteristic Curve Graph](https://github.com/IowaDave/Thermistors/blob/main/images/teamwavelength-log-graph-with-beta.png)<br>
**Figure 1 A Characteristic Curve Showing Beta 25&#8451;/50&#8451;**<br>
*Modified from image source [https://www.teamwavelength.com/thermistor-basics/](https://www.teamwavelength.com/thermistor-basics/) to add Beta line.*

The Beta line stays reasonably close to the curve between the two points but diverges from it at temperatures above or below the points.

It's easy to see in Figure 1 that a different set of end points could be more suitable for a colder range of temperatures. While the particular values depicted in Figure 1 are not those for the thermistor used in my project, the concept still applies.

The example code targets the range of temperatures likely to be encountered during April and May in the upper Midwestern U.S.: &ndash;0&#8451; to +30&#8451;.  The Beta for that range turned out to be 3936, calculated from the relevant values in Table 2.

```math
\beta =  \frac{\ln\left(\frac{57670}{8013}\right)}{\frac{1}{263.15} - \frac{1}{303.15}} \approx 3936
```

Similarly, a Beta for a winter range of &ndash;30&#8451; to +10&#8451; works out to be 3830. If one is going to use Beta for estimating a temperature within a certain target range, then why not work with the Beta value for that range?

I found that customizing the Beta and reference inputs then calibrating the measured resistance, as described above, worked well enough for my purposes. The thermistor indicated temperatures that agree to within a fraction of one degree compared to other thermometers I keep around the house.
<hr>

There are further ways to improve the temperature estimate corresponding to a measured resistance.

### Restrict the Range
Take a look at the graph in Figure 1.

The first and simplest way to improve the estimate may be to select two points closer to each other and calculate a Beta for that more narrow range. 

I would expect interpolation to become more accurate because a curve approaches closer to a straight line between two points as one brings the points closer together.

The range selected should correspond to the temperatures where the thermistor's reading would be of greatest interest.

A consideration for this approach would be that such a narrow Beta would likely be less accurate outside of its target range, compared to one computed for a wider range.

### Apply a Better Calculation
A lot of work has been done to find a good mathematical model for the characteristic curve of a thermistor.

According to Wikipedia, John S. Steinhart and Stanley R. Hart worked together on the problem at the Carnegie Institution of Washington in the 1960s. 

As I understand it, they started with the equation for Beta and asked, "Suppose you could narrow the range down to two points infinitely close to the measured resistance, then calculate *that* Beta? It would give precisely the right answer!"

They looked for a reasonably simple equation that would get really close at every point on the characteristic curve. In fact they evaluated a number of different candidate equations. In 1968 they published the one that is generally accepted today:

```math
\frac{1}{T} = A + B\ln(R) + C\ln(R)^3
```

The Steinhart-Hart equation actually skips the Beta. It takes the measured resistance, ```R``` and goes straight to the temperature, ```T```. 

All you need to know in advance are the three coefficients, ```A```, ```B``` and ```C```.

Some data sheets in the past would publish those coefficients. They don't seem to nowadays. However, they do typically provide tables like Table 2, above, and coefficients can be calculated using data found there.

Calculating the coefficients begins with values for three different temperature/resistance points.

The math for finding the coefficients has been well described elsewhere. See: 

[https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation](https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation)

See also the following link, which even provides an online calculator:

[https://www.ametherm.com/thermistor/ntc-thermistors-steinhart-and-hart-equation](https://www.ametherm.com/thermistor/ntc-thermistors-steinhart-and-hart-equation)

Some of the github libraries listed below among the [Links](#links-1) implement the Steinhart-Hart equation. One of them might even calculate the coefficients for you!

A unique benefit of the Steinhart-Hart equation is that it can be used with thermistors for which no data sheet is available. One carefully measures actual resistance of the thermistor at three different temperatures, spaced evenly at intervals reasonably far apart. The measurements allow a set of coefficients to be calculated for that thermistor.

## How I Could Be Wrong
I feel pretty confident about relatively simple steps toward accuracy described above, such as refining the method of calculation and applying a calibration ratio.

However, some things I do not know (yet) could make me change my mind. For the sake of completeness, consider the following.

* **More rigorous testing could be called for.** I merely used other thermometers in the room as my reference. Proper apparatus probably involves immersing the thermistor in carefully heated or chilled fluids. That was not going to happen in my bride's kitchen!
* **The calibration ratio might not be constant.** Just as the resistance itself changes as the thermistor's temperature changes, the *rate of change* in the resistance also might change. This drills down into the quantum nature of the materials forming the thermistor. Color me ignorant on that level.
* **The fixed resistor changes with temperature, too.** If it is to be exposed to the same environment as the thermistor, then its variation will have some effect on the measurement. My study held the fixed resistor constant for the sake of convenience.
* **Current may heat the thermistor.** My code example expects the thermistor circuit to remain energized continuously. Further minimizing current, e.g., with a higher-valued fixed resistor, or even switching it on only briefly to take measurements, may affect results. I chose not to pursue it, for simplicity's sake.
* If I think of any more, I'll add them here.

What I want to explore next is how closely this thermistor follows changes in outside air temperature. I plan to place a thermistor on the back porch to compare it with the commercial digital thermometers already out there.

Onward.


## Wonky Calculations
Golly gee it felt funny to be back in 9th grade algebra class!

### Calculating Resistance from the ADC Measurement
We combine two equations.

#### Equation #1 Output of a Voltage Divider, $V_O$

```math
V_O = V_I \times \frac{R2}{R1 + R2}
```
where: <br>
&nbsp;&nbsp; $V_I$ is the voltage fed into Resistor R1 <br>
&nbsp;&nbsp; R2 is the resistor following R1 in series, terminating at ground. 

#### Equation #2 ADC Measurement, $M_{ADC}$

```math
M_{ADC} = 1023 \times \frac{V_O}{V_I}
```
when $V_I$ applied to the voltage divider is also the voltage used for reference in the ADC.

Rearrange #2

```math
V_O = \frac{V_I \times M_{ADC}}{1023}
```

Substitute into #1

```math
\frac{V_I \times M_{ADC}}{1023} = \frac{V_I \times R2}{R1 + R2}
```

Invert and rearrange

```math
\frac{R1}{V_I \times R2} + \frac{R2}{V_I \times R2} = \frac{1023}{V_I \times M_{ADC}}
```

Multiply by $V_I$

```math
\frac{R1}{R2} + \frac{R2}{R2} = \frac{1023}{M_{ADC}}
```

Rearrange and simplify

```math
\frac{R1}{R2}= \frac{1023}{M_{ADC}} - 1
```

Multiply by R2
```math
R1 = R2 \times \left(\frac{1023}{M_{ADC}} - 1\right)
```

### Calculating Temperature from Resistance Using Beta
The formula for Beta takes two selected (temperature, resistance) data points from the data sheet.

```math
\beta = \frac{\ln\frac{R_{REF}}{R_{TEMP}}}{\frac{1}{T_{REF}} - \frac{1}{T_{TEMP}}}
```
where:

* $R_{REF}$ is the resistance at a chosen base temperature, $T_{REF}$.
* $R_{TEMP}$ is the resistance at a temperature $T_{TEMP}$ greater than the base

Note that with NTC thermistors the resistance increases as the temperature decreases, and vice versa.

We can rearrange the equation to solve for an unknown $T_{TEMP}$ after we have measured the resistance $R_{TEMP}$ in the thermistor.

Rewrite the numerator

```math
\beta = \frac {\ln(R_{REF}) - \ln(R_{TEMP})} {\frac{1}{T_{REF}} - \frac{1}{T_{TEMP}}}
```

Multiply by $-\left(\frac{\frac{1}{T_{REF}} - \frac{1}{T_{TEMP}}}{\beta}\right)$ and rearrange

```math
\frac{1}{T_{TEMP}} =  \left(\frac{1}{T_{REF}} - \frac{\ln(R_{REF})}{\beta}\right) + \frac{\ln(R_{TEMP})}{\beta}
```

The values between parentheses on the right-hand side of the equation are the Big Three constants. Thus, the unknown temperature is simply a function of the resistance in the thermistor.

Note that in C++ the natural logarithm $\ln()$ is coded as ```log()```.


## Links
During April and May 2023 I looked at least briefly at some of the contents accessed on the following URLs. No attempt was made to verify the contents. Information found online, including this article, may contain errors and can change. I list the links solely as an aid to memory.

### Github Repositories

* [https://github.com/panStamp/thermistor](https://github.com/panStamp/thermistor)
* [https://github.com/YuriiSalimov/NTC_Thermistor](https://github.com/YuriiSalimov/NTC_Thermistor)
* [https://github.com/electronicdrops/Thermistor](https://github.com/electronicdrops/Thermistor)
* [https://github.com/csosborn/Thermistor](https://github.com/csosborn/Thermistor)
* [https://github.com/AWatk/arduino-thermistor](https://github.com/AWatk/arduino-thermistor)
* [https://github.com/wyunreal/ThermistorLibrary](https://github.com/wyunreal/ThermistorLibrary)
* [https://github.com/miguel5612/ThermistorLibrary](https://github.com/miguel5612/ThermistorLibrary)

### About Thermistors

* [https://en.wikipedia.org/wiki/Thermistor](https://en.wikipedia.org/wiki/Thermistor)
* [https://learn.adafruit.com/thermistor/overview](https://learn.adafruit.com/thermistor/overview)
* [https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html](https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html)


### Vendors
I bought some thermistors from Jameco. Mouser sells them too. I feel comfortable buying from both of these firms. This is just a satisfied customer's personal impression. I do not receive any compensation or other benefit from them.

The point of buying thermistors from a reputable vendor is that they provide product-specific data sheets. By contrast, some popular online &ldquo;shopping&rdquo; sites do not make data sheets available.

The links below were responsive at the time of writing in May, 2023.

Jameco:

* [https://www.jameco.com/shop/keyword=thermistors](https://www.jameco.com/shop/keyword=thermistors)

Mouser -- those having the &ldquo;through hole&rdquo; termination style look like they would plug into the breadboards familiar to us amateur hackers.

* [https://www.mouser.com/c/circuit-protection/thermistors/ntc-thermistors/](https://www.mouser.com/c/circuit-protection/thermistors/ntc-thermistors/)
 