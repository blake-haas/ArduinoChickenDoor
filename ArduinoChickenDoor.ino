#include <Servo.h>
#include <avr/wdt.h>            // library for default watchdog functions
#include <avr/interrupt.h>      // library for interrupts handling
#include <avr/sleep.h>          // library for sleep
#include <avr/power.h>          // library for power control

int SERVO_PIN = 9;
int LIGHT_SENSOR_ANALOG_PIN = A0;
int LOW_LIGHT_THRESHOLD = 500; // ~1.3 lux You need to calibrate this against a lux meter, each sensor has its own resistance values.
int HIGH_LIGHT_THRESHOLD = 210; // ~24 lux
int HIGH_LIGHT_HIT_COUNT = 15; // 15 min
int LOW_LIGHT_HIT_COUNT = 20; // 20 min
int LowLightCount = 0;
int HighLightCount = 0;
int DoorState = 0; // 0 is unknown // 1 is open // 2 is closed
int pos = 25;
Servo Servo1;
int nbr_remaining; // how many times remain to sleep before wake up 

// interrupt raised by the watchdog firing
// when the watchdog fires during sleep, this function will be executed
// remember that interrupts are disabled in ISR functions
ISR(WDT_vect)
{
        // not hanging, just waiting
        // reset the watchdog
        wdt_reset();
}
// function to configure the watchdog: let it sleep 8 seconds before firing
// when firing, configure it for resuming program execution
void configure_wdt(void)
{
 
  cli();                           // disable interrupts for changing the registers

  MCUSR = 0;                       // reset status register flags

                                   // Put timer in interrupt-only mode:                                       
  WDTCSR |= 0b00011000;            // Set WDCE (5th from left) and WDE (4th from left) to enter config mode,
                                   // using bitwise OR assignment (leaves other bits unchanged).
  WDTCSR =  0b01000000 | 0b100001; // set WDIE: interrupt enabled
                                   // clr WDE: reset disabled
                                   // and set delay interval (right side of bar) to 8 seconds

  sei();                           // re-enable interrupts

  // reminder of the definitions for the time before firing
  // delay interval patterns:
  //  16 ms:     0b000000
  //  500 ms:    0b000101
  //  1 second:  0b000110
  //  2 seconds: 0b000111
  //  4 seconds: 0b100000
  //  8 seconds: 0b100001
 
}
// Put the Arduino to deep sleep. Only an interrupt can wake it up.
void sleep(int ncycles)
{  
  nbr_remaining = ncycles; // defines how many cycles should sleep

  // Set sleep to full power down.  Only external interrupts or
  // the watchdog timer can wake the CPU!
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 
  // Turn off the ADC while asleep.
  power_adc_disable();
 
  while (nbr_remaining > 0){ // while some cycles left, sleep!

  // Enable sleep and enter sleep mode.
  sleep_mode();

  // CPU is now asleep and program execution completely halts!
  // Once awake, execution will resume at this point if the
  // watchdog is configured for resume rather than restart
 
  // When awake, disable sleep mode
  sleep_disable();
  
  // we have slept one time more
  nbr_remaining = nbr_remaining - 1;
 
  }
 
  // put everything on again
  power_all_enable();
 
}
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  //while (! Serial) {}  // wait for serial monitor to attach
  // Initialize the door open.
  openDoor();
  delay(20);
  // configure the watchdog
  configure_wdt();
}

void loop() {
  int16_t currentLightLevel = analogRead(LIGHT_SENSOR_ANALOG_PIN);
  setLightLevel(currentLightLevel);
  if (HighLightCount == HIGH_LIGHT_HIT_COUNT && DoorState != 1) {
//    Serial.print("HighLightCount: ");
//    Serial.print(HighLightCount);
//    Serial.println("");
//    Serial.print("LowLightCount: ");
//    Serial.print(LowLightCount);
//    Serial.println("");
//    Serial.println("opening door!");
//    Serial.print("Previous DoorState: ");
//    Serial.print(DoorState);
//    Serial.println("");
    // Open the Door
    openDoor();
  }
  if (LowLightCount == LOW_LIGHT_HIT_COUNT && DoorState != 2) {
//    Serial.print("HighLightCount: ");
//    Serial.print(HighLightCount);
//    Serial.println("");
//    Serial.print("LowLightCount: ");
//    Serial.print(LowLightCount);
//    Serial.println("");
//    Serial.println("closing door!");
//    Serial.print("Previous DoorState: ");
//    Serial.print(DoorState);
//    Serial.println("");
    // Close the Door
    closeDoor();
  }
  sleep(8); // sleep for 1 min..
}
void setLightLevel(int16_t level) {
  if (level >= LOW_LIGHT_THRESHOLD) {
    // We got a low light value, RESET the high count
    HighLightCount = 0;
    LowLightCount = LowLightCount + 1;
    if (LowLightCount < LOW_LIGHT_HIT_COUNT) {
      // stop incrementing
      LowLightCount = LowLightCount + 1;
    }
  } else if (level <= HIGH_LIGHT_THRESHOLD) {
    // We got a high light value, RESET the low count
    LowLightCount = 0;
    if (HighLightCount < HIGH_LIGHT_HIT_COUNT) {
      // stop incrementing
      HighLightCount = HighLightCount + 1;
    }
  }
//  Serial.print("Lux: ");
//  Serial.print(level);
//  Serial.println("");
}
void openDoor() {
  Servo1.attach(SERVO_PIN, 500, 2500);
  for (pos = 160; pos >= 25; pos -= 1) {
    // goes from 160 degrees to 25 degrees
    // in steps of 1 degree
    Servo1.write(pos);
    delay(50);
  }
  DoorState = 1;
  Servo1.detach();
  delay(7000); // Wait for servo to finish movements
}

void closeDoor() {
  Servo1.attach(SERVO_PIN, 500, 2500);
  for (pos = 25; pos <= 160; pos += 1) {
    // goes from 25 degrees to 160 degrees
    Servo1.write(pos);
    delay(50);
  }
  DoorState = 2;
  Servo1.detach();
  delay(7000); // Wait for servo to finish movements
}
