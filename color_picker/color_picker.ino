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

int tunedBrightness = MAX_BRIGHTNESS / 2;

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

    // No LED blinking -> brightness tuning
    if (!rLed.isBlinking() && !gLed.isBlinking() && !bLed.isBlinking()) {
      rLed.brightnessTuningOn();
      gLed.brightnessTuningOn();
      bLed.brightnessTuningOn();
    }

    // Button press -> on/off
    if (rButton.isShortPress() && !rLed.isBlinking()) { rLed.toggleOnOff(); }
    if (gButton.isShortPress() && !gLed.isBlinking()) { gLed.toggleOnOff(); }
    if (bButton.isShortPress() && !bLed.isBlinking()) { bLed.toggleOnOff(); }

    // Multi-click -> blinking on/off
    if (rButton.isMultiClick()) { rLed.turnOn(); rLed.toggleBlinking(); }
    if (gButton.isMultiClick()) { gLed.turnOn(); gLed.toggleBlinking(); }
    if (bButton.isMultiClick()) { bLed.turnOn(); bLed.toggleBlinking(); }

    if (rButton.isMultiClick() || gButton.isMultiClick() || bButton.isMultiClick()) {
      // Entering/exiting blinking -> disable brightness tuning for all LEDs
      rLed.brightnessTuningOff();
      gLed.brightnessTuningOff();
      bLed.brightnessTuningOff();

      // Synchronize blinking clock between LEDs
      unsigned long syncTime = millis();
      rLed.syncBlink(syncTime);
      gLed.syncBlink(syncTime);
      bLed.syncBlink(syncTime);
    }

    // Button press when blinking -> off
    if (rButton.isShortPress() && rLed.isBlinking()) { rLed.toggleBlinking(); rLed.turnOff(); }
    if (gButton.isShortPress() && gLed.isBlinking()) { gLed.toggleBlinking(); gLed.turnOff(); }
    if (bButton.isShortPress() && bLed.isBlinking()) { bLed.toggleBlinking(); bLed.turnOff(); }

    // Update LEDs brightness
    int rBrightness = rLed.update(potentiometerValue, &tunedBrightness);
    int gBrightness = gLed.update(potentiometerValue, &tunedBrightness);
    int bBrightness = bLed.update(potentiometerValue, &tunedBrightness);

    // Light LEDs
    analogWrite(LED_R_PIN, rBrightness);
    analogWrite(LED_G_PIN, gBrightness);
    analogWrite(LED_B_PIN, bBrightness);
  }
}
