# NTUSAAD RFID door lock workshop

A simple RFID door lock system based on Ardunio microcontroller.  
For `NTUEE SAAD` workshop.

* Required library:
    * [Arduino RFID Library for MFRC522](https://github.com/miguelbalboa/rfid)
    * [Arduino Library for Maxim Integrated DS3232 and DS3231 Real-Time Clocks](https://github.com/JChristensen/DS3232RTC)
    * [Time library for Arduino](https://github.com/PaulStoffregen/Time)

RFID RC522 and Arduino Uno Pin Configuration
| RFID RC522 | Arduino Uno |  
|------------|-------------|  
| SS/SDA     | D10         |  
| SCK        | D13         |
| MOSI       | D11         |
| MISO       | D12         |
| IRQ        | Not Connected|  
| GND        | GND         |
| RST        | D9          |
| 3.3V       | 3.3V        |


1602 LCD Connections
| RFID RC522 | Arduino Uno |
|------------|-------------|  
|I<sup>2</sup>C SDA  | A4  |  
|I<sup>2</sup>C SCL  | A5  |    

Others  
|    Name     | Arduino Uno |  
|-------------|-------------|  
| RED_LED   | D6  |
| GREEN_LED | D7  |
| BUZZER    | D8  | 

Reference : [RFID Based Automatic Door System](https://www.hackster.io/user8523373/rfid-based-automatic-door-system-7b2065)