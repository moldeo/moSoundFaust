/*******************************************************************************

                               moEffectSoundFaust.cpp

  ****************************************************************************
  *                                                                          *
  *   This source is free software; you can redistribute it and/or modify    *
  *   it under the terms of the GNU General Public License as published by   *
  *   the Free Software Foundation; either version 2 of the License, or      *
  *   (at your option) any later version.                                    *
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

  Copyright (C) 2006 Fabricio Costa

  Authors:
  Fabricio Costa


*******************************************************************************/

#include "moEffectSoundFaust.h"
#define DMessage(X) MODebug2->Message( moText("moEffectSoundFaust::") + X )
#define DError(X) MODebug2->Error(moText("moEffectSoundFaust::") + X )
#define DPush(X) MODebug2->Push(moText("moEffectSoundFaust::") + X )
#define DLog(X) MODebug2->Log(moText("moEffectSoundFaust::") + X )

#ifdef MO_MACOSX
#include "sndfile.h"
#endif

#include "dsp-faust-portaudio/DspFaust.cpp"


string inouts[7] = {
  "button",
  "check",
  "vslider",
  "hslider",
  "numeric",
  "hbar",
  "vbar"
};

class moUISoundFaust : public UI {

public:

  moEffectSoundFaust* m_object;
  bool _save_new_config;
  int param_index_counter = 0;

  moUISoundFaust() {}

  moUISoundFaust(moEffectSoundFaust *p_object ) {
      //so good
      m_object = p_object;
      _save_new_config = false;
      if (m_object) ((moEffectSoundFaust*)m_object)->outports_outlet_base_index = p_object->GetOutlets()->Count();
  }

  virtual ~moUISoundFaust() {

  }


  // -- widget's layouts

  void openTabBox(const char* label) {

  }
  void openHorizontalBox(const char* label) {
  }
  void openVerticalBox(const char* label) {
  }
  void closeBox() {
  }

  // -- active widgets

  void addButton(const char* label, FAUSTFLOAT* zone) {
    moParamDefinition pdef( label, "NUMERIC" );
    int pidx = m_object->GetConfig()->GetParamIndex(label);
    if (pidx<0) { m_object->GetConfig()->CreateParam( pdef ); _save_new_config = true; }
  }
  void addCheckButton(const char* label, FAUSTFLOAT* zone) {
    moParamDefinition pdef( label, "NUMERIC" );
    int pidx = m_object->GetConfig()->GetParamIndex(label);
    if (pidx<0) { m_object->GetConfig()->CreateParam( pdef ); _save_new_config = true; }
  }
  void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    moParamDefinition pdef( label, "FUNCTION" );
    int pidx = m_object->GetConfig()->GetParamIndex(label);
    if (pidx<0) { m_object->GetConfig()->CreateParam( pdef ); _save_new_config = true; }
  }
  void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    moParamDefinition pdef( label, "FUNCTION" );
    int pidx = m_object->GetConfig()->GetParamIndex(label);
    if (pidx<0) { m_object->GetConfig()->CreateParam( pdef ); _save_new_config = true; }
  }
  void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    moParamDefinition pdef( label, "NUMERIC" );
    int pidx = m_object->GetConfig()->GetParamIndex(label);
    if (pidx<0) { m_object->GetConfig()->CreateParam( pdef ); _save_new_config = true; }
    for(auto& it : ( (moEffectSoundFaust*) m_object)->myDecoder->fUiItems ) {
      if (it.label==label) {
          it.index = param_index_counter++;
          m_object->MODebug2->Message("feliz coincidencia:"+moText(label));
      }
    }
  }

  // -- passive widgets

  void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {
    int pidx = m_object->GetOutletIndex(label);
    if (pidx<0) {
      moOutlet* pout = new moOutlet();
      pout->SetMoldeoLabelName( m_object->GetLabelName() );
      pout->Init( label, m_object->GetOutlets()->Count(), "NUMERIC");
      //pout->Init( plabel, i, m_Config.GetParam(plabel).GetPtr());
      m_object->GetOutlets()->Add( pout );
      m_object->MODebug2->Message("added outlet horizontal graph! "+moText(label)+ " zone: " + FloatToStr(*zone));
      m_object->m_Outports.Add(pout);
      _save_new_config = true;
    } else {
      moOutlet* pout = m_object->GetOutlets()->Get(pidx);
      if (pout)
        m_object->m_Outports.Add(pout);
    }

    for(auto& it : ( (moEffectSoundFaust*) m_object)->myDecoder->fUiItems ) {
      if (it.label==label) {
          it.index = param_index_counter++;
          m_object->MODebug2->Message("feliz coincidencia:"+moText(label));
      }
    }

  }
  void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {
    int pidx = m_object->GetOutletIndex(label);
    if (pidx<0) {
      moOutlet* pout = new moOutlet();
      pout->SetMoldeoLabelName( m_object->GetLabelName() );
      pout->Init( label, m_object->GetOutlets()->Count(), "NUMERIC");
      //pout->Init( plabel, i, m_Config.GetParam(plabel).GetPtr());
      //pout->GetData()->
      m_object->GetOutlets()->Add( pout );
      m_object->MODebug2->Message("added outlet vertical graph! "+moText(label)+ " zone: " + FloatToStr(*zone));
      m_object->m_Outports.Add(pout);
      _save_new_config = true;
    } else {
      moOutlet* pout = m_object->GetOutlets()->Get(pidx);
      if (pout)
        m_object->m_Outports.Add(pout);
    }

    for(auto& it : ( (moEffectSoundFaust*) m_object)->myDecoder->fUiItems ) {
      if (it.label==label) {
          it.index = param_index_counter++;
          m_object->MODebug2->Message("feliz coincidencia:"+moText(label));
      }
    }
  }

  // -- soundfiles

  void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {

  }

  // -- metadata declarations

  void declare(FAUSTFLOAT* zone, const char* key, const char* val) {

  }

};

//========================
//  Factory
//========================

moEffectSoundFaustFactory *m_EffectSoundFaustoFactory = NULL;

MO_PLG_API moEffectFactory* CreateEffectFactory(){
	if (m_EffectSoundFaustoFactory==NULL)
		m_EffectSoundFaustoFactory = new moEffectSoundFaustFactory();
	return (moEffectFactory*) m_EffectSoundFaustoFactory;
}

MO_PLG_API void DestroyEffectFactory(){
	delete m_EffectSoundFaustoFactory;
	m_EffectSoundFaustoFactory = NULL;
}

moEffect*  moEffectSoundFaustFactory::Create() {
	return new moEffectSoundFaust();
}

void moEffectSoundFaustFactory::Destroy(moEffect* fx) {
	delete fx;
}


//========================
// SOUND 3D
//========================

bool moEffectSoundFaust::m_bAlutInit = false;

moSoundFaustAL::moSoundFaustAL() {
	m_pData = NULL;
    m_BufferId = -1;
    m_SourceId = -1;
}

moSoundFaustAL::~moSoundFaustAL() {
	Finish();
}

MOboolean moSoundFaustAL::Finish() {
	// Clean up by deleting Source(s) and Buffer(s)

	alSourceStop(m_SourceId);
	alSourcei(m_SourceId, AL_BUFFER, 0);
    alDeleteSources(1, &m_SourceId);
	alDeleteBuffers(1, &m_BufferId);
	return true;
}


MOboolean moSoundFaustAL::Init() {
	m_BufferSize = 0;
	m_ActualSample = 0;
	m_OldSample = 0;
	return true;
}


MOboolean
moSoundFaustAL::BuildEmpty( MOuint p_size ) {

	// Generate an AL Buffer

	alGenBuffers( 1, &m_BufferId );
    moEffectSoundFaust::al_check_error( moText("BuildEmpty Buffer for  "+GetName()) );


  p_size = 0;
	return true;
}

MOboolean
moSoundFaustAL::BuildFromBuffer( MOuint p_size, GLvoid* p_buffer ) {
	//BuildEmpty(p_size);
	p_buffer = NULL;
	return true;
}

