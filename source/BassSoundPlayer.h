#pragma once

class BassSoundPlayer 
{
public:
  BassSoundPlayer();
  ~BassSoundPlayer();

  virtual bool PlayWav(const std::string& wavFile, float volume = 1.0f);
  virtual bool PlaySong(const std::string& songFile);
  virtual void StopSong();
  virtual void Update();
  virtual void SetSongMaxVolume(float);

private:
  // Channel for current song
  unsigned long m_chan;
};

