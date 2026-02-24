#include "MusicPlayer.h"
#include "songs.h"

#define POTENTIOMETER_PIN 0

#define LED_G_PIN 1
#define LED_R_PIN 3
#define LED_B_PIN 10

#define BUZZER_PIN 21

#define BUTTON_R_PIN 6
#define BUTTON_G_PIN 7
#define BUTTON_B_PIN 5

#define BUTTON_BACK_PIN 20

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

  delay(500);
  Serial.println("Connection established!");
}

#define SHORT_PRESS_TIME 500

int previousButtonBackState = HIGH;
int currentButtonBackState;
int buttonBackPressedTime = 0;
int buttonBackReleasedTime = 0;

int song_idx = 0;
Song song = ALL_SONGS[song_idx];

bool wasPlaying = false;

void loop() {
  Serial.println(NB_SONGS);

  // Detect short / long press on back button

  bool buttonBackPress = false;
  bool buttonBackShortPress = false;
  bool buttonBackLongPress = false;

  currentButtonBackState = digitalRead(BUTTON_BACK_PIN);
  
  if (previousButtonBackState == HIGH && currentButtonBackState == LOW) {
    buttonBackPressedTime = millis();
    buttonBackPress = true;
  }

  if (previousButtonBackState == LOW && currentButtonBackState == HIGH) {
    buttonBackReleasedTime = millis();

    if (buttonBackReleasedTime - buttonBackPressedTime < SHORT_PRESS_TIME)
      buttonBackShortPress = true;
    else
      buttonBackLongPress = true;
  }

  previousButtonBackState = currentButtonBackState;

  // Handle presses on back button

  if (buttonBackPress) {
    wasPlaying = player.isPlaying();
    player.stop();
  }

  if (buttonBackShortPress) {
    if (!wasPlaying)
      player.play(ALL_SONGS[song_idx]);
  }

  if (buttonBackLongPress) {
    song_idx = (song_idx + 1) % NB_SONGS;
    song = ALL_SONGS[song_idx];
    player.play(song);
  }


  if (!player.isFinished())
  {
    int potentiometerValue = analogRead(POTENTIOMETER_PIN);
    int tempo = map(potentiometerValue, 0, 4095, song.tempo / 2, song.tempo * 2);
    player.setTempo(tempo);
    player.update();
  }

  digitalWrite(LED_R_PIN, LOW);
  digitalWrite(LED_G_PIN, LOW);
  digitalWrite(LED_B_PIN, LOW);

  if (digitalRead(BUTTON_R_PIN) == LOW)
    digitalWrite(LED_R_PIN, HIGH);

  if (digitalRead(BUTTON_G_PIN) == LOW)
    digitalWrite(LED_G_PIN, HIGH);

  if (digitalRead(BUTTON_B_PIN) == LOW)
    digitalWrite(LED_B_PIN, HIGH);

  // if (digitalRead(BUTTON_BACK) == LOW)
  // {
  //   int potentiometerValue = analogRead(POTENTIOMETER);
  //   int frequency = map(potentiometerValue, 0, 4095, 50, 2000);
  //   tone(BUZZER, frequency);
  // }

  // delay(1);
}
