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

int previousButtonBackState = HIGH;
Song song = TETRIS_SONG;

void loop() {
  int currentButtonBackState = digitalRead(BUTTON_BACK_PIN);
  if (previousButtonBackState == HIGH && currentButtonBackState == LOW) {
    if (!player.isPlaying())
      player.play(song);
    else
      player.stop();
  }
  previousButtonBackState = currentButtonBackState;

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
