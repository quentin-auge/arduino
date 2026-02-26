#include "pins.h"
#include "MusicPlayer.h"
#include "songs.h"
#include "Button.h"
#include "leds.h"

MusicPlayer player(BUZZER_PIN);

Button rButton(BUTTON_R_PIN);
Button gButton(BUTTON_G_PIN);
Button bButton(BUTTON_B_PIN);

Button backButton(BUTTON_BACK_PIN);

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

int r;
int g;
int b;

bool rTuning = false;
bool gTuning = false;
bool bTuning = false;

void loop() {
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
    int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    int tempo = map(4095 - potentiometerValue, 0, 4095, song.tempo / 2, song.tempo * 2);
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

    // Handle forced LEDs

    if (rButton.isShortPress()) {
      r = forceLed(LED_R_PIN, r);
      rTuning = false; gTuning = false; bTuning = false;
    }

    if (gButton.isShortPress()) {
      g = forceLed(LED_G_PIN, g);
      rTuning = false; gTuning = false; bTuning = false;
    }

    if (bButton.isShortPress()) {
      b = forceLed(LED_B_PIN, b);
      rTuning = false; gTuning = false; bTuning = false;
    }

    // Handle tuned LEDs

    int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    int tunedBrightness = map(4095 - potentiometerValue, 0, 4095, 0, 255);

    if (rButton.isLongPress()) {
      blipLed(LED_R_PIN, r);
      rTuning = true; gTuning = false; bTuning = false;
    }

    if (gButton.isLongPress()) {
      blipLed(LED_G_PIN, g);
      rTuning = false; gTuning = true; bTuning = false;
    }

    if (bButton.isLongPress()) {
      blipLed(LED_B_PIN, b);
      rTuning = false; gTuning = false; bTuning = true;
    }

    if (rTuning) r = tunedBrightness;
    if (gTuning) g = tunedBrightness;
    if (bTuning) b = tunedBrightness;

    // Apply

    analogWrite(LED_R_PIN, r);
    analogWrite(LED_G_PIN, g);
    analogWrite(LED_B_PIN, b);
  }
}