MOboolean
moSoundFaustAL::BuildFromFile( const moText& p_filename ) {

	BuildEmpty(0);

  moFile Sfile( p_filename );
/*
	switch(m_SoundType) {
		case MO_SOUNDTYPE_WAV:

			break;
		case MO_SOUNDTYPE_MP3:
			break;
		case MO_SOUNDTYPE_M4A:
			break;
        case MO_SOUNDTYPE_OGG:
			break;
		case MO_SOUNDTYPE_UNDEFINED:
			break;
	}
*/

  ALenum error;
#ifdef MO_FREEALUT
    #ifndef MO_MACOSX
    //m_BufferId = alutCreateBufferFromFile( p_filename  );

    m_pData = (void*)alutLoadMemoryFromFile( p_filename, &m_eBufferFormat, &m_ulDataSize, &m_fFrequency );
    if (m_pData) {
      alBufferData( m_BufferId, m_eBufferFormat, m_pData, m_ulDataSize, (int)m_fFrequency );
        DMessage( " frames (m_ulDataSize): " + IntToStr(m_ulDataSize) );
        DMessage( " samplerate (m_fFrequency): " + IntToStr( (int)m_fFrequency) );
        //DMessage( " channels: " + IntToStr(wsndinfo.channels) );
        DMessage( " format: " + IntToStr( m_eBufferFormat ) );
        switch(m_eBufferFormat) {
          case AL_FORMAT_MONO8:
            DMessage( " AL_FORMAT_MONO8" );
            break;
          case AL_FORMAT_MONO16:
            DMessage( " AL_FORMAT_MONO16" );
            break;
          case AL_FORMAT_STEREO8:
            DMessage( " AL_FORMAT_STEREO8" );
            break;
          case AL_FORMAT_STEREO16:
            DMessage( " AL_FORMAT_STEREO16" );
            break;
          default:
            break;
        }
        //DMessage( " sections: " + IntToStr(wsndinfo.sections) );
        //DMessage( " seekable: " + IntToStr(wsndinfo.seekable) );
    }
    if ((error = alutGetError()) != ALUT_ERROR_NO_ERROR)
    {
        MODebug2->Error( "moSoundFaustAL::BuildFromFile > alutCreateBufferFromFile failed: " + IntToStr( error ) );
        // Delete Buffers
        //alDeleteBuffers(NUM_BUFFERS, buffers);
        return false;
    }
    #else
    SF_INFO wsndinfo;
    SNDFILE* wsndfile = sf_open( p_filename, SFM_READ, &wsndinfo );
    if (wsndfile) {
        DMessage( " frames: " + IntToStr(wsndinfo.frames) );
        DMessage( " samplerate: " + IntToStr(wsndinfo.samplerate) );
        DMessage( " channels: " + IntToStr(wsndinfo.channels) );
        DMessage( " format: " + IntToStr(wsndinfo.format) );
        DMessage( " sections: " + IntToStr(wsndinfo.sections) );
        DMessage( " seekable: " + IntToStr(wsndinfo.seekable) );
        if (wsndinfo.samplerate)
            DMessage( " seconds: " + FloatToStr( (float)wsndinfo.frames/(float)wsndinfo.samplerate) );
        long num_items = wsndinfo.frames * wsndinfo.channels;
        long numread = 0;
        short* sdata = new short [num_items*2];
        int i=0;
        DMessage( " num_items: " + IntToStr(num_items) );
        if ( sdata ) {
            int readcount;
            numread = sf_read_short( wsndfile, sdata, num_items );
            short min = 65535;
            short max = 0;
            for(; i<num_items; ++i) {
                if (sdata[i]>max) max = sdata[i];
                if (sdata[i]<min) min = sdata[i];
            }
            DMessage( " numread: " + IntToStr(numread) + " min:" + IntToStr(min)+ " max:" + IntToStr(max) );
            if (numread==num_items) {
                DMessage( " Loaded OK! " );
                //alBufferData( helloBuffer, AL_FORMAT_MONO16, samples, buf_size, sample_rate);
                alBufferData( m_BufferId, AL_FORMAT_STEREO16, sdata, num_items*2, wsndinfo.samplerate );
                moEffectSoundFaust::al_check_error("alBufferData");

            }
        }
        if (sdata) delete [] sdata;
    } else {
        DError( moText(sf_strerror(wsndfile)));
    }
    #endif
#else
  m_BufferId = 0;
#endif


  MODebug2->Message("moSoundFaustAL::BuildFromFile > Loaded Buffer from file: " + p_filename );

  alGetBufferi( m_BufferId, AL_SIZE, &m_BufferSize );
  moEffectSoundFaust::al_check_error("alGetBufferi AL_SIZE");
  ALint bcount,channels, frequency;
  alGetBufferi(m_BufferId, AL_SIZE, &m_ulBufferSize);
  m_AudioFormat.m_BufferSize = m_ulBufferSize;
  alGetBufferi(m_BufferId, AL_BITS, &bcount);
  m_AudioFormat.m_BitCount = bcount;
  alGetBufferi(m_BufferId, AL_CHANNELS, &channels);
  m_AudioFormat.m_Channels = channels;
  alGetBufferi(m_BufferId, AL_FREQUENCY, &frequency);
  m_AudioFormat.m_SampleRate = frequency;

  MODebug2->Message("moSoundFaustAL::BuildFromFile > Audioformat.m_BufferSize: " + IntToStr(m_BufferSize)
                    + " Audioformat.m_BitCount:" + IntToStr(m_AudioFormat.m_BitCount)
                    + " Audioformat.m_Channels:" + IntToStr(m_AudioFormat.m_Channels)
                    + " Audioformat.m_SampleRate:" + IntToStr(m_AudioFormat.m_SampleRate) );

    // Generate a Source to playback the Buffer
    alGenSources( 1, &m_SourceId );
    moEffectSoundFaust::al_check_error("Generating source");
    alSourcei( m_SourceId, AL_BUFFER, m_BufferId );
    moEffectSoundFaust::al_check_error("Attaching buffer to source");
    MODebug2->Message("moSoundFaustAL::BuildFromFile > buffersize: " + IntToStr(m_BufferSize) + " bufferid:" + IntToStr(m_BufferId) + " Attached to source:" + IntToStr(m_SourceId) );
/*
    alSourcePlay( m_SourceId );
    //alSourceStop( m_SourceId );
    moEffectSoundFaust::al_check_error("Playing source");

    ALenum current_playing_state;

    alGetSourcei( m_SourceId, AL_SOURCE_STATE, & current_playing_state);
    moEffectSoundFaust::al_check_error("alGetSourcei AL_SOURCE_STATE");

    if (AL_PLAYING == current_playing_state) {
        DMessage("Playing!!!!");

    }
    float spos;
    float fr = m_AudioFormat.m_SampleRate*m_AudioFormat.m_Channels*2;

    Update();
    if (m_ActualSample<3000000) {
        alSourcei(m_SourceId, AL_BYTE_OFFSET, 3000000+1);
        DMessage("Goto:" + IntToStr(3000000) );
    }

    while (AL_PLAYING == current_playing_state) {

        //printf("still playing ... so sleep\n");

        //alutSleep(1);   // should use a thread sleep NOT sleep() for a more responsive finish

        //alutSleep(1);
        alGetSourcei( m_SourceId , AL_BYTE_OFFSET , &m_ActualSample);

        spos = m_ActualSample;

        if (m_ActualSample!=m_OldSample)
            DMessage( "Playing at: " + IntToStr( m_ActualSample ) + " pos (s):" + FloatToStr(spos/fr,2,2) );

        m_OldSample = m_ActualSample;

        if (m_ActualSample>6000000) {
            DMessage("Goto:" + IntToStr(3000000 + 1) );
            //PlaySample( 3000000 + 1 );
            alSourcei(m_SourceId, AL_BYTE_OFFSET, 3000000+1);
        }

        alGetSourcei( m_SourceId, AL_SOURCE_STATE, & current_playing_state);
        //alGetSourcei( m_SourceId, AL_SOURCE_, & current_playing_state);
        moEffectSoundFaust::al_check_error("alGetSourcei AL_SOURCE_STATE");
    }
 */
    alSourceStop( m_SourceId );
    return true;

}

