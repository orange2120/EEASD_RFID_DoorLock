/*
 * ***********************************************************
 *  FileName        [ RFID_DOORLOCK.ino ]
 *  PackageName     [ RFID_DOORLOCK ]
 *  Synopsis        [ RFID doorlock system based on arduino ]
 *  Author          [ NTUEESAAD ]
 *  Original author [ Robodia Technology ]
 *  Copyright       [ Copyleft(c) EEASD, GIEE, NTU, Taiwan ]
 * ***********************************************************
 * 
 *  RFID RC522 and Arduino Uno Pin Configuration
 *
 *  RFID RC522          Arduino Uno
 *  SS/SDA              D10
 *  SCK                 D13
 *  MOSI                D11
 *  MISO                D12
 *  IRQ                 Not Connected
 *  GND                 GND
 *  RST                 D9
 *  3.3V                3.3V
 *
 *  LCD(16x2) Connections
 *  I2C SDA A4
 *      SCL A5
 *
 *  Other Connections
 *  RED_LED    D6
 *  GREEN_LED  D7
 *  BUZZER_PIN D8
 * 
 *  Reference : https://www.hackster.io/user8523373/rfid-based-automatic-door-system-7b2065
*/

//Include sections
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <DS3232RTC.h>
#include "pitches.h"

#define MAX_MSG_LEN 20

//Define Component to Arduino Pins
#define SS_PIN 10
#define RST_PIN 9

#define RELAY_PIN 4
#define RED_LED 6
#define GREEN_LED 7
#define BUZZER_PIN 8

uint32_t last = 0;

String card_uid;

//initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo DoorLock;
MFRC522 rfid(SS_PIN, RST_PIN);

//Unique ID of RFID Tag, which you want to give access.
int My_RFID_Tag[5] = {0x90, 0x78, 0x36, 0x0F, 0x00};

//variable to hold your Access_card
boolean Valid_Card = false;

int allow_melody[] = {NOTE_C8, NOTE_C8, NOTE_C8, NOTE_C8};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int allow_noteDurations[] = {4, 4, 4, 4};

int deny_melody[] = {NOTE_A3, NOTE_A3};
int deny_noteDurations[] = {4, 4};

void setup()
{
    //set the pins as an input/output
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    //open the serial port at 9600 baudrate.
    Serial.begin(9600);

    //Initialise the LCD to 16x2 Character Format
    lcd.init();
    lcd.backlight();
    //lcd.noBacklight();

    //Initialize RFID module
    SPI.begin();
    rfid.PCD_Init(); // Init MFRC522

    //Get real time from DS3231 module
    setSyncProvider(RTC.get);
}

void loop()
{
    //First Assume detected card(Or tag) is Valid_Card,
    //Then later we will check is it Valid_Card or not!
    Valid_Card = true;
    DoorLock.write(0); //Servo at 0 Position, Door is Closed.

    //Update display information
    uint32_t now = millis();
    if (now - last >= 10000)
    {
        print_welcome_msg();
        last = now;
    }

    //Serial.println("Alive");

    //Check if any RFID Tags Detected or not?
    if (rfid.PICC_IsNewCardPresent())
    {
        lcd.backlight();
        //if RFID Tag is detected, check for the Unique ID,
        //and print it on the Serial Window
        if (rfid.PICC_ReadCardSerial())
        {
            lcd.clear();
            lcd.print("Card UID:");
            delay(500);
            lcd.setCursor(0, 1); //Set LCD Cursor to Second Row, First Character

            //Unique id is 5 Digit Number.
            //Printing in HEX for better Understanding
            for (int i = 0; i < 5; i++)
            {
                char tmp_uid[MAX_MSG_LEN];
                sprintf(tmp_uid, "%02X", rfid.uid.uidByte[i]);
                card_uid += String(tmp_uid) + " ";
            }
            Serial.println(card_uid);
            lcd.print(card_uid);

            card_uid = "";

            delay(200);

            //Compare this RFID Tag Unique ID with your My_RFID_Tag's Unique ID
            for (int i = 0; i < 5; i++)
            {
                //if any one Unique ID Digit is not matching,
                //then make isValid_Card = false and come out from loop
                //No need to check all the digit!
                if (My_RFID_Tag[i] != rfid.uid.uidByte[i])
                {
                    Valid_Card = false;
                    break;
                }
            }

            //If RFID Tag is Valid_Card then give access to enter into room
            //else dont open the door.
            if (Valid_Card)
            {
                print_idf_success();
                //delay(500);

                //Turn on the Green LED as an indication of permission is given
                //to access the room.
                digitalWrite(GREEN_LED, HIGH);

                digitalWrite(RELAY_PIN, HIGH);
                play_melody(allow_melody, allow_noteDurations, sizeof(allow_noteDurations) / sizeof(int), BUZZER_PIN);
                delay(500);

                // open the lock
                digitalWrite(RELAY_PIN, HIGH);

                lcd.clear();

                //Give 3 Sec delay to enter into room
                //After that door will again closed!
                for (int i = 3; i > 0; i--)
                {
                    lcd.print("Door will close");
                    lcd.setCursor(0, 1);
                    lcd.print("in ");
                    lcd.print(i);
                    lcd.print(" Sec.");
                    delay(1000);
                    lcd.clear();
                }

                //Now,Door is closed and Green LED is Turned-Off.
                digitalWrite(RELAY_PIN, LOW);
                digitalWrite(GREEN_LED, LOW);
                delay(200);
                lcd.clear();
                lcd.print("Door closed");
                //lcd.setCursor(0,1);
                //lcd.print("Now!");
                delay(1000);
                print_welcome_msg();
            }

            // If RFID Tag is not Valid_Card then
            // Do not open the Door and
            //Turn-On Red LED and BUZZER_PIN as an indication of Warning:
            //Somebody else is trying to enter into your room.
            else
            {
                print_idf_denied();
                play_melody(deny_melody, deny_noteDurations, sizeof(deny_noteDurations) / sizeof(int), BUZZER_PIN);
                for (int i = 0; i < 3; i++)
                {
                    digitalWrite(BUZZER_PIN, HIGH);
                    digitalWrite(RED_LED, HIGH);
                    delay(200);
                    digitalWrite(BUZZER_PIN, LOW);
                    digitalWrite(RED_LED, LOW);
                    delay(200);
                }
                print_welcome_msg();
            }
        }
    }
    //Put RFID Reader into Halt, untill it not detects any RFID Tag.
    rfid.PICC_HaltA();
}

void print_welcome_msg(void)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready");
    lcd.setCursor(0, 1);
    lcd.print(get_time());
}

String get_time(void)
{
    char date[MAX_MSG_LEN];
    char time[MAX_MSG_LEN];
    sprintf(date, "%d/%d/%d", year(), month(), day());
    sprintf(time, "%02d:%02d", hour(), minute());
    return (String)date + " " + time;
}

void print_idf_success()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Permission");
    lcd.setCursor(0, 1);
    lcd.print("Allowed!");
}

void print_idf_denied()
{
    lcd.clear();
    lcd.print("Permission");
    lcd.setCursor(0, 1);
    lcd.print("Denied!");
}

void play_melody(int melody[], int noteDurations[], int thisNotes, int pin)
{
    for (int thisNote = 0; thisNote < thisNotes; thisNote++)
    {
        Serial.print(melody[thisNote]);
        Serial.print(',');
        Serial.println(noteDurations[thisNote]);
        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(pin, melody[thisNote], noteDuration);
        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(pin);
    }
}