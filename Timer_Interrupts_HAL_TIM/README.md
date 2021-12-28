# Timer Interrupts with HAL_TIM

This projects builds on top of [DS1631_HAL_I2C](https://github.com/Vincentho711/STM32-Embedded-Development/tree/master/DS1631_HAL_I2C). An interrupt timer has been set up to poll temperature data from the DS1631 at a set period. This is achived using a callback function so it is a non-blocking method as it doesn't require any delay functions in the main while loop. Therefore, the micro-controller can execute other functions in the while loop without being interfered by the polling of temperature data. Nucleo-F446RE has been used but it should work with other STM32 based boards as well.

## 📦 Usage

Set up a new project on STM32CubeIDE. Navigate to Project Manager, simply copy `ds1631.c` and `main.c` into `Src` folder and `ds1631.h` into `Inc` folder. 

Under the "Timers and watchdogs" section of the STM32 datasheet, find a general purpose timer. I used TIM10 on my STM32F446. 

Open STM32CubeIDE, under "Pinout & Configuration" of the `.ioc` file, navigate to the chosen timer, tick `Activated` checkbox, leave `Channel1` to `Disable`. In the "Parameter Settings" tab, set `Prescalar (PSC-16 bits value)` to set the frequency of the timer clock, my CPU clock runs at 84MHz, I want a timer clock that runs at 10,000 Hz, so I set the prescalar to (8400 - 1). The minus one is important here because a PSC value of 0 means a prescalar of 1, so a PSC value of (8400 - 1) gives a prescalar to 8400.

The counter period will be our interrupt period (note that the -1 rule applies here as well): 

<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\inline&space;\text{Counter&space;Period}&space;=&space;[\text&space;{&space;Desired&space;int.&space;period(s)&space;$\times$&space;(CPU&space;clk&space;speed(MHz)/Prescalar)]&space;-&space;1}" title="\inline \text{Counter Period} = [\text { Desired int. period(s) $\times$ (CPU clk speed(MHz)/Prescalar)] - 1}" />

<img src="https://latex.codecogs.com/gif.latex?\text{Counter Period} = [\text { Desired int. period(s) $\times$ (CPU clk speed(MHz)/Prescalar)] - 1 } " />

E.g. If we want 1s interrupts and we have 84Mhz CPU clock and prescalar = 8400: 

<img src="https://latex.codecogs.com/png.image?\dpi{110}&space;\inline&space;\text{Counter&space;Period}&space;=&space;&space;[1&space;\times&space;(\frac{84\times10^{6}}{8400})]&space;-1&space;=&space;10000&space;-&space;1&space;=&space;9999&space;" title="\inline \text{Counter Period} =  [1 \times (\frac{84\times10^{6}}{8400})] -1 = 10000 - 1 = 9999 " />

<img src="https://latex.codecogs.com/gif.latex?\text{Counter Period} =  [1 \times (\frac{84\times10^{6}}{8400})] -1 = 10000 - 1 = 9999 " />


Remeber to enable `TIM1 update interrupt TIM10 global interrupt` under the "NVIC Settings" tab.

Save and generate code.

The `void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)`in `main.c` is the code that gets run when the interrupt time resets. You can change the content in there to your own code. 

## ⚡ Circuit Diagram
The circuit diagram is the same as the one used in [DS1631_HAL_I2C](https://github.com/Vincentho711/STM32-Embedded-Development/tree/master/DS1631_HAL_I2C). 
![Circuit Diagram](https://github.com/Vincentho711/STM32-Embedded-Development/blob/master/Timer_Interrupts_HAL_TIM/circuit_diagram.JPG?raw=true)