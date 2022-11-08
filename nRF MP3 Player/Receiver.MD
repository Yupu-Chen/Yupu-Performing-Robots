#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer =  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// Pin usage
  //
  // The LEDs are on pins 2 and 4:
  // nRF24L01 uses SPI which is fixed on pins 11, 12, and 13.
  // It also requires two other signals
  // (CE = Chip Enable, CSN = Chip Select Not)
  // Which can be any pins:
  const int CEPIN = 9;
  const int CSNPIN = 10;
  // In summary,
  // nRF 24L01 pin    Arduino pin   name
  //          1                     GND
  //          2                     3.3V
  //          3             9       CE
  //          4             10      CSN
  //          5             13      SCLK
  //          6             11      MOSI/COPI
  //          7             12      MISO/CIPO

RF24 radio(CEPIN, CSNPIN);                // CE, CSN
  // Byte of array representing the address.
  // This is the address where we will send the data.
  // This should be same on the receiving side.
const byte address[6] = "00200";
void setup() {
  Serial.begin(9600);
  // RF24 setup
  if (!radio.begin()) {
    Serial.println("radio  initialization failed");
    while (1)
        ;
  } else {
    Serial.println("radio successfully initialized");
  }
  radio.openReadingPipe(0,address);  //destination addres
  radio.setPALevel(RF24_PA_MIN);   // min or max
  radio.startListening();           //This sets the module as transmitter

  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  musicPlayer.sineTest(0x44, 500);
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
    }
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(30,30);
  
  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT))
  {
    Serial.println(F("DREQ pin is not an interrupt pin"));
}
}


  void loop() {
  uint8_t pipeNum;

  if (radio.available(&pipeNum))  //Looking for the data.
  {
    int data;
    radio.read(&data, sizeof(data));  //Reading the data
    if (data == 1)
    {
      musicPlayer.playFullFile("track001.ogg");

      if (! musicPlayer.startPlayingFile("/track001.mp3")) {
        Serial.println("Could not open file track001.mp3");
      }else{
        Serial.println(F("Started playing"));
      }
      

    }

    if (data == 2)
    {
      musicPlayer.playFullFile("track002.ogg");
      if (! musicPlayer.startPlayingFile("/track002.mp3")) {
        Serial.println("Could not open file track002.mp3");
      }
      Serial.println(F("Started playing"));
    }
    }
  }