void moSoundFaustAL::Play() {
	// Start playing source

	alSourcePlay(m_SourceId);

}


void moSoundFaustAL::PlaySample( MOint sampleid ) {

    Update();//update actual state and sample

	//(m_SourceId);
	if (m_ActualSample!=sampleid && sampleid!=0 && (State() != MO_STREAMSTATE_PLAYING)  ) {
		alSourcei(m_SourceId, AL_BYTE_OFFSET, sampleid);
		alSourcePlay(m_SourceId);
		m_ActualSample = sampleid;
	}

	ALenum error = alGetError();
	if (error!=AL_NO_ERROR) {
    MODebug2->Error("moSoundFaustAL::PlaySample > playing sample id: " + IntToStr(sampleid) + " error:" + IntToStr(error) );
	}

}

void moSoundFaustAL::Update() {

    State();
    m_OldSample = m_ActualSample;
    alGetSourcei( m_SourceId , AL_BYTE_OFFSET , &m_ActualSample);

}

void moSoundFaustAL::Pause() {

	alSourcePause(m_SourceId);

}

void moSoundFaustAL::Stop() {
	// Stop the Source and clear the Queue

	alSourceStop(m_SourceId);


}

void moSoundFaustAL::Rewind() {

	alSourceRewind(m_SourceId);

}


void moSoundFaustAL::Final() {

	Update();
}

void moSoundFaustAL::Frame(int frame) {
	Update();
}

void moSoundFaustAL::Repeat(int repeat) {
    alSourcei(m_SourceId, AL_LOOPING, repeat==1 );
}

moStreamState moSoundFaustAL::State() {
	// Get Source State
	alGetSourcei( m_SourceId, AL_SOURCE_STATE, &m_iAlState);
	switch(m_iAlState) {
      case AL_INITIAL:
        return MO_STREAMSTATE_READY;
        break;

      case AL_PLAYING:
        return MO_STREAMSTATE_PLAYING;
        break;

      case AL_PAUSED:
        return MO_STREAMSTATE_PAUSED;
        break;

      case AL_STOPPED:
        return MO_STREAMSTATE_STOPPED;
        break;
	}

  return MO_STREAMSTATE_UNKNOWN;
}


bool
moSoundFaustAL::IsPlaying() {
  return (State()== MO_STREAMSTATE_PLAYING);
}


void moSoundFaustAL::SetPosition( float x, float y, float z ) {

	alSource3f( m_SourceId, AL_POSITION, x, y, z );
  float listenerPos[ 3 ] = { 0.0, 0.0, 0.0 };
  float listenerOri[ 6 ] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
  float listenerVel[ 3 ] = { 0.0, 0.0, 0.0  };
  alListenerfv( AL_POSITION, listenerPos );
  alListenerfv( AL_VELOCITY, listenerVel );
  alListenerfv( AL_ORIENTATION, listenerOri );
  //alSourcei( m_SourceId, AL_SOURCE_RELATIVE, true );
  //alSourcef( m_SourceId, AL_DOPPLER_FACTOR, 2.0 );
  //alSourcef( m_SourceId, AL_DOPPLER_VELOCITY, 1.0 );
}

void moSoundFaustAL::SetVelocity( float x, float y, float z ) {

	alSource3f( m_SourceId, AL_VELOCITY, x, y, z );

}

void moSoundFaustAL::SetDirection( float x, float y, float z ) {

	alSource3f( m_SourceId, AL_DIRECTION, x, y, z );

}

void moSoundFaustAL::SetVolume( float volume ) {

	alSourcef( m_SourceId, AL_GAIN, volume );

}

float moSoundFaustAL::GetVolume() {

	alGetSourcef( m_SourceId, AL_GAIN, &m_Volume );

	return m_Volume;

}

float moSoundFaustAL::GetActualSampleVolume() {
  float avolume = 0;
  if (m_pData) {
    //MODebug2->Message("actualsample:"+IntToStr( m_ActualSample )+"/"+IntToStr(m_ulDataSize) );
    if (m_ActualSample < m_ulDataSize  ) {
      /*int indexp = m_ActualSample / (2/m_AudioFormat.m_Channels);
      //avolume = (float) ((WORD*)m_pData)[ indexp ];
      avolume = 32000;
      avolume = (1.0f*fabs(avolume)) / (65535.0/2.0);
      */
    }

  }
  return avolume;
}

void moSoundFaustAL::SetPitch( float pitch )  {
    m_Pitch = pitch;

    alSourcef(m_SourceId, AL_PITCH, pitch);

}


float moSoundFaustAL::GetPitch()  {

  alGetSourcef( m_SourceId, AL_PITCH, &m_Pitch );

  return m_Pitch;
}

void moSoundFaustAL::SetLoop( bool loop ) {

  //alSource3f();
  alSourcei( m_SourceId, AL_LOOPING, loop);
}


void
moSoundFaustAL::SetSpeedOfSound( float speedofsound ) {
  //alSourcei( m_SourceId, AL_SPEED_OF_SOUND, speedofsound );
  alSpeedOfSound( speedofsound );
}



void moSoundFaustAL::SetBalance( float balance ) {
    //DMessage("SetBalance for "+GetName()+" balance:" + FloatToStr(balance) );
    //alSourcef( m_SourceId, AL_BALANCE, balance );
    float listenerPos[ 3 ] = { 0.0, 0.0, 0.0 };
    float listenerOri[ 6 ] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
    float listenerVel[ 3 ] = { 0.0, 0.0, 0.0  };
    alListenerfv( AL_POSITION, listenerPos );
    alListenerfv( AL_VELOCITY, listenerVel );
    alListenerfv( AL_ORIENTATION, listenerOri );

    float sourcePos[ 3 ] = { balance*10.0, 0.0, 0.0 };
    alSourcefv(m_SourceId, AL_POSITION, sourcePos);

    m_Balance = balance;
}


//========================
//  Efecto
//========================
moEffectSoundFaust::moEffectSoundFaust() {

	SetName("soundfaust");
	m_pSM = NULL;
  m_pAudio = NULL;
    m_pALCContext = NULL;
    m_pALCDevice = NULL;
    i_PlayedTimes = 0;

}

moEffectSoundFaust::~moEffectSoundFaust() {
	Finish();
}

int moEffectSoundFaust::al_check_error(const char * p_message) { // generic OpenAL error checker

    ALenum al_error;
    al_error = alGetError();

    if(AL_NO_ERROR != al_error) {

        printf("OPENAL ERROR - %s  (%s)\n", alGetString(al_error), p_message);
        return al_error;
    }
    return 0;
}

int moEffectSoundFaust::alutCheckError(const char * p_message) { // generic OpenAL error checker

    ALenum alut_error;
    alut_error = alutGetError();

    if( ALUT_ERROR_NO_ERROR != alut_error) {

        printf("ALUT ERROR - %s  (%s)\n", alutGetErrorString(alut_error), p_message);
        return alut_error;
    }
    return 0;
}

void moEffectSoundFaust::ShowBufferInfo( ALint p_BufferId ) {

    ALint bcount,channels, frequency, bsize;
    alGetBufferi(p_BufferId, AL_SIZE, &bsize);
    al_check_error("failed call to alGetBufferi AL_SIZE");
    alGetBufferi(p_BufferId, AL_BITS, &bcount);
    al_check_error("failed call to alGetBufferi AL_BITS");
    alGetBufferi(p_BufferId, AL_CHANNELS, &channels);
    al_check_error("failed call to alGetBufferi AL_CHANNELS");
    alGetBufferi(p_BufferId, AL_FREQUENCY, &frequency);
    al_check_error("failed call to alGetBufferi AL_FREQUENCY");

    DMessage("ShowBufferInfo > size: " + IntToStr(bsize)
            + " bitcount:" + IntToStr(bcount)
            + " channels:" + IntToStr(channels)
            + " frequency:" + IntToStr(frequency) );

}

