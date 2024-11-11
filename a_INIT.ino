//////////////////////////**/>- INIT -<\** /////////////////////////////

#include "Wire.h"  // This library allows you to communicate with I2C devices.
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ImgData.h"  // This includes BMP Data
#include "pitches.h" // Buzzer

#define DEBUG 1  // 1 ==> Enables Serial prints
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else  //Replace with nothing
#define debug(x)
#define debugln(x)
#endif

#define analogWrite ledcWrite

// Define button pins
#define Button_Pin_1 4
#define Button_Pin_2 16
#define Button_Pin_3 17
#define Button_Pin_4 5

#define EEPROM_ADDRESS 0x50

// Number of sensors to use
#define IR 8

// OLED Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define constants for the OLED address
const int OLED_ADDR = 0x3C;  //*****************if does not work change to 0x3C or 0x3D

// Define if the dispaly is B/W inverted
bool inverted = false;

// Variable for line sensors with IC multiplexer 4051
int Sensor_ADC_0[IR];
bool bit_Sensor[IR];
byte bin_Sensor = 0b00000000;

int HighIRValue = 4095;                           //4095
int LowIRValue = 1350;                            //800//0
int MidIRValue = (HighIRValue + LowIRValue) / 2;  //Avg=(Max+Min)/2

const int selectPins[3] = { 27, 14, 12 };  // --> A->27, B->14, C->12
const int AN_Input = 35;                   //--> Connect common (X) to A0 (analog input)

// Kp, and Kd parameters
float Kp = 19.0f;  //20.0 //P term
float Kd = 13.0f;  //14.0 //D term

float PID = 0.0f;  // PID output

float error = 0.0f, prevError = 0.0f;

// L298N motor driver  // ALL PINS MUST BE PWM
const int Motor_L = 19;      //--> 19->IN4 (To adjust the direction of the left motor rotation)
const int Motor_L_PWM = 18;  //--> 18->IN3 (To set the left motor PWM value)
const int Motor_R_PWM = 25;  //--> 25->IN2 (To set the right motor PWM value)
const int Motor_R = 26;      //--> 26->IN1 (To adjust the direction of the right motor rotation)

// setting PWM properties
const int resolution = 8;  //8
const int freq = 5000;     //5000
const int M_L_Channel_0 = 15; //channel 0 is used by tone()
const int M_L_Channel_1 = 14;
const int M_R_Channel_2 = 13;
const int M_R_Channel_3 = 12;

int PWM_R = 0, PWM_L = 0;
bool oneDirection = false;  // Sets motor to turn in one direction (not in reverse of each other when turning)
bool onOff = false;         // Trun on/off wheels
const int overshoot = 6;    // It correspond to error value in which motors start spining in opsite direction from each other to achieve quicker turns

// These values will be loaded from EEPROM
int PWM_Speed = 180;           //Base speed : 180
int MAX_SP = 255;              //Max speed : 255
int MIN_SP = 0;                //Min speed : 0
int revSpeed = PWM_Speed / 1.5;  //Reverse speed : 120

int track_menu_item = 0;
bool subMenu = false;
bool ModeUI = true;
bool twoOptions = false;

int debounce = 120;

const int Vbat = 32;
float V = 0.0f;
int percentV = 0;

const int buzzer_pin = 33;
const int buzzer_volume = 850; //0-16384
bool rgb_tone_toggle = true;
const int BUZZER_CHANNEL = 11;

// notes in the melody:
int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

const int R_PIN = 15, G_PIN = 2, B_PIN = 0;
const int LEDC_CHANNEL_R = 4;
const int LEDC_CHANNEL_G = 5;
const int LEDC_CHANNEL_B = 6;

byte dataAddr = 0x00;  // Address in the EEPROM where the data is stored
byte dataVal = 0x00;  // Current value of the data
byte valRead = 0x00;  // Value of the data read from the EEPROM

// Function prototypes
void initialize_display(void);

//////////////////////////**/>- INIT -<\** /////////////////////////////
