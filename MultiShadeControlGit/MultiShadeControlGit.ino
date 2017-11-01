/*
 *  This sketch allows up/down control via Alexa off/on for a number of Somfy shades. This one does it 
 *  over a network using a Global Cache GC-100 and an Urtsi by Somfy.  
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <fauxmoESP.h>

bool called = 0;
String action[13];

/*I currently only have 9 shades including the bathroom.  
 * The address 0116 is special.  It operates all shades.
 * Note: These arrays must be updated together and the integer
 * in the control loop where we add devices must be updated to 
 * reflect the number of items in this list.
 */
String UP[] = {"0101U", "0102U", "0103U", "0104U", "0105U", "0106U", "0107U", "0108U", "0109U", "0116U"};
String DOWN[] = {"0101D", "0102D", "0103D", "0104D", "0105D", "0106D", "0107D", "0108D", "0109D", "0116D"};
const char *AlexaWords[] = {"Nicks East", "Sitting Room", "Sitting Room Side", "First Slider", "Second Slider", "Third Slider", "First East Window", "Second East Window", "Bathroom Window", "All Master Shades"};

ESP8266WiFiMulti WiFiMulti;
fauxmoESP fauxmo;



struct State  // Struct to keep current state of device
{
  bool state = 0;
  const char *device_name;
  uint8_t device_id;
} Curstate;

void callback(uint8_t device_id, const char * device_name, bool state) {
  
  Curstate.device_name = device_name;
  Curstate.state= state;
  Curstate.device_id = device_id;
  called = 1;
  
} // Void callback
 
//First we connect to the local WIFI hotspot
void setup() {
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("SSID", "Password");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

   /*Next we add our Devices to Alexa
    * Note: When we add to the UP, DOWN or AlexaWords arrays, 
    * we must update the controling integer in the following loop
    */
   
   for (int x = 0; x < 10; x++) {
      fauxmo.addDevice(AlexaWords[x]);
    } //for
    
    fauxmo.onMessage(callback);
    
}  //setup()
    

void loop() {

    /*
     * Connect to the serial port of the GC-100 at host
     * This serial port is connected to an URTSI
     * We'll send Up commands when alexa gets 
     * the off, and Down when she gets the on
     */
    
    fauxmo.handle(); 
    if (called) {
      
      //Reset the called flag do stop from running this if 
      called = 0;
      
      // Devices id's match with the order in which devices were added
      action[0] = UP[Curstate.device_id];
      action[1] = DOWN[Curstate.device_id];
      
      //Setup the connection to Somfy via the GC-100>Urtsi link
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const uint16_t port = 4999;
      const char * host = "10.0.1.222"; // ip or dns
      
      if (!client.connect(host, port)) {
        Serial.print(host); Serial.print(" : "); Serial.println(port);
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
      }  //if !client.connect

      //Enter the Somfy command and close the tcp connection 
      Serial.println(action[Curstate.state]);     
      client.print(action[Curstate.state]);
      client.stop();

    } // if called
    
} //void loop()
    


