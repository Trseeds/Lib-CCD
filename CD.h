#ifndef CD_H
#define CD_H

#include <stdio.h>
#include <stdlib.h>

//Util
int CDTimeStampToFrames(char* TimeStamp);
void CDFramesToTimeStamp(int Frames, char* TimeStamp);
//Info
char* CDGetReturnPointer(); //Returns pointer to the buffer where CD commands write errors to.
void CDGetStatus();
int CDDiscCheck(); //1 if disc is present, 0 otherwise.
int CDGetNumberOfTracks();
void CDGetTrackPositions(int* TrackPositions); //Returns positions as frame values rather than timestamps.
void CDGetTrackLengths(int* TrackLengths); //Returns lengths in frames.
int CDGetRunTime(); //Returns lengths of all tracks added together in frames.
int CDGetCurrentTime(); //Returns the current timestamp on the disc as a frame.
int CDGetCurrentTrack(); //Returns the track that is currently playing.
int CDGetCurrentTrackTime(); //Returns a timestamp in the currently playing track as a frame.
int CDGetTimeRemaining();
int CDGetTrackTimeRemaining();
//MCI Controls
void CDInit();
void CDExit();
//Controls
void CDPlay(int From, int To); //From and to are frames, pass -1 for either value to get the beginning or the end. 
int CDPause(); //Returns time paused at.
void CDResume();
int CDStop(); //Returns time stopped at.
void CDSetVolume(int Volume);
//Physical
void CDOpenTray(); //Opens CD tray if supported. (almost always supported)
void CDCloseTray(); //Closes CD tray if supported. (supported sometimes)

#endif