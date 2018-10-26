//Include sections
#include <MFRC522.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"
#include <DS3232RTC.h>

#define MAX_MSG_LEN 20

//Define Component to Arduino Pins
#define SS_PIN 10
#define RST_PIN 9
#define RELAY_PIN 4
#define Red_LED 6
#define Green_LED 7
#define BUZZER_PIN 8

uint32_t last = 0;

String card_uid;


//initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN );

//Unique ID of RFID Tag, which you want to give access.
int My_RFID_Tag[5] = {0xD4, 0x83, 0x34, 0x5B, 0x00};

//variable to hold your Access_card
boolean My_Card = false;

// notes in the melody, taken from:
//File -> Examples -> Digital -> ToneMelody
//design your own melody
int allow_melody[] = {
    /********TODO*********/
    //example:NOTE_A3, NOTE_A3
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int allow_noteDurations[] = {
    /********TODO*********/ 
    //example: 4, 4
    
};

int deny_melody[] = {
    /********TODO*********/ 
    
};

int deny_noteDurations[] = {
    /********TODO*********/ 
  
};

void setup()
{
    // put your setup code here, to run once:
    
    /********TODO*********/
    //set the pins as an input/output

    
    //open the serial port at 9600 baudrate.
    Serial.begin(9600);
    //Initialise the LCD to 16x2 Character Format
    lcd.begin();
    //lcd.nobacklight();
    lcd.noBacklight();
    //Initialize RFID
    SPI.begin();
    rfid.PCD_Init();    // Init MFRC522

    setSyncProvider(RTC.get);
}

void loop()
{
    // put your main code here, to run repeatedly:
    
    //First Assume detected card(Or tag) is My_Card,
    //Then later we will check is it My_Card or not!
    My_Card = true;
   
    //Update time 
    uint32_t now = millis();
    if (now - last >= 10000)
    {
        print_welcome_msg();
        last = now;
    }

    //Check if any RFID Tags Detected or not?
    if ( rfid.PICC_IsNewCardPresent() )
    {
        lcd.backlight();
        //if RFID Tag is detected, check for the Unique ID,
        //and print it on the Serial Window
        if ( rfid.PICC_ReadCardSerial() )
        {
            lcd.clear();
            /********TODO*********/
            //Print "Card UID:" on LCD
            
            delay(500);
            lcd.setCursor(0, 1); //Set LCD Cursor to Second Row, First Character

            //Unique id is 5 Digit Number.
            //Printing in HEX for better Understanding
            for ( int i = 0; i < 5; i++ )
            {
                char tmp_uid[MAX_MSG_LEN];
                sprintf(tmp_uid, "%02X", rfid.uid.uidByte[i]);
                card_uid += String(tmp_uid) + " ";
            }

            /********TODO*********/
            //Print uid  on LCD and Serial then clear card_uid
           


            delay(200);

            //Compare this RFID Tag Unique ID with your My_RFID_Tag's Unique ID
            for (int i = 0; i < 5; i++)
            {
                //if any one Unique ID Digit is not matching,
                //then make My_Card = false and come out from loop
                //No need to check all the digit!
                if ( My_RFID_Tag[i] != rfid.uid.uidByte[i] )
                {
                    My_Card = false;
                    break;
                }
            }
           

            //If RFID Tag is My_Card then give access to enter into room
            //else dont open the door.
            if (My_Card)
            {
                print_idf_success();
               
                /********TODO*********/
                //Turn on the Green LED as an indication of permission is given
                //to access the room. And open the door.
               
                
                //BUZZER_PIN Config, taken from:
                //File -> Examples -> Digital -> ToneMelody
                // iterate over the notes of the melody:
                play_melody(allow_melody, allow_noteDurations, sizeof(allow_noteDurations) / sizeof(int), BUZZER_PIN);
                delay(500);
                
                digitalWrite(RELAY_PIN, HIGH);

                lcd.clear();

                //Give 3 Sec delay to enter into room
                //After that door will again closed!
                //print "Door will close" in first line of LCD
                //print in i Sec. in the second line.
                for (int i = 3; i > 0; i--)
                {
                    /********TODO*********/
                    
                    delay(1000);
                    lcd.clear();
                }

                
                /********TODO*********/
                //Now,Close the door and turn off the Green LED .
                
                delay(200);
                lcd.clear();
                lcd.print("Door closed");
                delay(1000);
                print_welcome_msg();
            }

            // If RFID Tag is not My_Card then
            // Do not open the Door and
            //Turn-On Red LED and BUZZER_PIN as an indication of Warning:
            else
            {
                print_idf_denied();
                play_melody(deny_melody, deny_noteDurations, sizeof(deny_noteDurations) / sizeof(int), BUZZER_PIN);
                
                //Turn-On and Turn-off Red LED and BUZZER_PIN (with delay())
                
                for (int i = 0; i < 3; i++)
                {
                   /********TODO*********/

                   
                }
                print_welcome_msg();
            }
        }
        lcd.noBacklight();
    }
    //Put RFID Reader into Halt, untill it not detects any RFID Tag.
    rfid.PICC_HaltA();
}


void print_welcome_msg(void)
{
    /********TODO*********/
    //print your weolcom message and print time on second line on LCD
    lcd.clear();

    
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
    /********TODO*********/
    //serial print and print success message on lcd 
    
}

void print_idf_denied()
{
    /********TODO*********/
    //serial print and print success message on lcd 
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
