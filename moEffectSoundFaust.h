/*******************************************************************************

                                moEffectSoundFaust.h

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *  (at your option) any later version.                                    *
  *                                                                          *
  *   This code is distributed in the hope that it will be useful, but       *
  *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
  *   General Public License for more details.                               *
  *                                                                          *
  *   A copy of the GNU General Public License is available on the World     *
  *   Wide Web at <http://www.gnu.org/copyleft/gpl.html>. You can also       *
  *   obtain it by writing to the Free Software Foundation,                  *
  *   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.         *
  *                                                                          *
  ****************************************************************************

  Copyright(C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa


*******************************************************************************/

#ifndef __MO_EFFECT_SOUNDFAUST_H__
#define __MO_EFFECT_SOUNDFAUST_H__

#include "moPlugin.h"
#include "moTimeManager.h"
#include "moSoundManager.h"

#define MO_FREEALUT 1

#ifdef MO_FREEALUT
#include <AL/alut.h>
#endif


/**
* Embedding Faust Here:
* @param
*     #define SOUNDFILE for using sndfile library and mp3/wav files
*     #define DYNAMIC_DSP for compiling DSP on the fly
*/

#define SOUNDFILE 1
#define MOLDEO_SOUND_FAUST_PLUGIN 1

//#define MIDICTRL
//#define DYNAMIC_DSP
//#define OSCCTRL
//#define JUCE_DRIVER
#ifdef MO_LINUX
#define PORTAUDIO_DRIVER 1
#endif

#ifdef MO_MACOSX
#define constexpr const
#define COREAUDIO_DRIVER 1
#endif


#include "dsp-faust-portaudio/DspFaust.h"



#define MO_SOUNDFAUST_TRANSLATE_X 0
#define MO_SOUNDFAUST_TRANSLATE_Y 1
#define MO_SOUNDFAUST_SCALE_X 2
#define MO_SOUNDFAUST_SCALE_Y 3

enum moSoundFaustParamIndex {
	SOUNDFAUST_ALPHA,
	SOUNDFAUST_COLOR,
	SOUNDFAUST_SYNC,
	SOUNDFAUST_PHASE,
	SOUNDFAUST_GUIDES,
	SOUNDFAUST_SOUND,
	SOUNDFAUST_SEEKPOSITION,
	SOUNDFAUST_MODE,
	SOUNDFAUST_LOOP,
	SOUNDFAUST_LAUNCH,
	SOUNDFAUST_SPEEDOFSOUND,
	SOUNDFAUST_PITCH,
	SOUNDFAUST_VOLUME,
	SOUNDFAUST_VOLUME_IN,
	SOUNDFAUST_BALANCE,
	SOUNDFAUST_BALANCE_MONO_STEREO,

	SOUNDFAUST_SHOWAUDIODATA,
	SOUNDFAUST_DISPLAY_X,
	SOUNDFAUST_DISPLAY_Y,
	SOUNDFAUST_DISPLAY_WIDTH,
	SOUNDFAUST_DISPLAY_HEIGHT,

	SOUNDFAUST_TEXTURE,
	SOUNDFAUST_BLENDING,

	SOUNDFAUST_SCALEX_VBARS,
  SOUNDFAUST_SCALEY_VBARS,
  SOUNDFAUST_SCALEZ_VBARS,

  SOUNDFAUST_SCALEX,
  SOUNDFAUST_SCALEY,
  SOUNDFAUST_SCALEZ,

	SOUNDFAUST_TRANSLATEX,
	SOUNDFAUST_TRANSLATEY,
	SOUNDFAUST_TRANSLATEZ,

	SOUNDFAUST_DIRECTIONX,
	SOUNDFAUST_DIRECTIONY,
	SOUNDFAUST_DIRECTIONZ,

	SOUNDFAUST_SPEEDX,
	SOUNDFAUST_SPEEDY,
	SOUNDFAUST_SPEEDZ,

	SOUNDFAUST_INLET,
	SOUNDFAUST_OUTLET
};

class JSONUITemplatedDecoder;
class JSONUIFloatDecoder;

enum moSoundFaustMode {
	MO_SOUND_MODE_SINGLE = 0,
	MO_SOUND_MODE_LONGPLAY = 1,
	MO_SOUND_MODE_SCRIPT = 2,
	MO_SOUNDFAUST_FILTER = 3,
	MO_SOUND_MODE_LAUNCH_AND_WAIT = 4,
	MO_SOUND_MODE_LAUNCH_NEXT_AND_WAIT = 5
};

struct Soundfile;

class moSoundFaustAudio : public moSound3D {

  public:

		moSoundFaustAudio( DspFaust* p_pDspFaust=NULL);
		void SetDsp( DspFaust* p_pDspFaust ) {
			m_pDspFaust	= p_pDspFaust;
		}
    virtual ~moSoundFaustAudio();

    virtual MOboolean  Init();
    virtual MOboolean  Finish();

  /**
    OVERRIDE moSound functions
    OpenAL es diferente a GStreamer
  */
    virtual void Play();
    virtual void Stop();
    virtual void Pause();
    virtual void Rewind();
    virtual moStreamState State();
    virtual void Update();
    virtual void SetVolume( float gain );
		virtual void SetVolumeIn( float gain );
    virtual void SetBalance( float gain );
		virtual void SetBalanceMonoStereo( float gain );
    virtual float GetVolume();
    virtual float GetActualSampleVolume();
    virtual void SetPitch( float pitch );
    virtual float GetPitch();
    virtual void SetLoop( bool loop );
    virtual void SetSpeedOfSound( float speedofsound );
    virtual bool IsPlaying();
		virtual MOulong GetDuration();
		virtual MOulong GetPosition();

