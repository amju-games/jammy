#pragma once

#include <string>

class BassSoundPlayer 
{
public:
  BassSoundPlayer();
  ~BassSoundPlayer();

  bool PlayWav(const std::string& wavFile, bool loop = false);
  bool PlaySong(const std::string& songFile);
  void StopSong();
  void Update();
  void SetSongMaxVolume(float);

private:
  // Channel for current song
  unsigned long m_chan;
};