MOboolean moEffectSoundFaust::Init() {

#ifdef MO_MACOSX
    m_pALCDevice = alcOpenDevice( alcGetString(NULL, ALC_DEVICE_SPECIFIER) );
    if (!m_pALCDevice) {
        DError("Init > alcOpenDevice no device created!");
        return false;
    } else {
        DMessage( moText("Init > alcOpenDevice device:") + IntToStr((long)m_pALCDevice) );
    }
#endif

    /*
    ALboolean enumeration;

    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE) {
        // enumeration not supported
        DError("Init > enumeration not supported.");
    } else {
        // enumeration supported
        DMessage("Init > enumerating devices:");
        const ALCchar *ldevices;
        ldevices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        const ALCchar *ldevice = ldevices, *next = ldevices + 1;
        size_t len = 0;
        while (device && *ldevice != '\0' && next && *next != '\0') {
            DMessage( moText("device:") + moText((char*)ldevice) );
            len = strlen(ldevice);
            ldevice += (len + 1);
            next += (len + 2);
        }

    }
    */

#ifdef MO_MACOSX
    m_pALCContext = alcCreateContext(m_pALCDevice, NULL);
    if (!alcMakeContextCurrent(m_pALCContext)) {
        DError("Init > alcMakeContextCurrent Error!");
        return false;
    } else {
        DMessage( moText("Init > context:") + IntToStr((long)m_pALCContext) );
    }
#endif

#ifdef MO_FREEALUT

   //if ( alutInitWithoutContext( NULL, NULL )) {

   if (m_bAlutInit==false) {
      m_bAlutInit = alutInit( NULL, NULL );
   }

    if ( m_bAlutInit ) {

      DMessage("Init > ALUT Initialized!");

      //alDistanceModel( AL_INVERSE_DISTANCE );
      //alDistanceModel( AL_INVERSE_DISTANCE );
      //MM_render_one_buffer();
      //generate al id
      //generate al buffer
      //MM_render_one_buffer();
  #ifndef MO_MACOSX
  /*
      helloBuffer = alutCreateBufferHelloWorld ();
      alutCheckError( "alutCreateBufferHelloWorld" );
      DMessage( "helloBuffer:"+ IntToStr(helloBuffer) );

      //al_check_error("failed call to alutCreateBufferHelloWorld");
      //associate buffer to al id source
      if (helloBuffer) {
        ShowBufferInfo(helloBuffer);
        alGenSources (1, &helloSource);
        al_check_error("failed call to alGenSources");
        alSourcei (helloSource, AL_BUFFER, helloBuffer);
        alSourcePlay(helloSource);
      }
    */
  #endif

  } else {
      DError("Init > ALUT not initialized. Check if ALUT is installed correctly.");
      //return false;
  }
#endif


  m_SoundFaustFilename = moText("");

  m_pSM = m_pResourceManager->GetSoundMan();
  if (!m_pSM) {
    return false;
  }

  if (!PreInit()) return false;

  moDefineParamIndex( SOUNDFAUST_ALPHA, moText("alpha") );
  moDefineParamIndex( SOUNDFAUST_COLOR, moText("color") );
  moDefineParamIndex( SOUNDFAUST_SYNC, moText("syncro") );
  moDefineParamIndex( SOUNDFAUST_PHASE, moText("phase") );
  moDefineParamIndex( SOUNDFAUST_SOUND, moText("sound") );

  moDefineParamIndex( SOUNDFAUST_SEEKPOSITION, moText("seekposition") );

  moDefineParamIndex( SOUNDFAUST_MODE, moText("mode") );
  moDefineParamIndex( SOUNDFAUST_LOOP, moText("loop") );
  moDefineParamIndex( SOUNDFAUST_LAUNCH, moText("launch") );
  moDefineParamIndex( SOUNDFAUST_SPEEDOFSOUND, moText("speedofsound") );
  moDefineParamIndex( SOUNDFAUST_PITCH, moText("pitch") );
  moDefineParamIndex( SOUNDFAUST_VOLUME, moText("volume") );
  moDefineParamIndex( SOUNDFAUST_BALANCE, moText("balance") );

  moDefineParamIndex( SOUNDFAUST_TEXTURE, moText("texture") );
  moDefineParamIndex( SOUNDFAUST_BLENDING, moText("blending") );

  moDefineParamIndex( SOUNDFAUST_SCALEX, moText("scalex") );
  moDefineParamIndex( SOUNDFAUST_SCALEY, moText("scaley") );
  moDefineParamIndex( SOUNDFAUST_SCALEZ, moText("scalez") );

  moDefineParamIndex( SOUNDFAUST_TRANSLATEX, moText("translatex") );
  moDefineParamIndex( SOUNDFAUST_TRANSLATEY, moText("translatey") );
  moDefineParamIndex( SOUNDFAUST_TRANSLATEZ, moText("translatez") );

  moDefineParamIndex( SOUNDFAUST_SPEEDX, moText("speedx") );
  moDefineParamIndex( SOUNDFAUST_SPEEDY, moText("speedy") );
  moDefineParamIndex( SOUNDFAUST_SPEEDZ, moText("speedz") );

  moDefineParamIndex( SOUNDFAUST_DIRECTIONX, moText("directionx") );
  moDefineParamIndex( SOUNDFAUST_DIRECTIONY, moText("directiony") );
  moDefineParamIndex( SOUNDFAUST_DIRECTIONZ, moText("directionz") );

  moDefineParamIndex( SOUNDFAUST_INLET, moText("inlet") );
  moDefineParamIndex( SOUNDFAUST_OUTLET, moText("outlet") );

  try {

    myDecoder = NULL;

  #ifdef DYNAMIC_DSP
      m_pDspFaust = new DspFaust(argv[1], 44100, 512);
      DMessage("Init > Dynamic Dsp Faust Created");
  #else
      m_pDspFaust = new DspFaust(44100, 512);
      DMessage("Init > Static Dsp Faust Created");
  #endif
    if (m_pDspFaust) {
      m_pDspFaust->start();
      m_pDspParamsCount = m_pDspFaust->getParamsCount();
      m_cuijson = m_pDspFaust->getJSONUI();
      DMessage( moText(m_cuijson.c_str()) );
      DMessage(moText(m_pDspFaust->getJSONMeta()));
      myDecoder = new JSONUIFloatDecoder( m_cuijson );


      //DMessage(IntToStr(myDecoder->getNumInputs());
      moUISoundFaust* pUI = new moUISoundFaust(this);
      if (myDecoder && pUI)
        myDecoder->buildUserInterface(pUI);

      bool _save_new_config = pUI->_save_new_config;
      /*
      for (int p = 0; p < m_pDspParamsCount; p++) {
          //std::string msg = UIJSON.get<std::string>("items.complex.path");
          moText ttype = m_pDspFaust->getMetadata(p, "tooltip")+moText("ID")+IntToStr(p);
          moText inout = ttype;
          moText plabel = m_pDspFaust->getMetadata(p, "label");
          APIUI::ItemType t;
          inout  = moText("in ")+inout;
          moText p_type = "NUMERIC";
          moText o_type = "NUMERIC";
          if (ttype=="button") { t = APIUI::ItemType::kButton; p_type = "NUMERIC"; }
          if (ttype=="checkbutton") { t = APIUI::ItemType::kButton; p_type = "NUMERIC"; }
          if (ttype=="vslider") { t = APIUI::ItemType::kVSlider; p_type = "FUNCTION"; }
          if (ttype=="hslider") { t = APIUI::ItemType::kHSlider; p_type = "FUNCTION"; }
          if (ttype=="numentry") { t = APIUI::ItemType::kNumEntry; p_type = "NUMERIC"; }

          if (ttype=="hbargraph") t = APIUI::ItemType::kHBargraph;
          if (ttype=="vbargraph") t = APIUI::ItemType::kVBargraph;
          //{ kButton = 0, kCheckButton, kVSlider, kHSlider, kNumEntry, kHBargraph, kVBargraph };
          switch(t) {
            case APIUI::ItemType::kButton:
            case APIUI::ItemType::kCheckButton:
            case APIUI::ItemType::kVSlider:
            case APIUI::ItemType::kHSlider:
            case APIUI::ItemType::kNumEntry:
              {
                //input values, make parameters
                moParamDefinition pdef( plabel, p_type );
                int pidx = m_Config.GetParamIndex(plabel);
                if (pidx<0) { m_Config.CreateParam( pdef ); _save_new_config = true; }
              }
              break;

            case APIUI::ItemType::kHBargraph:
            case APIUI::ItemType::kVBargraph:
              {
                //make outlets of them
                int pidx = GetOutletIndex(plabel);
                if (pidx<0) {
                  moOutlet* pout = new moOutlet();
                  pout->SetMoldeoLabelName( GetLabelName() );
                  pout->Init( plabel, GetOutlets()->Count(), o_type);
                  //pout->Init( plabel, i, m_Config.GetParam(plabel).GetPtr());
                  GetOutlets()->Add( pout );
                  _save_new_config = true;
                }
              }
              break;

            default:
            //nothing
              break;
          }
          DMessage(
                    "Type: " + inout +
                    "Label: " + plabel +
                    "Address: " + moText(m_pDspFaust->getParamAddress(p)) +
                    "Value: "   + FloatToStr(float(m_pDspFaust->getParamValue(p))) +
                    "Min: " + FloatToStr(float(m_pDspFaust->getParamMin(p))) +
                    "Max: " + FloatToStr(float(m_pDspFaust->getParamMax(p)))
                  );
      }*/
      if (_save_new_config) m_Config.SaveConfig();
    }
  } catch(...) {
    DError("DspFaust failed");
  }


  UpdateParameters();

  last_ticks = 0;
  m_UserPosition = 0;

  return true;
}

