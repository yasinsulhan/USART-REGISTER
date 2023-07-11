# USART with REGISTER
A USART (universal synchronous/asynchronous receiver/transmitter) is hardware that enables a device to communicate using serial protocols.

A UART device can use asynchronous communication protocols. A USART device can use both asynchronous and synchronous communication protocols. Therefore, a USART can do anything a UART can do and more. Because a USART requires more complex circuitry and more communication lines to fully implement, many devices may only implement a UART to save on cost, complexity or power usage.

#### What is UART ?
In asynchronous mode, only one data line is used to send data from the transmitter to the receiver. There is no shared synchronization signal from the sender to the receiver. So, the receiver has no way to know how fast or slow the data is coming. To circumvent this, both the sender and receiver must be manually configured beforehand to use the same data rate.

#### What is USART ?
In synchronous mode, a data and a clock line are used to send the data. The controller sends a clock signal, which synchronizes the controller and the peripheral at the same data rate. Because the clock signal keeps the devices in sync, the two devices don't need to be configured ahead of time to use the same bit rate.

In STM32F4 Discovery Board, USART and UART can be used by adjusting needed register in reference manuel.

#### GPIO and Clock Configuration
Firstly, the Tx and Rx pins is placed as **alternate function**. One of these are **PB10 and PB11 (PORT B)** which are Tx and Rx pins respectively.

`GPIOB->MODER |= (2 << 20) | (2 << 22)` makes PB10 and PB11 alternate function as shown in referance manuel below

![moder](https://github.com/yasinsulhan/USART-REGISTER/assets/109728194/54109527-3fa9-4875-b952-493f86b27035)

In addition, figure 26 guides which alternate function **(AF)** must be selected. As shown, AF7 belongs to USART3 (PB10 and PB11 which are indicated in the table below). `GPIOB->AFR[1] |= (7 << 8) | (7 << 12)`

![afrh](https://github.com/yasinsulhan/USART-REGISTER/assets/109728194/6976f3a6-4f55-4e72-9853-713389ef2ad7)

![afrh2](https://github.com/yasinsulhan/USART-REGISTER/assets/109728194/e070e34a-713b-45f0-aced-5e11f71ea871)

Also, the other Tx and Rx pins are given below.

![Tx and Rx pins](https://github.com/yasinsulhan/USART-REGISTER/assets/109728194/0fd7b5c3-99b5-4fb0-b0b8-f0aff3177b3c)

As seen in the list, PB10 and PB11 belong to **USART3**. So, registers are adjusted in Atollic TrueSTUDIO according to USART3.

According to STM32F4 board block diagram, PORT B goes to **AHB1** peripheral clock and USART3 goes **APB1** peripheral clock. These peripheral clocks must be enabled as `RCC->AHB1ENR |= 0x00000002`, `RCC->APB1ENR |= (1 << 18)` in RCC registers respectively. These are in referance manuel below.

![ahb1ener](https://github.com/yasinsulhan/USART-REGISTER/assets/109728194/be3bb97f-554b-497a-b77b-f437390c01f3)

![apb1enr](https://github.com/yasinsulhan/USART-REGISTER/assets/109728194/f583dec2-2a5e-4f82-926d-c0fb9eed8072)

#### USART Configuration
Lastly, USART configuration is adjusted in USART control register acccording to receiver and transmitter following procedures.

![CR1](https://github.com/yasinsulhan/USART-REGISTER/assets/109728194/cddcd6f8-9c66-4750-8446-398c5911bb58)

##### Transmitter
•Enable the USART by writing the UE bit in USART_CR1 register to 1. `USART3->CR1 |= 1 << 13`

•Program the M bit in USART_CR1 to define the word length. `USART3->CR1 |= 1 << 12`

•Select the desired baud rate using the USART_BRR register. `USART3->BRR = 0x1112` Desired baud rate is 9600. [The link shows that how to calculate baud rate.](https://fastbitlab.com/stm32-usart-lecture-9-usart-baud-rate-calculation-part-2/)

•Set the TE bit in USART_CR1 to send an idle frame as first transmission. `USART3->CR1 |= 1 << 3`

##### Receiver
•Enable the USART by writing the UE bit in USART_CR1 register to 1.

•Program the M bit in USART_CR1 to define the word length.

•Select the desired baud rate using the baud rate register USART_BRR. 

•Set the RE bit USART_CR1. This enables the receiver which begins searching for a start bit. `USART3->CR1 |= 1 << 2`

#### Steps for Sending Characters
•***Write the data to send in the USART_DR register (this clears the TXE bit).*** Repeat this for each data to be transmitted in case of single buffer.

•After writing the last data into the USART_DR register, ***wait until TC=1. This indicates that the transmission of the last frame is complete.*** This is required for instance when the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.

`USART3->DR = data`

`while(!(USART3->SR & (1 << 6)))`

#### Steps for Receiving Characters
•The RXNE bit is set. ***It indicates that the content of the shift register is transferred to the RDR***. In other words, data has been received and can be read.

•In single buffer mode, ***clearing the RXNE bit is performed by a software read to the USART_DR register. The RXNE flag can also be cleared by writing a zero to it***. The RXNE bit must be cleared before the end of the reception of the next character to avoid an overrun error.
