
#ifndef _LOOPIDITY_H_
#define _LOOPIDITY_H_


//#define MEMORY_CHECK              1 // if 0 choose DEFAULT_BUFFER_SIZE wisely
//#define INIT_LOOPIDITY            1
#define INIT_JACK                 1
//#define LOOP_COUNTER              1
//#define DSP                       1
#define USER_DEFINED_BUFFER       1 // TODO: user defined buffer sizes
#define DRAW_STATUS               1
#define DRAW_SCENES               1
#define DRAW_SCOPES               1
#define DRAW_DEBUG_TEXT           1
#define SCAN_LOOP_PEAKS_DATA      1
#define SCAN_TRANSIENT_PEAKS_DATA 1
#define DEBUG_TRACE               1

#if DRAW_STATUS
#define DRAW_MODE 0
/*
#define UPDATE_MEMORY 0
#define UPDATE_LOOP_PROGRESS 0
#define UPDATE_VU 0
#define VARDUMP 0
*/
#endif // #if DRAW_STATUS

#if DRAW_SCENES
#define DRAW_SCENE_SCOPE             1
#define DRAW_HISTOGRAMS              1
#define DRAW_MUTED_HISTOGRAMS        1
#define DRAW_PEAK_RINGS              1
#define DRAW_LOOPS                   1
#define DRAW_MUTED_LOOPS             0
#define DRAW_RECORDING_LOOP          DRAW_LOOPS && 1
#define DRAW_CURRENT_SCENE_INDICATOR 1
#endif // #if DRAW_SCENES

#if DRAW_DEBUG_TEXT
#define DRAW_DEBUG_TEXT_L Trace::SetDbgTextL() ;
#define DRAW_DEBUG_TEXT_R Trace::SetDbgTextR() ;
#else
#define DRAW_DEBUG_TEXT_L ;
#define DRAW_DEBUG_TEXT_R ;
#endif // #if DRAW_DEBUG_TEXT

// transition to Trace class
#if DEBUG_TRACE
#define DEBUG_TRACE_EVS   1
#define DEBUG_TRACE_IN    1
#define DEBUG_TRACE_OUT   1
#define DEBUG_TRACE_CLASS 0
#define TRACE_EVS         Trace::TraceEvs
#define TRACE_IN          Trace::TraceIn
#define TRACE_OUT         Trace::TraceOut
#define TRACE_SCENE       Trace::TraceScene
#endif // #if DEBUG_TRACE


// quantities
#define DEFAULT_BUFFER_SIZE 33554432 // 2^25 (approx 3 min @ 48k)
//#define DEFAULT_BUFFER_SIZE 25165824 // 1024 * 1024 * 24 (approx 135 sec @ 48k)
//#define DEFAULT_BUFFER_SIZE 16777216 // 2^24 (approx 90 sec @ 48k)
//#define DEFAULT_BUFFER_SIZE 8388608 // 2^23 (approx 45 sec @ 48k)
//#define DEFAULT_BUFFER_SIZE 2097152 // 2^21 (approx 10 sec @ 48k)
//#define DEFAULT_BUFFER_SIZE 1048576 // 2^20 (approx 5 sec @ 48k)

// TODO: implement setting N_CHANNELS via cmd line arg - GetTransientPeaks and updateVUMeters are especially brittle now
#define N_INPUT_CHANNELS  2 // TODO: nyi - only used for memory check and scope cache
#define N_OUTPUT_CHANNELS 2 // TODO: nyi - only used for N_PORTS
#define N_PORTS           N_INPUT_CHANNELS + N_OUTPUT_CHANNELS // TODO: nyi - only used for scope cache
#define N_SCENES          3
#define N_LOOPS           9 // N_LOOPS_PER_SCENE
#define LOOP_VOL_INC      0.1

#define SDL_PEAK_RADIUS   50
#define SDL_LOOP_DIAMETER ((SDL_PEAK_RADIUS * 2) + 1)
#define N_PEAKS_COURSE    SDL_LOOP_DIAMETER
#define N_PEAKS_FINE      360 // should be divisible into 360

