
// Include Libraries
#include "Arduino.h"
#include "../lib/HX711.h"
#include "../lib/StepperMotor.h"


// Pin Definitions
#define SCALE_PIN_DAT	2
#define SCALE_PIN_CLK	0
#define STEPPERGEARED_PIN_STEP	5
#define STEPPERGEARED_PIN_DIR	4



// Global variables and defines
#define stepperGearedDelayTime  1000
// object initialization
HX711 scale(SCALE_PIN_DAT, SCALE_PIN_CLK);
#define calibration_factor 2280 //This value is obtained using the SparkFun_HX711_Calibration sketch https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide?_ga=2.77038550.2126325781.1526891300-303225217.1493631967
StepperMotor stepperGeared(STEPPERGEARED_PIN_STEP,STEPPERGEARED_PIN_DIR);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    scale.set_scale(calibration_factor); 
    scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
    // enable the stepper motor, use .disable() to disable the motor
    stepperGeared.enable();
    // set stepper motor speed by changing the delay value, the higher the delay the slower the motor will turn
    stepperGeared.setStepDelay(stepperGearedDelayTime);
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // SparkFun HX711 - Load Cell Amplifier - Test Code
    float scaleUnits = scale.get_units(); //scale.get_units() returns a float
    Serial.print(scaleUnits); //You can change this to lbs but you'll need to refactor the calibration_factor
    Serial.println(" Kg"); //You can change this to lbs but you'll need to refactor the calibration_factor
    }
    else if(menuOption == '2') {
    // Small Reduction Stepper Motor with EasyDriver - 5VDC 32-Step 1/16 Gearing - Test Code
    // the higher the time value the slower the motor will run
    stepperGeared.step(1, 1000);  // move motor 1000 steps in one direction
    delay(1000);            // short stop
    stepperGeared.step(0, 1000);  // move motor 1000 steps in the other dirction
    delay(1000);            //short stop
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) SparkFun HX711 - Load Cell Amplifier"));
    Serial.println(F("(2) Small Reduction Stepper Motor with EasyDriver - 5VDC 32-Step 1/16 Gearing"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
              Serial.println(F("Now Testing SparkFun HX711 - Load Cell Amplifier"));
            else if(c == '2') 
              Serial.println(F("Now Testing Small Reduction Stepper Motor with EasyDriver - 5VDC 32-Step 1/16 Gearing"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}