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

int blinkInterval = 0;
int fadeSharpness = 0;

float blinkPhase = 0.0;  // 0.0 = off, 1.0 = on
int blinkTime = 0;

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

    // (Re)initialize blinking parameters when no LED blinks
    if (!rLed.mustBlink() && !gLed.mustBlink() && !bLed.mustBlink()) {
      blinkInterval = 500;
      fadeSharpness = 128;
    }

    // Handle LEDs state; blinkInterval set by single controlling LED if necessary
    int rBrightness = rLed.update(rButton.isShortPress(), rButton.isLongPress(), rButton.isMultiClick(),
                                  potentiometerValue, &blinkInterval, &fadeSharpness);
    int gBrightness = gLed.update(gButton.isShortPress(), gButton.isLongPress(), gButton.isMultiClick(),
                                  potentiometerValue, &blinkInterval, &fadeSharpness);
    int bBrightness = bLed.update(bButton.isShortPress(), bButton.isLongPress(), bButton.isMultiClick(),
                                  potentiometerValue, &blinkInterval, &fadeSharpness);

    // Handle LEDs blinking

    float speed = 1000.0 / blinkInterval; // Frequency in Hz (cycles per second)
    float sharpness = fadeSharpness / 10.;  // 1.0 = smooth sine, more = sharper

    unsigned long now = millis();
    float deltaTime = (now - blinkTime) / 1000.0;
    blinkTime = now;

    // 1. Update phase (fixed frequency)
    blinkPhase += 2.0 * PI * speed * deltaTime;
    if (blinkPhase > 2.0 * PI) blinkPhase -= 2.0 * PI;

    // 2. The "Stretched & Clamped" Formula
    // We center the sine at 0 (-1 to 1), multiply by sharpness,
    // then shift it back and constrain it.
    float rawSine = sin(blinkPhase);
    float stretched = rawSine * sharpness;
    
    // Constrain to -1.0 to 1.0, then map to 0-255
    float clamped = constrain(stretched, -1.0, 1.0);
    int finalBrightness = (clamped + 1.0) * 127.5;

    // 4. Assign to your LEDs
    if (rLed.mustBlink()) rBrightness = finalBrightness;
    if (gLed.mustBlink()) gBrightness = finalBrightness;
    if (bLed.mustBlink()) bBrightness = finalBrightness;

    // Light LEDs
    analogWrite(LED_R_PIN, rBrightness);
    analogWrite(LED_G_PIN, gBrightness);
    analogWrite(LED_B_PIN, bBrightness);

    // When one LED takes over tuning, clear the others
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