//Et forsøk på å gjenopplive gammel hardware til moderne standard. Mer spesifikt en Radionette Kurér.

// Koblingsdiagram:
// GPIO04 -> LCD4
// GPIO17 -> LCD6
// GPIO05  -> LCD11
// GPIO18 -> LCD12
// GPIO19 -> LCD13
// GPIO16 -> LCD14
// GPIO22 -> AUDIO_DATA_IN
// GPIO25 -> AUDIO_WORD_SELECT
// GPIO26 -> AUDIO_CLOCK

#include "BluetoothA2DPSink.h"

#include <LiquidCrystal.h>

LiquidCrystal lcd(4,17,5,18,19,16);

BluetoothA2DPSink a2dp_sink;
// Song artist provided by AVRC data (bluetooth)
String artistStr_ = "";
// Song title provided by AVRC data (bluetooth)
String titleStr_ = "";
String artist = "";
String title = "";
void setup() {

    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("-Radionette-");
    lcd.setCursor(3,1);
    lcd.print("Frakoblet");

    i2s_pin_config_t my_pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 22,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    a2dp_sink.set_pin_config(my_pin_config);
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_avrc_connection_state_callback(avrc_connection_state_callback);
    a2dp_sink.start("Radionette");
}

void loop() {

}


void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
    switch (id) {
        case ESP_AVRC_MD_ATTR_TITLE:
            titleStr_ = (char*) text;
            break;
        
        case ESP_AVRC_MD_ATTR_ARTIST:
            artistStr_ = (char*) text;
            break;
    }
    if ( artistStr_.isEmpty() ) {
    }
    else {
        artist = artistStr_;
        if ( titleStr_.isEmpty() ) {
        }
        else {
            title = titleStr_;
        }
    }
 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(title);
    lcd.setCursor(0,1);
    lcd.print(artist);
}

void avrc_connection_state_callback(bool spiller){
  if (spiller) {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("-Radionette-");
    lcd.setCursor(3,1);
    lcd.print("Tilkoblet!");
  }
  else {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("-Radionette-");
    lcd.setCursor(3,1);
    lcd.print("Frakoblet");
  }

}