// string constants
#define APP_NAME                "Loopidity"
//#define CONNECT_ARG             "--connect"
#define MONITOR_ARG             "--nomon"
#define SCENE_CHANGE_ARG        "--noautoscenechange"
#define FREEMEM_FAIL_MSG        "ERROR: Could not determine available memory - quitting"
#define JACK_FAIL_MSG           "ERROR: Could not register JACK client - quitting"
#define ZERO_BUFFER_SIZE_MSG    "ERROR: initBufferSize is zero - quitting"
#define INSUFFICIENT_MEMORY_MSG "ERROR: Insufficient memory - quitting"
#define OUT_OF_MEMORY_MSG       "ERROR: Out of Memory"
#define GETPEAK_ERROR_MSG       "Loopidity::GetPeak(): subscript out of range\n"

// sdl user events
#define EVT_NEW_LOOP      1
#define EVT_SCENE_CHANGED 2

// error states
#define JACK_INIT_SUCCESS 0
#define JACK_FAIL         1
#define JACK_BUFF_FAIL    2
#define JACK_MEM_FAIL     3

// aliases
#define SAMPLE jack_default_audio_sample_t


// dependencies

#include <cstdlib>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>
#include <SDL_ttf.h>
#include <X11/Xlib.h>

#include "jack_io.h"
#include "loopidity_sdl.h"
#include "scene.h"
#include "scene_sdl.h"
#include "trace.h"

using namespace std ;


class Loopidity
{
  friend class JackIO ;
  friend class Trace ;

  public:

    // init
    static SceneSdl** Init(           bool isMonitorInputs , bool isAutoSceneChange ,
                                      unsigned int recordBufferSize) ;

    // user actions
    static void ToggleAutoSceneChange(void) ;
    static void ToggleScene(          void) ;
    static void ToggleState(          void) ;
    static void DeleteLoop(           unsigned int sceneN , unsigned int loopN) ;
    static void DeleteLastLoop(       void) ;
    static void IncLoopVol(           unsigned int sceneN , unsigned int loopN , bool IsInc) ;
    static void ToggleLoopIsMuted(    unsigned int sceneN , unsigned int loopN) ;
    static void ToggleSceneIsMuted(   void) ;
    static void SetStatus(            unsigned int sceneN) ;
    static void ResetScene(           unsigned int sceneN) ;
    static void ResetCurrentScene(    void) ;
    static void Reset(                void) ;
    static void Cleanup(              void) ;

    // getters/setters
    static unsigned int    GetCurrentSceneN(      void) ;
    static unsigned int    GetNextSceneN(         void) ;
    static unsigned int    GetLoopPos(            void) ;
//    static bool            GetIsRolling(          void) ;
    static bool            GetShouldSaveLoop(     void) ;
    static bool            GetDoesPulseExist(     void) ;
    static void            SetNFramesPerPeriod(   unsigned int nFrames) ;
    static vector<SAMPLE>* GetPeaksInCache(       void) ;
    static vector<SAMPLE>* GetPeaksOutCache(      void) ;
    static SAMPLE*         GetTransientPeaksCache(void) ;
    static SAMPLE*         GetTransientPeakIn(    void) ;
    static SAMPLE*         GetTransientPeakOut(   void) ;
    static SAMPLE          GetPeak(               SAMPLE* buffer , unsigned int nFrames) ;

    // event handlers
    static void OnLoopCreation(unsigned int sceneN , Loop* newLoop) ;
    static void OnSceneChange( unsigned int sceneN) ;

    // helpers
    static void ScanTransientPeaks(void) ;
    static void UpdateView(        unsigned int sceneN) ;
    static void OOM(               void) ;

  private:

    static void SetMetaData(unsigned int sampleRate , unsigned int frameSize ,
                            unsigned int nFramesPerPeriod) ;

    // recording state
    static unsigned int CurrentSceneN ;
    static unsigned int NextSceneN ;
//	    static bool         IsRolling ;
    static bool         ShouldSceneAutoChange ;

    // audio data
    static Scene*       Scenes[N_SCENES] ;
    static SceneSdl*    SdlScenes[N_SCENES] ;
    static unsigned int NFramesPerPeriod ;
    static unsigned int NFramesPerGuiInterval ;

    // transient sample data
    static SAMPLE*        Buffer1 ;
    static SAMPLE*        Buffer2 ;
    static vector<SAMPLE> PeaksIn ;
    static vector<SAMPLE> PeaksOut ;
    static SAMPLE         TransientPeaks[N_PORTS] ;
    static SAMPLE         TransientPeakInMix ;
    static SAMPLE         TransientPeakOutMix ;
} ;


#endif // #ifndef _LOOPIDITY_H_
