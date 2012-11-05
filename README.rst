###########################################################
Brain Control Interface for the Flightgear Flight Simulator
###########################################################

This was my senior capstone project at Northeastern University in 2011.  It is
a an EEG based BCI used to control the Flightgear flight simulator. This repo
includes the C++ driver for the gTec g.UsbAmp that we used, the BCI 
application, which uses LEDs flashing m-sequences to determine user input.  The
application provides outputs based on the sequence the user is focusing on.
As it exists now, the outputs control flightgear over a network connection, but
the outputs could be abstracted to control anything, really.


We hacked some listener threads into flightgear that respond to the commands.
(at the time we did this thing) the documentation for the RPC interface was
sub-optimal so we just rolled our own.


The code works but needs to be cleaned up if anyone wants to actually do
anything with it. The documentation --if there is any in here-- is horrible to 
non-existant-like-i-mean-a-not-fucking-there-at-all-sort-of-mediocre-develper's-job-security-tactic-deal. 
I'm putting this here so I have access to it and know whereit is.


BioAmp
------
User space driver for the g.tec g.UsbAmp EEG amplifier that we used. This is 
reverse engineered from the Matlab drivers as I understand it (I didn't have
anything to do with the creation of this), and works for the most part but
has some issues, like sometimes giving some bunk data... (like seriously, 
this thing has one job and can't even get that done right, what gives?). If
I still had access to this silly thing I'd fix the driver, but I don't. Sorry.

BCI Interface
-------------
This is the main application that does all the processing and training and
command sending. Basically it creates training files and then uses those to
do some matched-filter detectoring and outputs commands.

FlightGear Interface
--------------------

This is the stuff that goes in Flightgear to make it work.


