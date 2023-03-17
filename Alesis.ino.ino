#include <Adafruit_NeoPixel.h>
#include <MIDI.h>  // Add Midi Library
MIDI_CREATE_DEFAULT_INSTANCE();

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    6

//Antall LEDS
#define LED_COUNT 32
int keys_count = 37;
bool pedal = false;
// Deklarere NeoPixel strip objekt:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:

int LED[32];
int gbend = 0;
int gtouch = 0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  int i;
  // flashy turn on animation:
  strip.fill(strip.Color(100,100,100),0,32);
  strip.show();
  delay(100);
  strip.fill(strip.Color(0,0,0),0,32);
  strip.show();
  for (i = 0; i < 32; i++ ) {
    strip.setPixelColor(i,100,100,100);
    strip.show();
    delay(10);
    //strip.setPixelColor(i,100,100,100);
    //strip.show();
  }
  for (i = 32; i >= 0; i-- ) {
    strip.setPixelColor(i,100,100,100);
    strip.show();
    delay(10);
    strip.setPixelColor(i,0,0,0);
    strip.show();
  }
  strip.fill(strip.Color(100,100,100),0,32);
  strip.show();
  delay(100);
  strip.fill(strip.Color(0,0,0),0,32);
  strip.show();

  //MIDI
  MIDI.begin(1); // Initialize the Midi Library.
  // OMNI sets it to listen to all channels.. MIDI.begin(2) would set it 
  // to respond to notes on channel 2 only.
  MIDI.setHandleNoteOn(MyHandleNoteOn); // This is important!! This command
  // tells the Midi Library which function you want to call when a NOTE ON command
  // is received. In this case it's "MyHandleNoteOn".
  MIDI.setHandleNoteOff(MyHandleNoteOff); // This command tells the Midi Library 
  // to call "MyHandleNoteOff" when a NOTE OFF command is received.
  MIDI.setHandleControlChange(MyHandleCommand); //et forsøk på å hente pitch bend osv.
  MIDI.setHandlePitchBend(MyBend); // går dette da?

}

void loop() {
  MIDI.read(); // Continuously check if Midi data has been received.
}


// MyHandleNoteON is the function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {  
  Serial.println(pitch);
  //Finne ut hvilken note:
  int inputnote = pitch;
  inputnote = inputnote - 48;
  inputnote = 36 - inputnote;
  inputnote = inputnote * 0.865; //faktor for 37 ned til 32
  if (inputnote >= 0 && inputnote < 32) {
    strip.setPixelColor(inputnote,200,gbend,gtouch);
    LED[inputnote] = 1;
    strip.show();
  }
}

// MyHandleNoteOFF is the function that will be called by the Midi Library
// when a MIDI NOTE OFF message is received.
// * A NOTE ON message with Velocity = 0 will be treated as a NOTE OFF message *
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOff(byte channel, byte pitch, byte velocity) { 
  Serial.println(pitch);
  //Finne ut hvilken note:
  
  if (pedal == false) {
  int inputnote = pitch;
  inputnote = inputnote - 48;
  inputnote = 36 - inputnote;
  inputnote = inputnote * 0.865; //faktor for å få 37 ned til 32
  if (inputnote >= 0 && inputnote < 32) {
    strip.setPixelColor(inputnote,0,0,0);
    LED[inputnote] = 0;
    strip.show();
  }
  }

  
}

void MyHandleCommand(byte channel, byte program, byte styrke) {
  //1 er touchbar, 90 er borteste fader
  Serial.print(program);
  Serial.println(styrke);
  if (program == 1) {
    int touch = 0;
    touch = styrke * 2;
    gtouch = touch;
    int i = 0;
    for (i = 0; i < 32; i++ ) {
      if (LED[i] != 0) {
        strip.setPixelColor(i,200,gbend,touch);
        strip.show();
      }
    }
  }
  if (program == 64) {
    if (styrke == 127) {
      pedal = true;
    }
    if (styrke == 0) {
      pedal = false;
      for (int i; i<33; i++) {
        strip.setPixelColor(i,0,0,0);
        LED[i] = 0;
        strip.show();
      }
    }
  }
}

void MyBend(byte channel, int bend) {
  //16 tusen maks.
  bend = bend + 8192;
  bend = bend * 0.015625;
  gbend = bend;
  int i = 0;
  for (i = 0; i < 32; i++ ) {
    if (LED[i] != 0) {
      strip.setPixelColor(i,200,bend,gtouch);
      strip.show();
    }
  }
}
