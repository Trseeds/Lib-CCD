LibCCD: a CD playback library for C.

This is mainly a wrapper for the Windows MCI CD functionality, first introduced on 9X systems, it remains usable on Windows 11, but is now deprecated. (as of 10/05/25)
As such, writing implementations for other systems may require great effort, as the high level functions I wrote the header in mind with may not be present.
MCI functions should work with little fuss in Linux under WINE.

Q&A:
    Q: Will I ever write for other operating systems?
    A: Probably not. This is a quick and dirty solution for my own project.
    
    Q: Why does everything return an integer timestamp as a count of frames instead of a readable MSF timestamp?
    A: Integers are easier to work with and faster in C. A Frames to MSF conversion function is included for convenience.

    Q: Will this receive long term support and or updates?
    A: No.

    Q: Who is asking these questions?
    A: Me.

Known quirks:
    Windows:
        Volume control does not work.
        Do not always need to use CDExit() as Windows will usually close the connection once the program that opened it is closed.
        Pausing and resuming rewinds a short amount of time then jumps back.

To compile:
    Windows:
        Link lwinmm

Documentation:
CDInit():
    Queries MCI to open cdaudio using alias cd.
    Returns to the MCI buffer.
CDExit():
    Queries MCI to close the cd alias.
CDTimeStampToFrames():
    One string as input. String is expected to be a CD Time formatted timestamp. (MM:SS:FF, 75 frames in a second)
    Returns one integer. Integer is the sum of all frames in the given timestamp. 01:00:00 becomes 4500. 
CDFramesToTimeStamp():
    One integer, and one pointer to a string as input. Integer is expected to be a timestamp in frames. Return value is written to string.
    Writes the given timestamp as a CD Time formatted timestamp to the given string. String must be at least 9 bytes in size.
CDGetReturnPointer():
    This returns a pointer to the MCI return buffer, needed for certain other functions.
CDGetStatus():
    Queries MCI for the current mode of the CD.
    MCI writes return value to buffer.
    Important return values:
        not ready - Drive is empty or unreadable.
        open - Drive is open
        playing - Playback is playing.
        paused - Playback is paused.
        stopped - Playback is stopped.
CDDiscCheck():
    Queries MCI if the drive has a readable disc inserted.
    Returns 1 if a disc is inserted, 0 if not.
CDGetNumberOfTracks():
    Queries MCI for the number of tracks on disc.
    Returns the number of tracks.
CDGetTrackLengths():
    Queries MCI for the lengths of each track on disc.
    Pointer to an integer array as input.
    Writes the length of each track on disc in frames to the given array.
CDGetTrackPosition():
    Queries MCI for the position of each track on disc.
    One integer array as input.
    Writes the position of each track on disc in frames to the given array.
CDGetRunTime():
    Adds the length in frames of each track on disc together.
    Returns the total runtime of all tracks as an integer frame value.
CDGetCurrentTime():
    Queries MCI for the current position of playback on disc.
    Returns that timestamp as a frame.
CDGetCurrentTrack():
    Queries MCI for the currently playing track on disc.
    Returns that track.
CDGetCurrentTrackTime():
    Subtracts the current time on disc from the beginning of the next track.
    Returns the time in the current track as a frame.
CDGetTimeRemaining():
    Subtracts the runtime from the current time on disc.
    Returns the amount of time remaining on disc as a frame.
CDGetTrackTimeRemaining():
    Subtracts the start of the next track from the current time on disc.
    Returns the amount of time remaining in the current track as a frame.
CDPlay():
    Queries MCI to play from the given values.
    Two integers as input. Both inputs are expected to be frame timestamps or -1.
    Value 1, 'From', is the frame that playback will begin from. If -1 is passed, playback will begin from track 1.
    Value 2, 'To', is the frame at which playback will cease. if -1 is passed, playback will stop at the end of the last track.
CDPause():
    Queries MCI to pause playback.
    Returns the time playback was paused at as a frame.
    Not recommended, stop and play instead.
CDResume():
    Queries MCI to resume playback, only works if previously paused and alias remains open.
CDStop():
    Queries MCI to stop playback.
    Returns the time playback was stopped at as a frame.
CDPrevious():
    One integer as input. Integer is the same 'To' that is passed into CDPlay()
    If playback is less than 5 seconds into the track, seeks to the beginning of the previous track.
    If playback is more than 5 seconds into the track, seeks to the beginning of the track.
    If playback is on the first track, seeks to the beginning of the first track.
CDNext():
    One integer as input. Integer is the same 'To' that is passed into CDPlay()
    Seeks to the beginning of the next track.
    If playback is on the last track, stops playback.
CDSetVolume():
    Queries MCI to change the volume of the alias.
    One integer as input. Value expected to be from 0-1000. (0.0%-100.0%)
    This function does not work correctly on most systems due to how Windows audio is structured.
CDOpenTray():
    Queries MCI to open the tray of the alias's drive.
CDCloseTray():
    Queries MCI to close the tray of the alias's drive.