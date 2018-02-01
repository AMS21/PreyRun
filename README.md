# PreyRun 1.6.0
PreyRun is a speedrunning focused mod the video game Prey (2006) created by Human Head Studio.
The mod can interact with [LiveSplit](https://livesplit.github.io/) using the [PreySplit](https://github.com/AMS21/LiveSplit.PreySplit) addon.

## Video of the mod in action
[![Showcase video](http://img.youtube.com/vi/UsfWV4P1Dqc/0.jpg)](http://www.youtube.com/watch?v=UsfWV4P1Dqc)

# Features
* Automatic timing of runs without loading times
* Timer recovery when the game should crash
* In-game visualization of past runs called [Ghosting](https://youtu.be/Um7RdCR-3kI)
* Extra hud elements like speedometer or the timer
* Quality of life features like loading the most recent savegame or muting all jukeboxes
* Utilities to make routing and testing easier

For a list of all available commands and console variables have a look at the [wiki page](https://github.com/AMS21/PreyRun/wiki/Console-commands-and-variables)

# Installation
- Download [PreyRun](https://github.com/AMS21/PreyRun/releases/latest)
- Extract the file from the archive somewhere
- In your Prey root folder (where Prey.exe and PreyDed.exe are located) create a new folder called 'PreyRun'
- Copy the files you have extracted previously to the newly created sub folder 'PreyRun'
- Your folders shoud look something like [this](https://imgur.com/a/4SxWX)

### Creating a shortcut
- Create a shortcut to "Prey.exe"
- Right click on the shortcut -> Properties
- at "Target" add "+set fs_game PreyRun" at the end
- The "Target" field should look something like this : "D:\Prey\PREY.exe +set fs_game PreyRun"