    MOboolean BuildEmpty( MOuint p_size );
    MOboolean BuildFromBuffer( MOuint p_size, GLvoid* p_buffer );
    MOboolean BuildFromFile( const moText& p_filename );

    virtual void Final();
    virtual void Frame(int frame);
    virtual void Repeat(int repeat);

    void PlaySample( MOint sampleid );

    void SetPosition( float x, float y, float z );
    void SetPosition( const moVector3f& v_pos ) { SetPosition(v_pos.X(), v_pos.Y(), v_pos.Z() );  }

    void SetVelocity( float x, float y, float z );
    void SetVelocity( const moVector3f& v_vel ) { SetVelocity(v_vel.X(), v_vel.Y(), v_vel.Z() );  }

    void SetDirection( float x, float y, float z );
    void SetDirection( const moVector3f& v_dir ) { SetDirection(v_dir.X(), v_dir.Y(), v_dir.Z() );  }

		void Seek( long position );

		long m_SoundfileIndex;
		Soundfile*	m_pSoundfile;

		float m_BalanceMonoStereo = 1.0;

    int m_iAlState;

    MOuint		    m_Buffers[NUMBUFFERS];

/*
    ALenum			m_eBufferFormat;
    ALsizei	m_ulDataSize;
    ALsizei	m_ulFrequency;
    ALsizei	m_ulFormat;
    ALsizei	m_ulBufferSize;
    ALsizei	m_ulBytesWritten;*/

    void *			m_pData;
    ALboolean	m_ulLoop;


    ALfloat	m_fFrequency;

    /**

      int m_iAlState;

      MOint			m_ActualSample;
      MOint			m_OldSample;

      moFile*			m_pFile;
      moDataManager*	m_pDataMan;
      moFileManager*	m_pFileMan;

      MOuint		    m_Buffers[NUMBUFFERS];
      MOuint		    m_SourceId;
      MOuint			  m_BufferId;

      MOint			m_eBufferFormat;


      unsigned long	m_ulDataSize;
      unsigned long	m_ulFrequency;
      unsigned long	m_ulFormat;
      unsigned long	m_ulBufferSize;
      unsigned long	m_ulBytesWritten;
      void *			m_pData;

      bool	m_ulLoop;

      MOint			m_BufferSize;
    */
		DspFaust*     m_pDspFaust;

};

class moEffectSoundFaust: public moEffect {

public:

	moOutlets						m_Outports;
  JSONUIFloatDecoder* myDecoder;
  moPlaneGeometry  m_LevelBoxes[15];
  moGLMatrixf   m_LevelModels[15];
  moMesh        m_LevelMeshes[15];
  moMaterial  m_LevelMats[15];


  moEffectSoundFaust();
  virtual ~moEffectSoundFaust();

  virtual MOboolean Init();
  void Draw( moTempo* tempogral, moEffectState* parentstate = NULL);
  MOboolean Finish();
  void Interaction( moIODeviceManager * );
  moConfigDefinition * GetDefinition( moConfigDefinition *p_configdefinition );

  virtual MOboolean ResolveValue( moParam& param, int value_index, bool p_refresh=false );

  void UpdateParameters();
  void UpdateSound( const moText& p_newfilename );
  void Update( moEventList *Events );

  void MM_render_one_buffer();
  static int al_check_error(const char * p_message);
  int alutCheckError(const char * p_message);
  void ShowBufferInfo( ALint p_BufferId );

	void DrawDisplayInfo( MOfloat x, MOfloat y);

  int outports_outlet_base_index;

	moDataMessages   m_DataMessages;
	moOutlet* m_pOutDataMessages;

private:

  moSoundManager* m_pSM;
  ALCdevice*  m_pALCDevice;
  ALCcontext* m_pALCContext;

  ALuint helloBuffer, helloSource;
  long    last_ticks;

  //moGsGraph   m_Audio;
  moSoundFaustAudio*   m_pAudio;
  DspFaust*     m_pDspFaust;

  moText      m_SoundFaustFilename;
  moText      m_SoundFaustFilenameFull;

  float m_fSampleVolume;
	int			m_iGuides;
  bool    m_bAudioStarted;
  bool    m_bLaunch;
  float    m_fLaunch;
  bool    m_bLoop;
  float   m_fPitch;
  float   m_fVolume;
	float   m_fVolumeIn;
  float   m_fSpeedOfSound;

  int i_PlayedTimes;
	moSoundFaustMode m_iPlayMode;

	float         m_DisplayX;
	float         m_DisplayY;
	float         m_DisplayW;
	float         m_DisplayH;
	long					m_FramesLength;
	long					m_FramePosition;

  MOint Tx, Ty, Tz;
  MOfloat Sx, Sy, Sz;
  MOint Rx, Ry, Rz;

  moVector3f m_vScale;
	moVector3f m_vScaleVBars;
	moVector3f m_vScaleHBars;
	moVector3f m_vScaleButtons;
  moVector3f m_vPosition;
  moVector3f m_vDirection;
  moVector3f m_vSpeed;

  float     m_UserPosition;

  static bool m_bAlutInit;

  int m_pDspParamsCount;

  friend class moConfig;
  moText  m_uijson;
  string m_cuijson;


};

class moEffectSoundFaustFactory : public moEffectFactory {

public:
    moEffectSoundFaustFactory() {}
    virtual ~moEffectSoundFaustFactory() {}
    moEffect* Create();
    void Destroy(moEffect* fx);
};

extern "C"
{
MO_PLG_API moEffectFactory* CreateEffectFactory();
MO_PLG_API void DestroyEffectFactory();
}

#endif
