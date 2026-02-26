#include "pins.h"
#include "MusicPlayer.h"
#include "songs.h"
#include "Button.h"
#include "LedController.h"
#include "leds.h"

Button rButton(BUTTON_R_PIN);
Button gButton(BUTTON_G_PIN);
Button bButton(BUTTON_B_PIN);
LedController rLed;
LedController gLed;
LedController bLed;

Button backButton(BUTTON_BACK_PIN);
MusicPlayer player(BUZZER_PIN);

void setup() {
  Serial.begin(9600);

  pinMode(LED_R_PIN, OUTPUT);
  pinMode(LED_G_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(BUTTON_R_PIN, INPUT_PULLUP);
  pinMode(BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);

  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);

  analogSetAttenuation(ADC_11db);

  delay(50);
  Serial.println("Connection established!");
}

int song_idx = 0;
Song song = ALL_SONGS[song_idx];

bool wasPlaying = false;

void loop() {
  int potentiometerValue = 4095 - analogRead(POTENTIOMETER_PIN);

  // Handle back button

  backButton.update();

  if (backButton.isPressed()) {
    wasPlaying = player.isPlaying();
    if (player.isPlaying()) {
      player.stop();
      setRGBFromPitch(REST);
    }
  }

  if (backButton.isReleased()) {
    if (player.isStopped() && !wasPlaying) {
      player.play(ALL_SONGS[song_idx]);
    }
  }

  if (backButton.isMultiClick()) {
    song_idx = (song_idx + 1) % NB_SONGS;
    player.play(ALL_SONGS[song_idx]);
  }

  if (player.isPlaying())
  {
    // Handle tempo tuning with potentiometer
    int tempo = map(potentiometerValue, 0, 4095, song.tempo / 2, song.tempo * 2);
    player.setTempo(tempo);
    player.update();
    setRGBFromPitch(player.getPitch());
  }
  
  if (!player.isPlaying())
  {
    // Handled RGB LEDs / buttons

    rButton.update();
    gButton.update();
    bButton.update();

    int rBrightness = rLed.update(rButton.isShortPress(), rButton.isLongPress(), rButton.isMultiClick(), potentiometerValue);
    analogWrite(LED_R_PIN, rBrightness);
    
    int gBrightness = gLed.update(gButton.isShortPress(), gButton.isLongPress(), gButton.isMultiClick(), potentiometerValue);
    analogWrite(LED_G_PIN, gBrightness);
    
    int bBrightness = bLed.update(bButton.isShortPress(), bButton.isLongPress(), bButton.isMultiClick(), potentiometerValue);
    analogWrite(LED_B_PIN, bBrightness);

    // When one led takes over tuning, clear the others
    if (rButton.isShortPress(), rButton.isLongPress() || rButton.isMultiClick()) {
      gLed.exitTuning(); bLed.exitTuning();
    }
    if (gButton.isShortPress() || gButton.isLongPress() || gButton.isMultiClick()) {
      rLed.exitTuning(); bLed.exitTuning();
    }
    if (bButton.isShortPress() || bButton.isLongPress() || bButton.isMultiClick()) {
      rLed.exitTuning(); gLed.exitTuning();
    }
  }
}