MOboolean moEffectSoundFaust::Finish()
{
  #ifdef MO_MACOSX

    if (m_pALCContext) {
        alcDestroyContext(m_pALCContext);
        m_pALCContext = NULL;
    }

    if (m_pALCDevice) {
        alcCloseDevice(m_pALCDevice);
        m_pALCDevice = NULL;
    }
  #endif

#ifdef MO_FREEALUT
    alutExit ();
#endif
    return PreFinish();
}


void moEffectSoundFaust::MM_render_one_buffer() {


    alGenBuffers( 1, &helloBuffer );
    al_check_error("failed call to alGenBuffers");

    // fill buffer with an audio curve
    // float freq = 440.f;
    float freq = 100.f;
    float incr_freq = 0.1f;

    int seconds = 4;	// this determines buffer size based on seconds of playback audio independent of sample rate
    // unsigned sample_rate = 22050;
    unsigned sample_rate = 44100;
    unsigned int channels = 2;
    double my_pi = 3.14159;
    size_t buf_size = seconds * sample_rate * channels;

    //short * samples = (short *)malloc(sizeof(short) * buf_size); // allocates audio buffer memory
    short * samples = new short[buf_size];

    printf("\nhere is freq %f\n", freq);

    int i=0;
    for(; i<buf_size; ++i) {



        samples[i] = 32760 * sin( (2.f * my_pi * freq)/sample_rate * i ); // populate audio curve data point

        //if (i%2==0) samples[i] = samples[i];

        freq += incr_freq; // jack around frequency ... just comment out this for sin curve

        if (10.0 > freq || freq > 5000.0) { // toggle frequency increment if we reach min or max freq

            incr_freq *= -1.0f;
        }
    }

    // upload buffer to OpenAL
    if (channels==1) alBufferData( helloBuffer, AL_FORMAT_MONO16, samples, buf_size, sample_rate);
    if (channels==2) alBufferData( helloBuffer, AL_FORMAT_STEREO16, samples, buf_size, sample_rate);
    al_check_error("populating alBufferData");

    free(samples); // release audio buffer memory after above upload into OpenAL internal buffer

    /// Set-up sound source and play buffer

    //alGenSources(1, & streaming_source[0]);
    //alSourcei(streaming_source[0], AL_BUFFER, helloBuffer);
    alGenSources (1, &helloSource);
    al_check_error("failed call to alGenSources");
    alSourcei (helloSource, AL_BUFFER, helloBuffer);
    alSourcePlay(helloSource);

    // ---------------------

    ALenum current_playing_state;
    alGetSourcei( helloSource, AL_SOURCE_STATE, & current_playing_state);
    al_check_error("alGetSourcei AL_SOURCE_STATE");

    while (AL_PLAYING == current_playing_state) {

        //printf("still playing ... so sleep\n");

        //alutSleep(1);   // should use a thread sleep NOT sleep() for a more responsive finish
        alGetSourcei( helloSource, AL_SOURCE_STATE, & current_playing_state);
        al_check_error("alGetSourcei AL_SOURCE_STATE");
    }

    printf("end of playing\n");


}


MOboolean
moEffectSoundFaust::ResolveValue( moParam& param, int value_index, bool p_refresh ) {

  int idx = -1;
  moValue& value( param.GetValue(value_index) );
  moParamType param_type = param.GetParamDefinition().GetType();
  //MODebug2->Message( moText("+Init value #") + IntToStr(v) );

  if (value.GetSubValueCount()<=0) {
      return false;
  }

  moValueBase& valuebase0(value.GetSubValue(0));

  switch( param_type ) {

      case MO_PARAM_SOUND:
          if (value.GetSubValueCount()>0) {
            if (valuebase0.Text()!="") {
              UpdateSound( valuebase0.Text() );
              if (m_pAudio) {
                  moSound* m_pPrevAudio = valuebase0.Sound();
                  if (m_pPrevAudio) m_pPrevAudio->Stop();
                  valuebase0.SetSound( m_pAudio );
                  return true;
              }
              return false;
            }
          }
          break;
      default:
        break;

  }//fin siwtch


  return moMoldeoObject::ResolveValue( param, value_index, p_refresh );
}

