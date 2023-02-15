# Thermistors
Using thermistors with microcontrollers to sense temperature

A thermistor is a type of semiconductor. Its resistance to current flow varies with temperature. 

The resistance can be evaluated and interpreted as a temperature.

These simple devices cost very little. Yet they are durable and can deliver usefully accurate measurements.

Perhaps best of all, these sensors do not require a special library. The analogRead() function of an Arduino is sufficient to read the device. A bit of code converts the reading into its corresponding temperature.

Jameco Electronics supplies a range of different thermistors. The firm gives a good introduction to the devices, including a simple demonstration script, at the following url:

[https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html](https://www.jameco.com/Jameco/workshop/TechTip/temperature-measurement-ntc-thermistors.html)


Putting it very briefly, a thermistor is wired in series with a fixed resistor, forming a voltage divider in such a way that the voltage output fluctuates with temperature. The microcontroller's ADC peripheral evaluates the voltage as a number between zero and the ADC's maximum value. For example the ADC in an Arduino typically has a resolution of 10 bits, giving it a maximum value of 1023.

Software in the Arduino converts the ADC reading to a voltage, then applies Ohm's Law to interpret that voltage as the resistance in the thermistor. The resistance can be converted into a temperature.

The example given in the article referenced above demonstrates using a value, called "Beta", which may be published in the manufacturer's data sheet for the device. This approach may give a reasonable approximation for temperatures near 25 Celsius, 77 Fahrenheit.

There are at least two other ways to do it.

Manufacturers may publish in the product data sheet a set of measured resistance values at intervals across a range of temperatures. The values could be encoded as a lookup table. The software could locate the values immediately greater- and less-than the measured resistance, interpolating to obtain the resulting temperature.

Finally, an equation called the Steinhart&mdash;Hart Equation (SHEq), may be used to calculate the temperature directly.

Each of the methods has advantages and disadvantages.

* Beta
    * Advantage: simple to code
    * Disadvantages:
        * Not usable if beta value not known
        * Beta value is actually not constant
        * Uses floating-point operations
* Lookup
    * Advantages: 
        * May be more accurate across the device's full operating range
        * Code can avoid floating-point operations
    * Disadvantages:
        * Lookup table takes up space in memory
        * Code has to identify and interpolate relevant values in the table
        * Linear interpolation (the easiest way) may diminish accuracy somewhat
        * Not usable if values are not available, e.g., absence of the data sheet
* SHEq
    * Advantages:
        * Closer approximation across the operating range compared to the other ways
        * Straightforward, one-line calculation
        * A potential solution when Beta or Look-up Table data are not available
    * Disadvantages:
        * Requires knowing three coefficients for the thermistor being used.
        * Uses multiple, different floating-point operations

If not provided by the manufacturer, coefficients for the SHEq may be estimated from a set of three measurements of resistance at three, different temperatures taken at intervals of ten degrees Celsius or greater. A method for deriving the coefficients was described in February 2023 on a Web page at the following url of Wikipedia:

[https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation](https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation)

The SHEq can be reversed to obtain expected resistance values as a function of given temperature. The example Arduino program included in this repo, "Calculate_Expected_Resistance", demonstrates.

I wrote the program to run on an Arduino-compatible SAMD21 module. What practical use might it have? I don't know. One could adapt the output as a lookup table. 

A couple of things impressed me.

Firstly, the Arduino IDE appears to link the C or C++ math library by default, at least for the SAMD21 platform. That's a nice convenience compared to the gcc compiler, for example, where the math library must be explicitly included in the code and a flag provided to the Linker.

Secondly, the code ran really fast on that little SAMD21 chip. Floating-point computations do not seem to burden the machine very much. Based on experience with this code I plan to use the SHEq for converting thermistor readings into temperatures.

You know, we should not forget the original task given to computers was... to perform computations. A program would include the data to be analyzed along with the algorithm to be applied to it. The program would load-in, execute, print results, and stop. Programs were not interactive, and we did not require them to be so.

In that classical sense, a SAMD21-based Arduino-compatible board is truly a computer. Moreover, it's much faster and more capable compared to that first Apple ][ I bought more than 40 years ago. And it costs a whole lot less money to buy.
        