Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does a single LED draw when the output drive is set to "Strong" with the original code?**

0.48mA

**2. After commenting out the standard output drive and uncommenting "Weak" drive, how much current does a single LED draw?**

0.48mA

**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, 
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

No, both the currents are same. For strong drive the pins drive 10mA current and for the weak drive they drive 1mA. In both cases the current consumption of the circuit changes but the current consumed by the LED remains same as LED is in series with other circuitry.

**4. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with only LED1 turning on in the main loop?**

Average current: 4.8mA  Average Energy: 11.34uWh

**5. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with both LED1 and LED0 turning on in the main loop?**

Averge current: 5.13mA Average energy: 11.95uWh