void
moEffectSoundFaust::UpdateParameters() {

  for (int p = 0; p < 15; ++p) {
      //DMessage( moText("B")+IntToStr(p) + moText(":")+ FloatToStr(float(m_pDspFaust->getParamValue(p))) );
  }
  if (myDecoder) {
    int zp = 0;
    for( ExtZoneParam* z : myDecoder->getOutputControls()) {
      //cout << zoneparam << endl;
      JSONUIFloatDecoder::ZoneParam* zpf = (JSONUIFloatDecoder::ZoneParam*) z;
      float value = m_pDspFaust->getParamValue( zp );
      float min = m_pDspFaust->getParamMin( zp );
      float max = m_pDspFaust->getParamMax( zp );
      float norVal = (value - min) / ( max - min);
      moOutlet *pout = m_Outports.Get(zp);
      if (pout) {
        //DMessage(moText("Updating outlet: ") + pout->GetConnectorLabelName());
        for(int k=0; k<pout->GetConnections()->Count();k++) {
          moConnection* pc = pout->GetConnections()->Get(k);
          //DMessage(moText("to: ") + pc->GetDestinationMoldeoLabelName() + ">" + pc->GetDestinationConnectorLabelName());
        }
        pout->GetData()->SetDouble(value);
        pout->Update(true);
      }
      zp++;
      //DMessage("output zone param p: "+IntToStr(zp)+" zone: " + FloatToStr(zpf->fZone)+" index: "+IntToStr(zpf->fIndex)+ " value: "+FloatToStr(value) );
    }


    float input_level[2];
    input_level[0] = m_Config.Eval("Level Averaging Time");
    input_level[1] = m_Config.Eval("Level dB Offset");
    int zp_ref = zp;

    for( ExtZoneParam* z : myDecoder->getInputControls()) {
      //cout << zoneparam << endl;
      JSONUIFloatDecoder::ZoneParam* zpf = (JSONUIFloatDecoder::ZoneParam*) z;

      float value = m_pDspFaust->getParamValue( zp );
      int inp_idx = zp-zp_ref;
      float newvalue = input_level[zp-zp_ref];
      m_pDspFaust->setParamValue( zp, newvalue );
      zp++;
      /*DMessage("input zone param p: "+IntToStr(zp)+" zone: "
              + FloatToStr(zpf->fZone)+" index: "
              +IntToStr(zpf->fIndex)+ " value: "
              +FloatToStr(value)+ " newvalue:"+FloatToStr(newvalue) );*/
    }
    /**
    for(auto& it : myDecoder->fUiItems ) {
      float value = m_pDspFaust->getParamValue( it.address.c_str() );
      DMessage("item info: type: "+moText(it.type.c_str())
                + " label: " + moText(it.label.c_str())
                + " index:"+ IntToStr(it.index)
                + " value:"+ FloatToStr(value)
                + " address:" + moText(it.address.c_str()) );
    }*/

  }

  UpdateSound( m_Config.Text( moR(SOUNDFAUST_SOUND)) );

  m_bLoop = m_Config.Int( moR( SOUNDFAUST_LOOP ) ) == 1;
  m_fPitch = m_Config.Eval( moR( SOUNDFAUST_PITCH ) );
  m_fVolume = m_Config.Eval( moR( SOUNDFAUST_VOLUME ) );
  m_fSpeedOfSound = m_Config.Eval( moR( SOUNDFAUST_SPEEDOFSOUND ) );
  //MODebug2->Message("Pitch:" + FloatToStr(m_fPitch) );
  m_vScale = moVector3f( m_Config.Eval(moR(SOUNDFAUST_SCALEX)),
                            m_Config.Eval(moR(SOUNDFAUST_SCALEY)),
                            m_Config.Eval(moR(SOUNDFAUST_SCALEZ)) );

  m_vPosition = moVector3f( m_Config.Eval(moR(SOUNDFAUST_TRANSLATEX)),
                            m_Config.Eval(moR(SOUNDFAUST_TRANSLATEY)),
                            m_Config.Eval(moR(SOUNDFAUST_TRANSLATEZ)) );

  m_vSpeed = moVector3f( m_Config.Eval(moR(SOUNDFAUST_SPEEDX)),
                            m_Config.Eval(moR(SOUNDFAUST_SPEEDY)),
                            m_Config.Eval(moR(SOUNDFAUST_SPEEDZ)) );

  m_vDirection = moVector3f( m_Config.Eval(moR(SOUNDFAUST_DIRECTIONX)),
                            m_Config.Eval(moR(SOUNDFAUST_DIRECTIONY)),
                            m_Config.Eval(moR(SOUNDFAUST_DIRECTIONZ)) );

  if (m_pAudio) {
    m_pAudio->SetLoop( m_bLoop );
    m_pAudio->SetPitch( m_fPitch );
    m_pAudio->SetVolume( m_fVolume );
    m_pAudio->SetSpeedOfSound( m_fSpeedOfSound );
    m_pAudio->SetDirection( m_vDirection );
    m_pAudio->SetVelocity( m_vSpeed );
    m_pAudio->SetPosition( m_vPosition );

    m_pAudio->Update();
    m_fSampleVolume = 0.0;
    //m_fSampleVolume = m_pAudio->GetActualSampleVolume();
    //MODebug2->Message("m_fSampleVolume:"+FloatToStr( m_fSampleVolume )  );
    //MODebug2->Message("m_bLoop:"+IntToStr( m_bLoop )  );

  }





}


void
moEffectSoundFaust::UpdateSound( const moText& p_newfilename ) {

    moDataManager* pDataMan = m_pResourceManager->GetDataMan();
    moFile SoundFile;
    moSoundFaustAL* prev_Audio  = NULL;
    if (m_SoundFaustFilename!=p_newfilename ) {

        m_SoundFaustFilename = p_newfilename;
        if (m_pAudio) {
          prev_Audio = m_pAudio;
        }
        m_pAudio = (moSoundFaustAL*)m_pSM->GetSound( m_SoundFaustFilename, false );

        if (m_pAudio) {
            if (prev_Audio) {
              prev_Audio->Stop();
            }
        } else {

          SoundFile = pDataMan->GetDataFile( m_SoundFaustFilename );
          m_SoundFaustFilenameFull = SoundFile.GetCompletePath();
          m_pAudio = new moSoundFaustAL();
          if (m_pAudio) {
            m_pAudio->Init();
            //m_pAudio->Init( m_SoundFaustFilename, m_SoundFaustFilenameFull );
            m_pAudio->SetName( m_SoundFaustFilename );
            m_pAudio->SetFileName( m_SoundFaustFilenameFull );

            MODebug2->Message("moEffectSoundFaust::UpdateSound > New audio building from file: " + m_pAudio->GetName() );
              if (m_pAudio->BuildFromFile( m_pAudio->GetFileName() ) ) {
                  DMessage("Adding sound to SoundManager!");
                    m_pSM->AddSound( (moSound*) m_pAudio );
              }

          }

        }

    }

    double mUserPosition = m_Config.Eval(moR(SOUNDFAUST_SEEKPOSITION));

    if (mUserPosition!=m_UserPosition) {
      //MODebug2->Push(" mUserPosition:" + FloatToStr(mUserPosition) + "/" + FloatToStr(m_UserPosition) );
      //Sound->Seek( (MOulong)mUserPosition, 1.0 );
      if (m_pAudio) {
        //TODO: seek position in frame al sound buffer
        //m_pAudio->
      }
      m_UserPosition = mUserPosition;
    }

    double newflaun = m_Config.Eval(moR(SOUNDFAUST_LAUNCH));
    if (m_fLaunch != newflaun && m_pAudio) {

      DMessage("Launching sound!" + m_pAudio->GetName());
      //m_bLaunch = m_Config.Int(moR(SOUNDFAUST_LAUNCH));
      m_fLaunch = newflaun;
        if (m_fLaunch>=1.0 && moIsTimerPlaying() && m_EffectState.tempo.Started()) {
          m_pAudio->Play();
        }
    }

    if (m_bLoop) {
        if (  ( moIsTimerStopped()
              || !m_EffectState.tempo.Started()
              || !Activated()
              || !m_EffectState.Activated() )
          && m_pAudio ) {

          if (m_pAudio->IsPlaying())
            m_pAudio->Stop();

        }

        if (m_fLaunch>=1.0 && m_pAudio && Activated() && m_EffectState.Activated() ) {
          if (!m_pAudio->IsPlaying() && moIsTimerPlaying() && m_EffectState.tempo.Started() ) {
            m_pAudio->Play();
          }
        }
    } else {
        int ntimes = 1;
        if (i_PlayedTimes==0) {
            if (m_fLaunch>=1.0 && m_pAudio && Activated() && m_EffectState.Activated() ) {
              if (!m_pAudio->IsPlaying() && moIsTimerPlaying() && m_EffectState.tempo.Started() ) {
                m_pAudio->Play();
                i_PlayedTimes = 1;
              }
            }
        } else {
            if ( ( moIsTimerStopped() || !m_EffectState.tempo.Started()  || !Activated() || !m_EffectState.Activated() ) && m_pAudio ) {
              if (m_pAudio->IsPlaying()) {
                m_pAudio->Stop();
              }
              i_PlayedTimes = 0;
            }
        }
    }
}

