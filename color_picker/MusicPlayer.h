#pragma once
#include <Arduino.h>

struct Song {
  const int* melody;   // interleaved { note, duration, note, duration, … }
  int        length;   // total number of ints in the array (notes * 2)
  int        tempo;
};

class MusicPlayer {
public:
  MusicPlayer(int buzzerPin);
  void play(const Song& song);
  bool update();
  void stop();
  void pause();
  void resume();

  void setTempo(int tempo);

  bool isPlaying() const;
  bool isPaused()  const;
  bool isFinished() const;

private:
  int _buzzerPin;

  const int*   _melody;
  int          _noteCount;       // total notes (length / 2)
  int          _tempo;

  int          _noteIndex;       // current index into melody[] (steps of 2)
  unsigned long _noteStartMs;
  int           _noteDurationMs; // full duration of current note
  bool          _noteToneActive; // has tone() been called for this note?

  bool _active;
  bool _paused;
  bool _finished;

  // Internal helpers
  void _startNote();
  void _advanceNote();
};