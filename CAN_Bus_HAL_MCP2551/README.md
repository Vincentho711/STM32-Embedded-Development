# STM32 CAN Bus with CAN_HAL (MCP2551)

This projects shows how the CAN Bus can be used to send and receive data between 2 STM32 micro-controllers. Each STM32 is connected to a CAN transceiver (MCP2551), the CAN interface is implemented using the HAL library. A Nucleo-F446re and Nucleo-F746zg have been used in this example.

## 🎭 Demo
![Demo](https://github.com/Vincentho711/STM32-Embedded-Development/blob/master/CAN_Bus_HAL_MCP2551/CAN_HAL_MCP2551_GIF.gif)

Once the button on the F446re has been pressed, a GPIO callback function will be executed which sends a Tx message containing 2 bytes of data to the CAN bus. The first byte of data contains the delay between toggles and the second byte contains the number of times the LED should toggle. The Tx data will be received by the F746zg and its LED will blink. Upon receiving the data, the F746zg will transmit 2 bytes of data back to the F446re through the CAN bus. The LED on the F446re will then blink according to the data in those 2 bytes.

## 📦 Usage
Since 2 boards have been used, we will have to set up a 2 new projects on STM32CubeIDE. One for the F446re and another one for the F746zg. Navigate to Project Manager, simply copy `F446RE_code/main.c` into the `Src` folder of the F446re project and `F746ZG_code/main.c` into `Src` folder of the F746zg project. 

Open up the `.ioc` file for the F446re project. The code for the F446re has been set up to use FIFO0 for the CAN buffer and CAN1, so make sure `CAN1 RX0 interrupt` has been enabled under "NVIC settings". Depending on the clock of your STM32, settings in the "Parameter Settings" tab of CAN1 has to be adjusted to give a `Baud Rate` of 500000 bit/s. This can be achieved by experimenting with the values of `Prescalar (for Time Quantum)`, `Time Quanta in Bit Segment 1` and `Time Quanta in Bit Segment 2`. The CAN ID for the F446re micro-controller has been set to 0x446.

Open up the `.ioc` file for the F746zg project. FIFO1 for the CAN buffer and CAN1 has been used, therefore enable `CAN1 RX1 interrupt` under "NVIC settings". Adjust the settings in "Parameter Settings" tab for CAN1 to give `Baud Rate` of 500000 bit/s. The CAN ID for the F746zg has been set to 0x103.

Both of the `main.c` have been commented thoroughly so you can understand how it works by reading the comments.

## ⚡ Circuit Diagram 
![Circuit Diagram](https://github.com/Vincentho711/STM32-Embedded-Development/blob/master/CAN_Bus_HAL_MCP2551/nucleo_CAN_mcp2551_circuit_diagram.png?raw=true)

Although both of the STM32 look like the F446re board in the circuit diagram, one of them is acutally a F746zg board in reality as I couldn't find the diagram for F746zg. Therefore, you will have to find the Rx and Tx pin of CAN1 for F746zg on its datasheet and connect it to one of the MCP2551. 