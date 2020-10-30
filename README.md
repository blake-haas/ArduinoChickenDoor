# ArduinoChickenDoor
Automatic door for chicken coop (eglu cube) | Can be adapted to work with any coop.
Designed for very low power usage.

This is a project for an automatic chicken door based on light levels.
Door closes at around 19 lux and opens after 50 lux.

Parts list:
1. Arduino Uno / Mini / micro
2. SunFounder 20KG Servo Motor Waterproof High Torque Servo, SF3218MG Metal Gear Digital Servo
3. LM393 Digital Light Intensity Photosensitive Sensor Module for Arduino UNO
4. Power source 9v / 12v. For me it was a 12v tractor battery with a LM2596s Buck Converter tuned to 9v. 
5. Wires and a clear container to mount the light sensing hardware (mayo jar for me).
6. A 3d printer to print my designed parts for the eglu cube or your own diy solution for your coop.
7. Misc M2, M3 and M4 bolts for my designed parts (Not Necessary if using ServoCoverBody_ClosedTop).
8. Silicone / Gasket maker to seal the servo body if using my designed parts (Not Necessary if using ServoCoverBody_ClosedTop).

This has lasted over 2 seasons and is doing great!
Enancements after 2 seasons of use:

Wire in a 5vdc converter to have a solid 5v off of the main 9v power vs having the arduino try to handle the load (my uno handles it but just barely).

I also wired up some 3 Pin Way Car Waterproof Wire Connector Plug Auto Electrical Wire Connectors so that i can easily disconnect the system if i needed
to troubleshoot anything.

I added some DIY Battery Pack PVC Heat Shrink Wrap Tube 180±3MM to the jar on the bottom third to act like a skirt to allow water to flow AWAY
from the jar lid. This keeps the entire inside of the jar bone dry while still allowing me to unscrew the lid for access.

Im located in minnesota. I upgraded my power source (battery) to a full 12v car battery to power this thing and i use a 30w solar panel to trickle charge it all day.
It works flawlessly this way all summer. In the winter i have a trickle charger plugged in via extension cord to keep it topped off.

Lastly, i covered the uno and electronics in some clear nail polish to protect from moisture (just in case).