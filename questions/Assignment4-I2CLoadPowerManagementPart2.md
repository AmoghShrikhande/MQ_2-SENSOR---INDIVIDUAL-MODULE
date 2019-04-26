m.Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements in the "Default" configuration of the profiler to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See [Shared document](https://docs.google.com/document/d/1Ro9G2Nsr_ZXDhBYJ6YyF9CPivb--6UjhHRmVhDGySag/edit?usp=sharing) for instructions.* 

1. What is the average current per period?
   Answer:16.34uA
   <br>Screenshot:  
   ![Avg_current_per_period](https://github.com/CU-ECEN-5823/assignment4-lpm-part2-AmoghShrikhande/blob/master/screenshots/assignment-4/Avg_current_per_period.JPG)

2. What is the average current when the Si7021 is Load Power Management OFF?
   Answer:2.12uA
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/assignment4-lpm-part2-AmoghShrikhande/blob/master/screenshots/assignment-4/Avg_current_LPM_Off.JPG)

3. What is the average current when the Si7021 is Load Power Management ON?
   Answer:430.58
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/assignment4-lpm-part2-AmoghShrikhande/blob/master/screenshots/assignment-4/Avg_current_LPM_On.JPG)

4. How long is the Si7021 Load Power Management ON for 1 temperature reading?
   Answer:
   <br>Screenshot:  89.6ms
   ![duration_lpm_on](https://github.com/CU-ECEN-5823/assignment4-lpm-part2-AmoghShrikhande/blob/master/screenshots/assignment-4/duration_lpm_on.JPG)

5. What is the total operating time of your design for assignment 4 in hours assuming a 1000mAh supply?	
-> average current per period = 16.34uA.
-> Total operating time = 61199.5104 Hrs

6. How has the power consumption performance of your design changed since the previous assignment?
-> I observed a drop in power consumption in this assignment as compared to the previous one. The reason is that in this assignment we
   are sleeping in EM3 mode for most of the time. Beacause of this, the power consumption and the current consumption will be very less.
   Also we are using interrupts to carry out different functions instead of polling. Therefore the controller is able to sleep for a longer time

7. Describe how you have tested your code to ensure you are sleeping in EM1 mode during I2C transfers.
-> Use of SleepBlockBegin and SleepBlockEnd made me ensure that the sleep is happening in EM1 mode when the I2c transfers are going on. Also it is evident from the graph that we are sleeping in EM1 mode during I2C transfer. We could also conclude the same using breakpoint in the code