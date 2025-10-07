#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>

char Buffer[128] = "The buffer has not been overwritten.";

void SendCommand(char* String)
{
    mciSendString(String, Buffer, 128, NULL);
}


//util
int CDTimeStampToFrames(char* TimeStamp)
{
    int PreMinutes;
    int PreSeconds;
    int PreFrames;
    int Frames;
    if (sscanf(TimeStamp, "%d:%d:%d", &PreMinutes, &PreSeconds, &PreFrames) != 3)
    {
        return(-1);
    }
    Frames = PreFrames;
    PreSeconds += PreMinutes * 60;
    Frames += PreSeconds * 75;
    return(Frames);
}

void CDFramesToTimeStamp(int Frames, char* TimeStamp)
{
    int PostSeconds = Frames / 75;
    int PostFrames = Frames % 75;
    int PostMinutes = PostSeconds / 60;
    PostSeconds = PostSeconds % 60;
    sprintf(TimeStamp,"%02d:%02d:%02d",PostMinutes,PostSeconds,PostFrames);
}


//info
char* CDGetReturnPointer()
{
    return(Buffer);
}

void CDGetStatus()
{
    SendCommand("status cd mode");
}

int CDDiscCheck()
{
    SendCommand("status cd media present");
    if (strcmp(Buffer, "true") == 0)
    {
        return(1);
    }
    return(0);
}

int CDGetNumberOfTracks()
{
    SendCommand("status cd number of tracks");
    return(atoi(Buffer));
}

void CDGetTrackLengths(int* TrackLengths)
{
    int NumberOfTracks = CDGetNumberOfTracks();
    char Command[32];
    for(int i = 1; i <= NumberOfTracks; i++)
    {
        sprintf(Command,"status cd track %d length",i);
        SendCommand(Command);
        TrackLengths[i-1] = CDTimeStampToFrames(Buffer);
    }
}

void CDGetTrackPositions(int* TrackPositions)
{
    int NumberOfTracks = CDGetNumberOfTracks();
    char Command[32];
    for(int i = 1; i <= NumberOfTracks; i++)
    {
        sprintf(Command,"status cd track %d position",i);
        SendCommand(Command);
        TrackPositions[i-1] = CDTimeStampToFrames(Buffer);
    }
}

int CDGetRunTime()
{
    int NumberOfTracks = CDGetNumberOfTracks();
    int TrackLengths[NumberOfTracks];
    int Total = 0;
    CDGetTrackLengths(TrackLengths);
    for(int i = 0; i < NumberOfTracks; i++)
    {
        Total += TrackLengths[i];
    }
    return(Total);
}

int CDGetCurrentTime()
{
    SendCommand("status cd position");
    return(CDTimeStampToFrames(Buffer));
}

int CDGetCurrentTrack()
{
    SendCommand("status cd current track");
    return(atoi(Buffer));
}

int CDGetCurrentTrackTime()
{
    int CurrentTrack = CDGetCurrentTrack();
    int NumberOfTracks = CDGetNumberOfTracks();
    int Tracks[NumberOfTracks];
    CDGetTrackPositions(Tracks);
    return(CDGetCurrentTime() - Tracks[CurrentTrack-1]);
}

int CDGetTimeRemaining()
{
    return(CDGetRunTime() - CDGetCurrentTime());
}

int CDGetTrackTimeRemaining()
{
    int CurrentTrack = CDGetCurrentTrack();
    int NumberOfTracks = CDGetNumberOfTracks();
    int Tracks[NumberOfTracks];
    CDGetTrackPositions(Tracks);
    return(Tracks[CurrentTrack] - CDGetCurrentTime());
}


//MCI controls
void CDInit()
{
    SendCommand("open cdaudio alias cd shareable");
}

void CDExit() //Usually unecessary as Windows will automatically close the MCI alias once the program exits.
{
    SendCommand("close cd");
}


//controls
void CDPlay(int From, int To)
{
    int NumberOfTracks = CDGetNumberOfTracks();
    int TrackPositions[CDGetNumberOfTracks()];
    char Command[32];
    CDGetTrackPositions(TrackPositions);
    if(From == -1)
    {
        From = TrackPositions[0];
    }
    if(To == -1)
    {
        To = TrackPositions[NumberOfTracks - 1];
    }
    char FromTimeStamp[9];
    char ToTimeStamp[9];
    CDFramesToTimeStamp(From,FromTimeStamp);
    CDFramesToTimeStamp(To,ToTimeStamp);
    sprintf(Command,"play cd from %s to %s",FromTimeStamp,ToTimeStamp);
    SendCommand(Command);
}

int CDPause()
{
    int CurrentTime = CDGetCurrentTime();
    SendCommand("pause cd");
    return(CurrentTime);
}

void CDResume()
{
    SendCommand("resume cd");
}

int CDStop()
{
    int CurrentTime = CDGetCurrentTime();
    SendCommand("stop cd");
    return(CurrentTime);
}

void CDSetVolume(int Volume) //0-1000, 
{
    char Command[32];
    sprintf(Command,"set cd volume to %d",Volume);
    SendCommand(Command);
}
/* This function is unusuable on many systems due to how Windows audio is structured.
Not recommended unless you have a niche system it's confirmed to work on.
CD audio over MCI is separate from process audio, creative solutions are needed. */


//physical
void CDOpenTray()
{
    SendCommand("set cd door open");
    CDExit();
}

void CDCloseTray()
{
    SendCommand("set cd door closed");
}

