# Android eXiSoundVis Plugin

This plugin is an implementation of the [eXiSoundVis plugin for UE4](https://github.com/eXifreXi/eXiSoundVis) which has been altered to allow for use on Android devices. It uses an implementation of Kiss-FFT (see [here](https://github.com/itdaniher/kissfft) for an implementation on Github), and allows for music visualization on the Android platform in real time.

This repository contains a modified version of the plugin, as well as a

### Notes:
- There is a known incompatibility in the case of the Google Project Tango Yellowstone Tablet being used with Unreal Engine 4, which means it is not possible to play .obb files with a 44khz sample rate correctly. This is true of both files loaded at runtime and those imported as assets at build/compile time in UE4. Please be sure to replace any 44khz files with .obbs with a 22khz sample rate, otherwise the .obb files will be played back at an incorrect, slower speed.

- The plugin currently only supports loading from absolute paths on Android at present.

### License

This software is licensed under [Apache 2.0.](http://www.apache.org/licenses/LICENSE-2.0)
The eXiSoundVis plugin is licensed under the MIT License and is copyright Cederic Neukirchen.
The Kiss-FFT software package, copyright Mark Borgerding, is provided under a modified version of the BSD license (see [here]( https://github.com/itdaniher/kissfft/blob/master/COPYING)) for full license.
