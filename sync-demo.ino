/*    SYNC demo Arduino code
      Mark Stanley, https://thingitude.com

      You will need:
      Things UNO
      Grove Starter kit
      Plug the Grove base shield into the Things UNO
      Plug the light sensor into port A0
      Plug the buzzer into port D3

      Create an application on The Things Network console
      Register your Things UNO as a device for that application
      Change the APP EUI and the AppKey below to match the settings for your device
 */

 // Payload decoder for v3 TTN  (copy and paste the code below)
 /*
  function decodeUplink(input) {
  lightValue=input.bytes[0]*256;
  lightValue+=input.bytes[1];
  return {
    data: {
      bytes: input.bytes,
      light: lightValue
    },
    warnings: [],
    errors: []
  };
}
  */

#include <TheThingsNetwork.h>

// Set your AppEUI and  AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

// For light sensor
const int pinLight = A0;
//For buzzer
const int buzzerPin=3;

// Defines the light-sensor threshold value below which the LED will turn on.
// Decrease this value to make the device more sensitive to ambient light, or vice-versa.
int thresholdvalue = 50;

void setup()
{
  pinMode(LED_BUILTIN, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  // Set callback for incoming messages
  ttn.onMessage(message);

}

void loop()
{
    byte *bytes;
    byte payload[2];
    // Read the value of the light sensor. The light sensor is an analog sensor.
    int lightValue = analogRead(pinLight);
    debugSerial.println("LightValue is "+String(lightValue));

    // Turn the LED on if the sensor value is below the threshold.
    if(lightValue < thresholdvalue)
    {
        debugSerial.println("It is dark - send an alert!");
        // Encode data
        bytes=(byte *)&lightValue;
        payload[0]=bytes[1];
        payload[1]=bytes[0];        

      
        ttn.sendBytes(payload, sizeof(payload), 100);

        //wait for 10 secs
        delay(10000);
    }
    else
    {
        debugSerial.print("It is still light");
        delay(5000);
    }
}

void message(const uint8_t *payload, size_t size, port_t port)
{
  debugSerial.println("-- MESSAGE");
  debugSerial.print("Received " + String(size) + " bytes on port " + String(port) + ":");

  for (int i = 0; i < size; i++)
  {
    debugSerial.print(" " + String(payload[i]));
    digitalWrite(buzzerPin, HIGH);
    delay(2000);
    digitalWrite(buzzerPin, LOW);
  }

  debugSerial.println();
}
