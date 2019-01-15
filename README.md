# Deprecation notice 

Thanks for your interest in the Android eXiSoundVis plugin. This repository is no longer maintained, and has been archived to allow for future reference and forking by the developer community.

# Android eXiSoundVis Plugin

This plugin is an implementation of the [eXiSoundVis plugin for UE4](https://github.com/eXifreXi/eXiSoundVis) which has been altered to allow for use on Android devices. It uses an implementation of Kiss-FFT (see [here](https://github.com/itdaniher/kissfft) for an implementation on Github), and allows for music visualization on the Android platform in real time.

This repository contains a modified version of the plugin, as well as the example project which demonstrates how to use the plugin functions. You will also need to source .obb music files and place them in a known location on your Android device- the Android File Transfer application can be used to achieve this. 

### Notes:
- This project has been tested on and is compatible with the Google Tango Yellowstone tablet.
- There is a known issue in the case of the Google Project Tango Yellowstone Tablet being used with Unreal Engine 4, which means it is not possible to play .obb files with a 44khz sample rate correctly. This is true of both files loaded at runtime and those imported as assets at build/compile time in UE4. Please be sure to replace any 44khz files with .obbs with a 22khz sample rate, otherwise the .obb files will be played back at an incorrect, slower speed.

- The plugin currently only supports loading from absolute paths on Android at present.

### License

This software is licensed under [Apache 2.0.](http://www.apache.org/licenses/LICENSE-2.0)
The eXiSoundVis plugin is licensed under the MIT License and is copyright Cederic Neukirchen 2015.
The Kiss-FFT software package, copyright Mark Borgerding 2003-2010, is provided under a modified version of the BSD license (see [here]( https://github.com/itdaniher/kissfft/blob/master/COPYING)) for full license. Please note that this software is not endorsed by the authors of the included open-source packages or plugins.