void moEffectSoundFaust::Draw( moTempo* tempogral, moEffectState* parentstate )
{
    MOint indeximage;
    MOdouble PosTextX0, PosTextX1, PosTextY0, PosTextY1;
    int ancho,alto;
    int w = m_pResourceManager->GetRenderMan()->ScreenWidth();
    int h = m_pResourceManager->GetRenderMan()->ScreenHeight();

    PreDraw( tempogral, parentstate);


    /** Callling UpdateParameters() in ::Update(), no need here*/
    // UpdateParameters();

    /*

    */


    /*
    if ( tempogral->ticks>1 && !moTimeManager::MoldeoTimer->Paused() && !m_bAudioStarted ) {
        m_Audio.Seek( tempogral->ticks );
        m_Audio.Play();
        m_bAudioStarted = true;
    } else if ( tempogral->ticks==0 && !moTimeManager::MoldeoTimer->Started() ) {
        m_Audio.Stop();
        m_bAudioStarted = false;
    }

    if (  last_ticks == tempogral->ticks ) {
        m_Audio.Pause();
        m_Audio.Seek( tempogral->ticks );
        m_bAudioStarted = false;
    }
    */
    /*
    moStreamState audiostate = m_Audio.GetState();
    moText  textstate = m_Audio.StateToText( audiostate );
    long    audioposition;

    last_ticks = moGetTicks();
*/
    /*audioposition = m_Audio.GetPosition();



    if ( (long) abs(audioposition - last_ticks) > 8000 ) {
       if (audiostate!=MO_STREAMSTATE_STOPPED)
           m_Audio.Seek( last_ticks );
           audioposition = last_ticks;

    }

    moSetDuration( audioposition );

    if ( moIsTimerPaused() ) {

        if (audiostate!=MO_STREAMSTATE_PAUSED)
            m_Audio.Pause();

    } else {

        if ( moIsTimerPlaying() ) {
            if (audiostate!=MO_STREAMSTATE_PLAYING)
                m_Audio.Play();
        } else {
            if (audiostate!=MO_STREAMSTATE_STOPPED)
                m_Audio.Stop();
        }

    }

    // Guardar y resetar la matriz de vista del modelo //
    glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
    glLoadIdentity();									// Reset The View

    // Cambiar la proyeccion para una vista ortogonal //
    glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix
    glOrtho(0,w,0,h,-1,1);                              // Set Up An Ortho Screen


    // Funcion de blending y de alpha channel //
    glEnable(GL_BLEND);

    glDisable(GL_ALPHA);

    // Draw //
    glTranslatef(  ( m_Config[moR(SOUNDFAUST_TRANSLATEX)].GetData()->Fun()->Eval(m_EffectState.tempo.ang)+Tx )*w,
                   ( m_Config[moR(SOUNDFAUST_TRANSLATEY)].GetData()->Fun()->Eval(m_EffectState.tempo.ang)+Ty )*h,
          m_Config[moR(SOUNDFAUST_TRANSLATEZ)].GetData()->Fun()->Eval(m_EffectState.tempo.ang)+Tz);

    glRotatef(  m_Config[moR(SOUNDFAUST_ROTATEX)].GetData()->Fun()->Eval(m_EffectState.tempo.ang), 1.0, 0.0, 0.0 );
    glRotatef(  m_Config[moR(SOUNDFAUST_ROTATEY)].GetData()->Fun()->Eval(m_EffectState.tempo.ang), 0.0, 1.0, 0.0 );
    glRotatef(  m_Config[moR(SOUNDFAUST_ROTATEZ)].GetData()->Fun()->Eval(m_EffectState.tempo.ang), 0.0, 0.0, 1.0 );
    glScalef(   m_Config[moR(SOUNDFAUST_SCALEX)].GetData()->Fun()->Eval(m_EffectState.tempo.ang)*Sx,
                m_Config[moR(SOUNDFAUST_SCALEY)].GetData()->Fun()->Eval(m_EffectState.tempo.ang)*Sy,
                m_Config[moR(SOUNDFAUST_SCALEZ)].GetData()->Fun()->Eval(m_EffectState.tempo.ang)*Sz);

    SetColor( m_Config[moR(SOUNDFAUST_COLOR)][MO_SELECTED], m_Config[moR(SOUNDFAUST_ALPHA)][MO_SELECTED], m_EffectState );

    SetBlending( (moBlendingModes) m_Config[moR(SOUNDFAUST_BLENDING)][MO_SELECTED][0].Int() );

    moTexture* pImage = (moTexture*) m_Config[moR(SOUNDFAUST_TEXTURE)].GetData()->Pointer();

    glBindTexture( GL_TEXTURE_2D, m_Config[moR(SOUNDFAUST_TEXTURE)].GetData()->GetGLId(&m_EffectState.tempo) );

    PosTextX0 = 0.0;
    PosTextX1 = 1.0 * ( pImage!=NULL ? pImage->GetMaxCoordS() :  1.0 );
    PosTextY0 = 0.0;
    PosTextY1 = 1.0 * ( pImage!=NULL ? pImage->GetMaxCoordT() :  1.0 );

    //ancho = (int)m_Config[ moR(SOUNDFAUST_WIDTH) ].GetData()->Fun()->Eval(m_EffectState.tempo.ang)* (float)(w/800.0);
    //alto = (int)m_Config[ moR(SOUNDFAUST_HEIGHT) ].GetData()->Fun()->Eval(m_EffectState.tempo.ang)* (float)(h/600.0);
    */
    /*
    glBegin(GL_QUADS);
    glTexCoord2f( PosTextX0, PosTextY1);
    glVertex2f( -0.5*w, -0.5*h);

    glTexCoord2f( PosTextX1, PosTextY1);
    glVertex2f(  0.5*w, -0.5*h);

    glTexCoord2f( PosTextX1, PosTextY0);
    glVertex2f(  0.5*w,  0.5*h);

    glTexCoord2f( PosTextX0, PosTextY0);
    glVertex2f( -0.5*w,  0.5*h);
    glEnd();
    */
    /*
    moFont* pFont = m_pResourceManager->GetFontMan()->GetFont(0);

    textstate+= moText(" position") + IntToStr(audioposition);
    textstate+= moText(" diff:") + (moText)IntToStr( abs(audioposition - last_ticks) );

    if (pFont) {
        pFont->Draw(    0.0,
                        0.0,
                        textstate );
    }
    */
    moShaderManager* pSMan;
    moGLManager* pGLMan;
    moRenderManager* pRMan;
    moTextureManager* pTMan;

    if (m_pResourceManager) {
      pSMan = m_pResourceManager->GetShaderMan();
      if (!pSMan) return;

      pGLMan = m_pResourceManager->GetGLMan();
      if (!pGLMan) return;

      pRMan = m_pResourceManager->GetRenderMan();
      if (!pRMan) return;

      pTMan = m_pResourceManager->GetTextureMan();
      if (!pTMan) return;

    } else return ;

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    moObject3D Scene;

    int TGLId = m_Config.GetGLId( moR(SOUNDFAUST_TEXTURE), this, &m_EffectState.tempo );
    moData* TD = m_Config[moR(SOUNDFAUST_TEXTURE)].GetData();
    if (TD && TD->Texture()==NULL) UpdateConnectors();
    glBindTexture( GL_TEXTURE_2D, TGLId );


    ///MATERIAL
    moMaterial Mat;
      //Mat.m_Map = pTMan->GetTexture(pTMan->GetTextureMOId( "default", false ));
      Mat.m_Map = TD->Texture();
      Mat.m_MapGLId = Mat.m_Map->GetGLId();
      //Mat.m_MapGLId = 1;
      //Mat.m_Color = moColor( 1.0, 1.0, 1.0 );
      //Mat.m_fTextWSegments = 13.0f;
      //Mat.m_fTextHSegments = 13.0f;
      //Mat.m_vLight = moVector3f( -1.0, -1.0, -1.0 );
      //Mat.m_vLight.Normalize();
      //Mat.m_PolygonMode = MO_POLYGONMODE_LINE;
      //Mat.m_PolygonMode = MO_POLYGONMODE_FILL;
      //Mat.m_fWireframeWidth = 0.0005f;

    ///GEOMETRY
    moSphereGeometry Sphere( 0.1+0.01*m_fSampleVolume, 8, 8 );

    ///MESH MODEL (aka SCENE NODE)
    float progress = 0.0;
    moGLMatrixf Model;
    Model.MakeIdentity()
         .Rotate(   360.0*progress*moMathf::DEG_TO_RAD, 0.0, 1.0, 0.0 )
         .Translate(    m_vPosition.X(), m_vPosition.Y(), m_vPosition.Z() );
    moMesh Mesh( Sphere, Mat );
    Mesh.SetModelMatrix(Model);


    //moBoxGeometry LevelBox( 0.2,0.2,0.2);
    progress = float(m_EffectState.tempo.ticks)*0.000;
    for( int a=0; a<15; a++) {
      //set material
      m_LevelMats[a].m_Map =TD->Texture();
      m_LevelMats[a].m_MapGLId = m_LevelMats[a].m_Map->GetGLId();
      moVector4d color = m_Config.EvalColor( moR(SOUNDFAUST_COLOR) );
      m_LevelMats[a].m_Color = moColor( color.X(), color.Y(), color.Z() );
      m_LevelMats[a].m_fOpacity = m_Config.Eval( moR(SOUNDFAUST_ALPHA));
      //set level meshes
      m_LevelMeshes[a].m_Geometry = m_LevelBoxes[a];
      m_LevelMeshes[a].m_Material = m_LevelMats[a];
      //set models
      float value = m_pDspFaust->getParamValue(a);
      float min = m_pDspFaust->getParamMin( a );
      float max = m_pDspFaust->getParamMax( a );
      float norVal = (value - min) / ( max - min);
      m_LevelModels[a].MakeIdentity()
                .Scale(   m_vScale.X(),  m_vScale.Y()*norVal,  m_vScale.Z() )
                .Rotate(   360.0*progress*moMathf::DEG_TO_RAD, 0.0, 1.0, 0.0 )
                .Translate(    m_vPosition.X()+(a-7)*0.2, m_vPosition.Y(), m_vPosition.Z() );
      m_LevelMeshes[a].SetModelMatrix(m_LevelModels[a]);
      //add to scene
      Scene.AddChild(&m_LevelMeshes[a]);
    }


    //Scene.AddChild(&Mesh);

    ///CAMERA PERSPECTIVE
    moCamera3D Camera3D;
    pGLMan->SetDefaultPerspectiveView( pRMan->ScreenWidth(), pRMan->ScreenHeight() );
    //  Camera3D.MakePerspective(60.0f, p_display_info.Proportion(), 0.01f, 1000.0f );
    Camera3D = pGLMan->GetProjectionMatrix();

    ///RENDERING
    pRMan->Render( &Scene, &Camera3D );
}



