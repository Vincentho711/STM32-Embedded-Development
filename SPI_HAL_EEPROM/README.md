# STM32 EEPROM SPI with SPI_HAL (25AA080B)

This projects shows how SPI can be used to write and read data to and from an EEPROM. A STM32 is connected to the EEPROM (25AA080B), the HAL_SPI library and a Nucleo-F446re has been used in this example.


## 📦 Usage
First, locate the pinouts for SPI interface of your STM32 micro-controller. I used SPI1 and pin D10 as Chip Select, D13(PA5) will be used for our SCK line but it is configured to control the Green LED on the nucleo board by default. Therefore, we have to disable it in the `.ioc` file.

Then go to the connectivity page and enable SPI1, configure the `Mode` as "Full-Duplex Master". Leave `Hardwae NSS Signal` disabled. Under the "configuration' tab, leave `Frame Format` as "Motorola" but change `Data Size` to "8 Bits" as the 25AA080B EEPROM communicates in 1 byte chunks. `First Bit` should be set to "MSB First". Referring to the datasheet of the EEPROM, it has a max clock speed of 10MHz, so adjust the `Prescalar(for Baud Rate)` so the `Baud Rate` stays below 10MHz, I have used a prescalar of 64 here. 

Finally, for D10 to be used as Chip Select, change PB6 to "GPIO_Output" in the pinout view.

The code to interface with the EEPROM is in `25AA080B.c` and `25AA080B.h`. The funtions to read and write data to the EEPROM is located in `25AA080B.c`. All of the code have been commented thoroughly so you can understand how it works by reading the comments while referring to the [datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/21808D.pdf).


## ⚡ Circuit Diagram 
![Circuit Diagram](https://github.com/Vincentho711/STM32-Embedded-Development/blob/master/SPI_HAL_EEPROM/nucleo_SPI_25AA080B_circuit_diagram.png?raw=true)
