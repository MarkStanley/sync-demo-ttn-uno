# sync-demo-ttn-uno - SYNC demo Arduino code
      April 2021 - Mark Stanley, https://thingitude.com
      You will need:
      *Things UNO
      *Grove Starter kit
      1. Plug the Grove base shield into the Things UNO
      2. Plug the light sensor into port A0
      3. Plug the buzzer into port D3
      4. Create an application on The Things Network console
      5. Register your Things UNO as a device for that application
      6. Change the APP EUI and the AppKey in the sketch to match the settings for your device

That should get you sending data to TTN v3
If you want to decode the data into something meaningful like {light: 123} then you'll need to add this
as an Uplink decoder function in the TTN v3 console for your application:

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

TEST
