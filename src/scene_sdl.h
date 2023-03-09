/*\
|*|  Loopidity - multi-track multi-channel audio looper designed for live handsfree use
|*|  Copyright 2012,2013,2015,2017 bill-auger <https://github.com/bill-auger/loopidity/issues>
|*|
|*|  This file is part of the Loopidity program.
|*|
|*|  Loopidity is free software: you can redistribute it and/or modify
|*|  it under the terms of the GNU General Public License version 3
|*|  as published by the Free Software Foundation.
|*|
|*|  Loopidity is distributed in the hope that it will be useful,
|*|  but WITHOUT ANY WARRANTY; without even the implied warranty of
|*|  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*|  GNU General Public License for more details.
|*|
|*|  You should have received a copy of the GNU General Public License
|*|  along with Loopidity.  If not, see <http://www.gnu.org/licenses/>.
\*/


#ifndef _SCENE_SDL_H_
#define _SCENE_SDL_H_


typedef float Sample ; // jack_default_audio_sample_t

#include "constants/scene_constants.h"
#include "scene.h"


class LoopSdl
{
  friend class SceneSdl ;


  private:

    /* LoopSdl class side private functions */

    LoopSdl(SDL_Surface* playingImg , SDL_Surface* mutedImg , Sint16 x , Sint16 y) ;
    ~LoopSdl() ;


    /* LoopSdl instance side private varables */

    // drawing backbuffers
    SDL_Surface* playingSurface ;
    SDL_Surface* mutedSurface ;
    SDL_Surface* currentSurface ;

    // drawing coordinates
    Sint16   loopL ;
    Sint16   loopC ;
    SDL_Rect rect ;

    /* LoopSdl instance side private functions */

    // loop state
    void setStatus(Uint16 loopStatus) ;
} ;


class SceneSdl
{
  friend class Loopidity ;
  friend class LoopiditySdl ;
  friend class Trace ;


  private:

    /* SceneSdl class side private constants */

    // drawing coordinates
    // TODO: some of these Uint16 have caused conversion warnings (perhaps all should be Sint16)
    static Sint16       HistogramsT ;  // drawRecordingLoop()
    static Sint16       HistogramsB ;  // drawRecordingLoop()
    static const Sint16 HistFramesT ;  // drawRecordingLoop()
    static const Sint16 HistFramesB ;  // drawRecordingLoop()
    static const Uint16 HistSurfaceW ; // drawHistogram()
    static const Uint16 HistSurfaceH ; // drawHistogram()
    static const Sint16 HistFrameR ;   // drawHistogram()
    static const Sint16 HistFrameB ;   // drawHistogram()
    static const float  HistPeakH ;    // drawHistogram()
    static const Sint16 Histogram0 ;   // drawHistogram()
    static const Uint16 LoopD ;
    static const Sint16 LoopW ;
    static const Uint16 Loop0 ;
    static const Sint16 LoopsL ;
    static const Sint16 LoopsT ;
    static const Uint16 Loops0 ;
    static const Uint16 LoopsB ;
    static const Sint16 LoopFrameT ;
    static const Sint16 LoopFrameB ;
    static const Uint16 SceneH ;
    static const Uint16 SceneL ;
    static const Uint16 SceneR ;
    static const Uint16 SceneFrameL ;
    static const Uint16 SceneFrameR ;
    static const float  PieSliceDegrees ;
    static const Uint16 SECONDS_PER_HOUR ;
    static const Uint8  MINUTES_PER_HOUR ;
    static const Uint8  SECONDS_PER_MINUTE ;


    /* SceneSdl class side private functions */

    // setup
    SceneSdl(Scene* a_scene , std::vector<Sample>* peaks_in) ;

    // helpers
    static void PixelRgb2Greyscale(SDL_PixelFormat* fmt , Uint32* pixel) ;

    // getters/setters
    static Sint16 GetLoopL(Uint16 loopN) ;


    /* SceneSdl instance side private constants */

    // drawing coordinates
    const Sint16   sceneT ;
    const Uint16   sceneFrameT ;
    const Uint16   sceneFrameB ;
//    const SDL_Rect sceneRect ;
    SDL_Rect sceneRect ;


    /* SceneSdl instance side private varables */

    // model
    Scene*               scene ;
    Uint8                sceneN ;
    std::vector<Sample>* peaksIn ;

    // loop image caches
    std::list<LoopSdl*> histogramImgs ;
    std::list<LoopSdl*> loopImgs ;

    // drawScene() instance variables
    Uint32 loopFrameColor ;
    Uint32 sceneFrameColor ;

    // drawScene() , drawHistogram() , and drawRecordingLoop() 'temp' variables
    Uint16       currentPeakN ;
    Uint16       hiScenePeak ;
    Uint16       loopN ;
    Uint16       histPeakN ;
    Uint16       peakH ;
    Sint16       loopL ;
    Sint16       loopC ;
    Sint16       histFrameL ;
    Sint16       histFrameR ;
    Sint16       ringR ;
    Sint16       loopFrameL ;
    Sint16       loopFrameR ;
    SDL_Rect     scopeMaskRect ;
    SDL_Rect     scopeGradRect ;
    SDL_Rect     histogramRect ;
    SDL_Rect     rotRect ;
    SDL_Rect     histMaskRect ;
    SDL_Rect     histGradRect ;
    LoopSdl*     histogramImg ;
    LoopSdl*     loopImg ;
    Loop*        loop ;
    SDL_Surface* rotImg ;

    // drawing backbuffers
    SDL_Surface* activeSceneSurface ;
    SDL_Surface* inactiveSceneSurface ;


    /* SceneSdl instance side private functions */

    // setup
    void reset(  void) ;
    void cleanup(void) ;

    // getters/setters
    void     startRolling(void) ;
    void     updateState( Uint8 currentSceneN , bool isRolling) ;
    LoopSdl* getLoopView( std::list<LoopSdl*>* imgs , Uint32 loopN) ;

    // drawing
    void     drawScene(              SDL_Surface* screen , Uint32 currentPeakN ,
                                     Uint16 loopProgress) ;
    void     drawRecordingLoop(      SDL_Surface* aSurface , Uint16 loopProgress) ;
    void     drawSceneStateIndicator(SDL_Surface* aSurface , Uint8 nextSceneN) ;
    LoopSdl* drawHistogram(          Loop* aLoop) ;
    LoopSdl* drawLoop(               Loop* aLoop , Uint16 loopN) ;

    // images
    void addLoop(   Loop* newLoop , Uint16 nLoops) ;
    void deleteLoop(Uint8 loopN) ;

    // helpers
    SDL_Surface* createHwSurface(       Sint16 w , Sint16 h) ;
    SDL_Surface* createSwSurface(       Sint16 w , Sint16 h) ;
    std::string  makeDurationStatusText(void) ;
} ;


#endif // #ifndef _SCENE_SDL_H_
