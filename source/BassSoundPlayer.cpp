#include <iostream>
#include "bass.h"
#include "BassSoundPlayer.h"
//#include "StringUtils.h"

#define _DEBUG

void ReportError(const std::string& s)
{
  std::cout << s << "\n";
}

BassSoundPlayer::BassSoundPlayer()
{
  m_chan = (DWORD)-1; 

  // check that BASS 2.3 was loaded
  unsigned long ver = BASS_GetVersion();
#ifdef _DEBUG
std::cout << "BASS version: " << ver << "\n";
#endif

  // setup output - default device
  if (!BASS_Init(-1,44100,0,0,NULL))
  {
    std::cout << "BASS: Can't initialize device\n";
  }

  SetSongMaxVolume(1.0f);
}

BassSoundPlayer::~BassSoundPlayer()
{
  BASS_Free();
}

bool BassSoundPlayer::PlayWav(const std::string& wavFile, bool loop)
{
  // max no of simultaneous playbacks (of same wav ? or all wavs ?)
  static const int MAX_PLAYBACKS = 6;

  HSAMPLE hs = 0;

  auto flag = BASS_SAMPLE_OVER_POS;
  if (loop)
  {
    flag |= BASS_SAMPLE_LOOP;  
  }

  hs = BASS_SampleLoad(
    FALSE, // in mem ?
    wavFile.c_str(), // filename
    0, // file offset
    0, // "use all data up to end of file": 2 'iterators' defining a range ?
    MAX_PLAYBACKS, 
    flag);
 

  if (!hs)
  {
    std::string s = "BASS: Wav: Failed to load sample: ";
    s += wavFile;
    ReportError(s);
    return false;
  } 
  HCHANNEL hc = BASS_SampleGetChannel(hs, FALSE);
  if (!hc)
  {
    std::string s = "BASS: Wav: Failed to get sample channel: ";
    s += wavFile;
    ReportError(s);
    return false;
  }

  BASS_ChannelPlay(hc, FALSE);

  // Set vol
//  int vol = (int)(volume * 100.0f);
//  BASS_ChannelSetAttributes(hc, -1, vol, -1);

  return true;
}

bool BassSoundPlayer::PlaySong(const std::string& songFile)
{
  // Play song even if song vol is currently zero - it may be turned up.

#ifdef _DEBUG
std::cout << "BASS: playing new song: " << songFile.c_str() << "\n";
#endif
  // If Glue File is set, use it to load song into memory.
  // Else use file.

    if (!(m_chan=BASS_MusicLoad(
      FALSE, // mem ?
      songFile.c_str(), // file
      0, // offset
      0, // length
      BASS_SAMPLE_LOOP|BASS_MUSIC_SURROUND, 
      0)))  // sample rate - 0 => use default value
    {
      std::string s = "BASS: Music: Can't play file: "; 
      s += songFile;
	  int errCode = BASS_ErrorGetCode();
	  s += " Error code: ";
	  s += std::to_string(errCode);
      ReportError(s);
      return false;
    }

  // Set vol
  int vol = (int)(100.0f);
  BASS_ChannelSetAttributes(m_chan, -1, vol, -1);

  BASS_ChannelPlay(m_chan,FALSE);

#ifdef _DEBUG
std::cout << "BASS: new song: " << songFile.c_str() << " chan: " << m_chan << "\n";
#endif

  return true;
}

void BassSoundPlayer::StopSong()
{
  if (m_chan == -1)
  {
    return;
  }

#ifdef _DEBUG
std::cout << "BASS: Stopping song on channel " << m_chan << "\n";
#endif
  BASS_ChannelStop(m_chan); 
}

void BassSoundPlayer::Update()
{
}

void BassSoundPlayer::SetSongMaxVolume(float f)
{
  if (m_chan == -1)
  {
    return;
  }

  int newVol = (int)(f * 100.0f);
  BASS_ChannelSetAttributes(m_chan, -1, newVol, -1);
}

