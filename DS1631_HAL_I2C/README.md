# STM32 with HAL_I2C (DS1631)

A project to demostrate how HAL_I2C can be used for a STM32 micro-controller to communicate with the DS1631 temperature sensor via I2C. Nucleo-F446RE has been used but it should work with other STM32 based boards as well.

## 📦 Usage

For STM32CubeIDE, navigate to Project Manager, simply copy `ds1631.c` and `main.c` into `Src` folder and `ds1631.h` into `Inc` folder. 

Remeber to setup pull-up resistors for the `SDA` and `SCL` lines, this can be done with external pull-up resistors or implemented internally by configuring `GPIO Mode and Configuration` in the `.ioc` file.

You can configure the address of DS1631 in `ds1631.h`.

`main.c` demonstrates how to setup the sensor and obtain temperature value from the sensor.

## ⚡ Circuit Diagram

![Circuit Diagram](https://github.com/Vincentho711/STM32-Embedded-Development/blob/master/DS1631_HAL_I2C/circuit_diagram.JPG?raw=true)