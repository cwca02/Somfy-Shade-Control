# Somfy-Shade-Control
Somfy Shade Control via GC-100 to Urtsi
This is an Arduino.ino that issues up and down commands to Somfy 
motorized RTS shades to raise and lower as Off and On commands from Alexa.
Libraries used: fauxmoESP and ESP8266WiFiMulti
Many thanks to Xose Pérez for fauxmoESP https://bitbucket.org/xoseperez/fauxmoesp
and to Jerry Olsen for creating the instructable that got me started.  Go here to 
see what he did.  Very clever use of a Somfy remote and Arduio board to control 
shades.  If you do it this way, you do not need a GC-anything or an Urtsi, so, 
big cost savings there.  You can see Jerry's instructable here: 

https://www.instructables.com/id/How-to-Connect-Your-Somfy-Remote-to-Alexa-With-an-/


Hardware used: Arduino ESP8266 board, Global Cache GC-100 and Somfy Urtsi device

It works by naming shades for Alexa through fauxmoESP, and associating their UP/DOWN
commands for Somfy's Urtsi controller.  I'm not in love with the way I did this but 
it was the easiest to code without tinkering with multi-dimensional string arrays.  I may
go back and do that.  For now, it's positional, which isn't great but asl long as you
modify the UP, DOWN and AlexaWords arrays to coincide, it works.

It connects to your wifi network using ESP8266WiFiMulti.  You will need to update 
WiFiMulti.addAP("SSID", "Password"); to be your SSID and password.

It also uses the WiFIClient library from ESP8266WiFiMulti to connect to a GC-100.  You'll need
to modify this part of the sketch as well, to match your config.  The Urtsi is connected to the GC-100 via serial
port so all you need is the port number for your GC-100 serial port and the ip address
of your GC-100.  It starts at 4999 for serial 1 and goes up incrementally from there.  You can 
use any of the GlobalCache devices that allow serial control for this.  I just happen to have
an older GC-100.