void moEffectSoundFaust::Interaction( moIODeviceManager *IODeviceManager ) {

	moDeviceCode *temp;
	MOint did,cid,state,valor;

	moEffect::Interaction( IODeviceManager );

	if (devicecode!=NULL)
	for(int i=0; i<ncodes; i++) {

		temp = devicecode[i].First;

		while(temp!=NULL) {
			did = temp->device;
			cid = temp->devicecode;
			state = IODeviceManager->IODevices().GetRef(did)->GetStatus(cid);
			valor = IODeviceManager->IODevices().GetRef(did)->GetValue(cid);
			if (state)
			switch(i) {
				case MO_SOUNDFAUST_TRANSLATE_X:
					Tx = valor;
					MODebug2->Push(IntToStr(valor));
					break;
				case MO_SOUNDFAUST_TRANSLATE_Y:
					Ty = m_pResourceManager->GetRenderMan()->RenderHeight() - valor;
					MODebug2->Push(IntToStr(valor));
					break;
				case MO_SOUNDFAUST_SCALE_X:
					Sx+=((float) valor / (float) 256.0);
					MODebug2->Push(IntToStr(valor));
					break;
				case MO_SOUNDFAUST_SCALE_Y:
					Sy+=((float) valor / (float) 256.0);
					MODebug2->Push(IntToStr(valor));
					break;
			}
		temp = temp->next;
		}
	}

}

moConfigDefinition *
moEffectSoundFaust::GetDefinition( moConfigDefinition *p_configdefinition ) {

	//default: alpha, color, syncro
	p_configdefinition = moEffect::GetDefinition( p_configdefinition );
  p_configdefinition->Add( moText("sound"), MO_PARAM_SOUND, SOUNDFAUST_SOUND, moValue( "default", "TXT") );

  p_configdefinition->Add( moText("seekposition"), MO_PARAM_FUNCTION, SOUNDFAUST_SEEKPOSITION, moValue( "0", MO_VALUE_FUNCTION ).Ref() );
  p_configdefinition->Add( moText("mode"), MO_PARAM_NUMERIC, SOUNDFAUST_MODE, moValue( "0", MO_VALUE_NUM_INT ) );
	p_configdefinition->Add( moText("loop"), MO_PARAM_FUNCTION, SOUNDFAUST_LOOP, moValue( "0.0", MO_VALUE_FUNCTION).Ref() );
	p_configdefinition->Add( moText("launch"), MO_PARAM_FUNCTION, SOUNDFAUST_LAUNCH, moValue( "0.0", MO_VALUE_FUNCTION).Ref() );
	p_configdefinition->Add( moText("speedofsound"), MO_PARAM_FUNCTION, SOUNDFAUST_SPEEDOFSOUND, moValue( "0.0", MO_VALUE_FUNCTION).Ref() );
	p_configdefinition->Add( moText("pitch"), MO_PARAM_FUNCTION, SOUNDFAUST_PITCH, moValue( "1.0", MO_VALUE_FUNCTION).Ref() );

	//p_configdefinition->Add( moText("sound"), MO_PARAM_TEXT, SOUND_SOUND, moValue( "default", "TXT") );
	p_configdefinition->Add( moText("volume"), MO_PARAM_FUNCTION, SOUNDFAUST_VOLUME, moValue( "1.0", MO_VALUE_FUNCTION).Ref() );
	p_configdefinition->Add( moText("balance"), MO_PARAM_TRANSLATEX, SOUNDFAUST_BALANCE, moValue( "0.0", MO_VALUE_FUNCTION).Ref() );
	p_configdefinition->Add( moText("texture"), MO_PARAM_TEXTURE, SOUNDFAUST_TEXTURE, moValue( "default", "TXT") );
	p_configdefinition->Add( moText("blending"), MO_PARAM_BLENDING, SOUNDFAUST_BLENDING, moValue( "0", "NUM").Ref() );


  p_configdefinition->Add( moText("scalex"), MO_PARAM_SCALEX, SOUNDFAUST_SCALEX, moValue( "1.0", "FUNCTION").Ref() );
	p_configdefinition->Add( moText("scaley"), MO_PARAM_SCALEY, SOUNDFAUST_SCALEY, moValue( "1.0", "FUNCTION").Ref() );
	p_configdefinition->Add( moText("scalez"), MO_PARAM_SCALEZ, SOUNDFAUST_SCALEZ, moValue( "1.0", "FUNCTION").Ref() );
	p_configdefinition->Add( moText("translatex"), MO_PARAM_TRANSLATEX, SOUNDFAUST_TRANSLATEX, moValue( "0.5", "FUNCTION").Ref() );
	p_configdefinition->Add( moText("translatey"), MO_PARAM_TRANSLATEY, SOUNDFAUST_TRANSLATEY, moValue( "0.5", "FUNCTION").Ref() );
	p_configdefinition->Add( moText("translatez"), MO_PARAM_TRANSLATEZ, SOUNDFAUST_TRANSLATEZ );
	p_configdefinition->Add( moText("directionx"), MO_PARAM_FUNCTION, SOUNDFAUST_DIRECTIONX );
	p_configdefinition->Add( moText("directiony"), MO_PARAM_FUNCTION, SOUNDFAUST_DIRECTIONY );
	p_configdefinition->Add( moText("directionz"), MO_PARAM_FUNCTION, SOUNDFAUST_DIRECTIONZ );
	p_configdefinition->Add( moText("speedx"), MO_PARAM_FUNCTION, SOUNDFAUST_SPEEDX );
	p_configdefinition->Add( moText("speedy"), MO_PARAM_FUNCTION, SOUNDFAUST_SPEEDY );
	p_configdefinition->Add( moText("speedz"), MO_PARAM_FUNCTION, SOUNDFAUST_SPEEDZ );
	return p_configdefinition;
}



void
moEffectSoundFaust::Update( moEventList *Events ) {

    UpdateParameters();

    //get the pointer from the Moldeo Object sending it...
    moMoldeoObject::Update(Events);

}
