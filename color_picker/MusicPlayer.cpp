#include "MusicPlayer.h"

MusicPlayer::MusicPlayer(int buzzerPin)
  : _buzzerPin(buzzerPin),
    _melody(nullptr),
    _noteCount(0),
    _tempo(0),
    _noteIndex(0),
    _noteStartMs(0),
    _noteDurationMs(0),
    _noteToneActive(false),
    _active(false),
    _paused(false),
    _finished(false)
{}

void MusicPlayer::play(const Song& song) {
  noTone(_buzzerPin);

  _melody    = song.melody;
  _noteCount = song.length / 2;
  _tempo     = song.tempo;
  _noteIndex = 0;

  _noteToneActive = false;
  _active         = true;
  _paused         = false;
  _finished       = false;
}

bool MusicPlayer::update() {
  if (!_active || _paused || _finished) return false;

  // All notes done
  if (_noteIndex >= _noteCount * 2) {
    noTone(_buzzerPin);
    _active   = false;
    _finished = true;
    return false;
  }

  unsigned long now = millis();

  // Start the current note
  if (!_noteToneActive) {
    _startNote();
    _noteStartMs    = now;
    _noteToneActive = true;
  }

  // Check whether the full note duration has elapsed
  if (now - _noteStartMs >= (unsigned long)_noteDurationMs) {
    _advanceNote();
  }

  return true;
}

void MusicPlayer::stop() {
  noTone(_buzzerPin);
  _active         = false;
  _paused         = false;
  _finished       = false;
  _noteToneActive = false;
}


void MusicPlayer::pause() {
  if (!_active || _paused) return;
  noTone(_buzzerPin);
  _paused         = true;
  _noteToneActive = false;
}

void MusicPlayer::resume() {
  if (!_active || !_paused) return;
  _paused       = true;
  _noteStartMs  = millis();  // reset the note timer so the note replays fully
  _paused       = false;
}

void MusicPlayer::setTempo(int tempo) {
  _tempo = tempo;
}

bool MusicPlayer::isPlaying()  const { return _active && !_paused && !_finished; }
bool MusicPlayer::isPaused()   const { return _paused; }
bool MusicPlayer::isStopped()  const { return !_active;}
bool MusicPlayer::isFinished() const { return _finished; }
int MusicPlayer::getPitch()    const { return _melody[_noteIndex]; }

void MusicPlayer::_startNote() {
  int wholenote = (60000 * 4) / _tempo;   // duration of a whole note in ms

  int divider = _melody[_noteIndex + 1];

  if (divider > 0) {
    _noteDurationMs = wholenote / divider;
  } else {
    // Negative divider = dotted note (1.5x duration)
    _noteDurationMs = wholenote / abs(divider);
    _noteDurationMs = (int)(_noteDurationMs * 1.5f);
  }

  // Tone plays for 90% of the duration; last 10% is a natural gap between notes
  int pitch = _melody[_noteIndex];
  if (pitch > 0) {
    tone(_buzzerPin, pitch, (int)(_noteDurationMs * 0.9f));
  }
 
  // REST (pitch == 0) → no tone, just the silence gap
}

void MusicPlayer::_advanceNote() {
  noTone(_buzzerPin);
  _noteIndex     += 2;
  _noteToneActive = false;
}