README
FlightGear Interface

FlightGearInterface.java
 - used to interface to FlightGear Application
 - Accepts a command from BCI Interface code, and sends a flight command to FlightGear Application
 
FlightGearGUI.java
 - used in the beginning stages (before BCI Interface was completed)
 - There are buttons in this GUI in place of LEDs
 - Clicking a button would send a command to FlightGearInterace.java to simulate what we should get
from the BCI Interface code.
 
threads.cxx
 - this is a new file that we created, that we built into FlightGear itself
 - This accepts commands on a socket from FlightGearInterface.java
 - Received commands would translate to internal function calls in FlightGear Application
 