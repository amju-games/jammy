/*
Amju Games source code (c) Copyright Jason Colman 2006
$Log: BassSoundPlayer.cpp,v $
Revision 1.15  2007/08/29 21:53:54  jay
Fixes for Bass + Windows Vista; error message

Revision 1.14  2007/02/25 20:32:28  jay
Remove BASS_Free in dtor, to try to fix crash on shutdown

Revision 1.13  2007/02/07 14:07:27  Administrator
Fix bad sound quality on Windows: larger buffer was needed apparently.

Revision 1.12  2007/02/07 13:24:34  jay
Fix: not checking BASS version correctly

Revision 1.11  2007/02/06 23:26:18  Administrator
New Bass version 2.3; new Midi functions

Revision 1.10  2006/11/18 23:13:32  Administrator
Hopefully fix nasty sound when song started witn no volume

Revision 1.9  2006/09/23 20:28:33  Administrator
Always call base class version of SetSongMaxVolume

Revision 1.8  2006/09/15 12:47:45  Administrator
Fixes to get PetZoo+SceneEditor to build on Windows/MSVC

Revision 1.7  2006/06/27 18:01:12  jay
Updated comments

Revision 1.6  2006/06/02 18:05:29  jay
Merge fix from pool - volume was being ignored.

Revision 1.4.2.5  2006/06/02 18:01:47  jay
Fix bug - volume was being ignored!

Revision 1.4.2.4  2006/03/25 12:49:29  jay
Fix set song volume

Revision 1.4.4.5  2006/03/25 12:46:31  jay
Fix Song volume - this was incorrectly setting the SYSTEM volume!

Revision 1.4.4.4  2006/03/22 21:19:21  Administrator
Make sure song volume starts at 1.0

Revision 1.4.4.3  2006/03/15 23:27:00  Administrator
More debug output

Revision 1.4.4.2  2006/03/15 07:42:23  jay
Add BASS Sound lib

Revision 1.4.2.2  2006/03/09 22:59:08  jay
Add BASS - to correct dir this time!

Revision 1.1.2.1  2006/03/09 21:49:08  jay
Add BASS to pool

Revision 1.3  2006/02/17 18:57:21  jay
Add support for Glue Files; fix StopSong()

Revision 1.2  2006/02/15 23:32:59  Administrator
Fix Windows compile

Revision 1.1  2006/02/14 13:23:01  jay
Added support for BASS sound system

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#ifdef AMJU_USE_BASS
#include <iostream>
#include "BassSoundPlayer.h"
#include "StringUtils.h"
#ifdef MACOSX
#include "Bass2.3/Macosx/bass.h"
#include "Bass2.3/Macosx/bassmidi.h"
#endif
#ifdef WIN32
#include "Bass2.3/Win/bass.h"
#include "Bass2.3/Win/bassmidi.h"
#endif
#include "StringUtils.h"

namespace Amju
{
void ReportError(const std::string&);

BassSoundPlayer::BassSoundPlayer()
{
  m_chan = (DWORD)-1; 

  // check that BASS 2.3 was loaded
  unsigned long ver = BASS_GetVersion();
#ifdef _DEBUG
std::cout << "BASS version: " << ToHexString(ver).c_str() << "\n";
#endif

  if (HIWORD(ver) != BASSVERSION) 
  {
    std::string s = "Unexpected BASS version: expected " + ToHexString(BASSVERSION) + " got: " + ToHexString(HIWORD(ver));
    ReportError(s);
    return;
  }

  // setup output - default device
  if (!BASS_Init(-1,44100,0,0,NULL))
  {
    ReportError("BASS: Can't initialize device");
  }

  SetSongMaxVolume(1.0f);
}

BassSoundPlayer::~BassSoundPlayer()
{
  // TODO Shut down to avoid stuttering or clicks
  // This seems to cause a crash, so best avoided
  //BASS_Free();
}

bool BassSoundPlayer::PlayWav(const std::string& wavFile, float volume)
{
  if (m_maxWavVol == 0)
  {
    return true; // ok, not an error
  }

  // max no of simultaneous playbacks (of same wav ? or all wavs ?)
  static const int MAX_PLAYBACKS = 6;

  HSAMPLE hs = 0;
  if (GetGlueFile())
  {
    std::string strippedFile = StripPath(wavFile);
    // Find the start of the wav in the glue file; and find the length
    uint32 wavPos = 0;
    if (!GetGlueFile()->GetSeekBase(strippedFile, &wavPos))
    {
      std::string s = "BASS: Wav: not in Glue File: ";
      s += strippedFile;
      ReportError(s);
    }
    uint32 wavLength = GetGlueFile()->GetSize(strippedFile);

    // Use GlueFileBinaryData to get the data without copying it
    GlueFileBinaryData data = GetGlueFile()->GetBinary(wavPos, wavLength);
    hs = BASS_SampleLoad(
      TRUE, // in mem ?
      data.GetBuffer(), // start of wav in memory 
      0, // not used 
      wavLength,
      MAX_PLAYBACKS, 
      BASS_SAMPLE_OVER_POS); // flags
  }
  else
  {
    hs = BASS_SampleLoad(
      FALSE, // in mem ?
      wavFile.c_str(), // filename
      0, // file offset
      0, // "use all data up to end of file": 2 'iterators' defining a range ?
      MAX_PLAYBACKS, 
      BASS_SAMPLE_OVER_POS); // flags
  }

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
  int vol = (int)(volume * m_maxWavVol * 100.0f);
  BASS_ChannelSetAttributes(hc, -1, vol, -1);

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

  if (GetGlueFile())
  {
    std::string strippedFile = StripPath(songFile);
    // Find the start of the song in the glue file; and find the length
    uint32 songPos = 0;
    if (!GetGlueFile()->GetSeekBase(strippedFile, &songPos))
    {
      std::string s = "BASS: Music: not in Glue File: ";
      s += strippedFile;
      ReportError(s);
    }
    uint32 length = GetGlueFile()->GetSize(strippedFile);

    // Use GlueFileBinaryData to get the data without copying it
    GlueFileBinaryData data = GetGlueFile()->GetBinary(songPos, length);
    if (!(m_chan=BASS_MusicLoad(
      TRUE, // mem ?
      data.GetBuffer(), // start of song data 
      0, // offset
      length, // length
      BASS_SAMPLE_LOOP|BASS_MUSIC_SURROUND, 
      0)))  // sample rate - 0 => use default value
    {
      std::string s = "BASS: Music: Can't play song from Glue file: "; 
      s += strippedFile;
      ReportError(s);
      return false;
    }
  }
  else
  {
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
	  s += ToString(errCode);
      ReportError(s);
      return false;
    }
  }

  // Set vol
  int vol = (int)(m_maxSongVol * 100.0f);
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
  SoundPlayer::SetSongMaxVolume(f); 

  if (m_chan == -1)
  {
    return;
  }

  int newVol = (int)(f * 100.0f);
  BASS_ChannelSetAttributes(m_chan, -1, newVol, -1);
}


static HSTREAM str = 0;
void BassSoundPlayer::MidiSetSoundFont(const char* soundfont)
{
  BASS_MIDI_FONT font;
  if (font.font=BASS_MIDI_FontInit(soundfont,0)) 
  { 
    font.preset=-1; // all presets
    font.bank=0; // default bank(s)
    BASS_MIDI_StreamSetFonts(0,&font,1); // make it the default
  }
  else
  {
#ifdef _DEBUG
    std::cout << "Failed to load soundfont: " << soundfont << "\n";
#endif
    return;
  }

	/* 10ms update period */
	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD,10);

	BASS_SetConfig(BASS_CONFIG_BUFFER, 64); 
  // 500 default; we want to decrease latency but not
  //  break up the sound

	/* create a MIDI stream */
	str=BASS_MIDI_StreamCreate(1 /* JC - num channels ? */, 0,44100);
	BASS_ChannelPlay(str,FALSE);
}

void BassSoundPlayer::MidiNoteOn(int note)
{
  // 60 is middle C
  // 127 is max volume
  BASS_MIDI_StreamEvent(str,0,MIDI_EVENT_NOTE,MAKELONG(60 + note,127));
}

void BassSoundPlayer::MidiNoteOff(int note)
{
	//BASS_MIDI_StreamEvent(str,0,MIDI_EVENT_NOTESOFF /* JC - was NOTEOFF */,60 + note);
  BASS_MIDI_StreamEvent(str,0,MIDI_EVENT_NOTE,MAKELONG(60 + note,0));
}
}
#endif // AMJU_USE_BASS

