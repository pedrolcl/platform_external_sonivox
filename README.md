# Fork of the AOSP 'platform_external_sonivox' project to use it outside of Android 

[![Linux CI](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-linux.yml/badge.svg)](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-linux.yml)

[![Windows CI](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-win.yml/badge.svg)](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-win.yml)

[![macOS CI](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-mac.yml/badge.svg)](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-mac.yml)

[![FreeBSD CI](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-freebsd.yml/badge.svg)](https://github.com/pedrolcl/sonivox/actions/workflows/cmake-freebsd.yml)

This project is a fork of the Android Open Source Project 'platform_external_sonivox', including a CMake based build system to be used not on Android, but on any other computer Operating System.
Google licensed this work originally named Sonivox EAS (Embedded Audio Synthesis) from the company Sonic Network Inc. under the terms of the Apache License 2.0.

This is a Wave Table synthesizer, not using external soundfont files by default but embedded samples. It also supports external DLS soundfont files for better rendering quality. It is also a real time GM synthesizer. It consumes very little resources, so it may be indicated in projects for small embedded devices.
There is neither MIDI input nor Audio output facilities included in the library. You need to provide your own input/output.

You may find several projects already using this library as a git submodule:

* [Drumstick::RT](https://github.com/pedrolcl/drumstick) multiplatform realtime MIDI library. It has a Sonivox output backend.
* [Linux-SonivoxEas](https://github.com/pedrolcl/Linux-SonivoxEas) with ALSA Sequencer MIDI input and Pulseaudio output.
* [multiplatform-sonivoxeas](https://github.com/pedrolcl/multiplatform-sonivoxeas) with Drumstick::RT MIDI input and Qt Multimedia audio output.

## Build options

The build system has the following options:

* `USE_44KHZ`: Renders 44100 Hz audio (ON by default). If set to OFF will output 22050 Hz audio.
* `USE_16BITS_SAMPLES`: Uses 16 bits samples (instead of 8 bit). ON by default. The rendered audio uses always 16 bits.
* `BUILD_SONIVOX_STATIC` and `BUILD_SONIVOX_SHARED`: to control the generation and install of both the static and shared libraries from the sources. Both options are ON by default (at least one must be selected).
* `BUILD_TESTING`: ON by default, to control if the unit tests are built, which require Google Test.
* `BUILD_EXAMPLE`: ON by default, to build and install the example program.
* `CMAKE_POSITION_INDEPENDENT_CODE`: Whether to create position-independent targets. ON By default.
* `NEW_HOST_WRAPPER`: Uses the new CRT-based host wrapper for faster DLS loading. ON by default.
* `MAX_VOICES`: Maximum number of voices. 64 by default.

See also the [CMake documentation](https://cmake.org/cmake/help/latest/index.html) for common build options.

## Differences with upstream

This fork currently reverts these commits:

* Full revert of [af41595](https://github.com/pedrolcl/platform_external_sonivox/commit/af41595537b044618234fe7dd9ebfcc652de1576) (Remove unused code from midi engine)
* Full revert of [34ba480](https://github.com/pedrolcl/platform_external_sonivox/commit/34ba4804f643549b8ac74e5f56bfe64db3234447) (Remove unused code)
* Partial revert of [2fa59c8](https://github.com/pedrolcl/platform_external_sonivox/commit/2fa59c8c6851b453271f33f254c7549fa79d07fb) (Build separate sonivox libs with and without jet...)

All the sources from the Android repository are kept in place, but some are not built and included in the compiled products. A few headers, mostly empty, are included in the 'fakes' subdirectory to allow compilation outside Android.

## Using the library

You may find and use the installed libraries with `pkg-config` or the `find_package()` CMake command. The library API is documented in the 'docs' directory contents.

The 'example' directory contains a simple command line utility to render standard MIDI files into raw PCM audio streams. This utility may be compiled when building the library and also installed. You may find a standalone project containing this program here: https://github.com/pedrolcl/sonivox-example

You can use the program to listen MIDI files or to create audio files (like MP3 or WAV). These are the available command line options:

~~~
$ ./sonivoxrender -h
Usage: ./sonivoxrender [-h|--help] [-v|--version] [-d|--dls file.dls] [-r|--reverb 0..4] [-w|--wet 0..32767] [-n|--dry 0..32767] [-c|--chorus 0..4] [-l|--level 0..32767] [-g|--gain 0..100] file.mid ...
Render standard MIDI files into raw PCM audio.
Options:
    -h, --help          this help message.
    -v, --version       sonivox version.
    -d, --dls file.dls  DLS soundfont.
    -r, --reverb n      reverb preset: 0=no, 1=large hall, 2=hall, 3=chamber, 4=room.
    -w, --wet n         reverb wet: 0..32767.
    -n, --dry n         reverb dry: 0..32767.
    -c, --chorus n      chorus preset: 0=no, 1..4=presets.
    -l, --level n       chorus level: 0..32767.
    -g, --gain n        master gain: 0..100.
~~~

The following examples assume the default option USE_44KHZ=ON:

Example 1: Render a MIDI file and save the rendered audio as a raw audio file:

    $ sonivoxrender ants.mid > ants.pcm

Example 2: pipe the rendered audio thru the Linux ALSA 'aplay' utility:

    $ sonivoxrender ants.mid | aplay -c 2 -f S16_LE -r 44100

equivalent to:

    $ sonivoxrender ants.mid | aplay -f cd

Example 3: pipe the rendered audio thru the ['lame'](https://lame.sourceforge.io) utility creating a MP3 file:

    $ sonivoxrender ants.mid | lame -r -s 44100 - ants.mp3
    
Example 4: pipe the rendered audio thru the ['sox'](https://sourceforge.net/projects/sox/) utility creating a WAV file:

    $ sonivoxrender ants.mid | sox -t s16 -c 2 -r 44100 - ants.wav

Example 5: pipe the rendered audio thru the PulseAudio's 'pacat' utility:

    $ sonivoxrender ants.mid | pacat

You may replace "ants.mid" by another MIDI or XMF file, like "test/res/testmxmf.mxmf"

## Unit tests

The Android unit tests have been integrated in the CMake build system, with little modifications. A requirement is GoogleTest, either installed system wide or it will be downloaded from the git repository. 

It is **strongly** recommended that you run the test suite after changing some code or before trying the library on a new platform/compiler. Some compiler versions are known to output crashing products. Running the unit tests is a practical way to quickly detect this problem.

To run the tests, you may use this command:

    $ cmake --build <build_directory> --target test
        
or simply:

    $ ctest

There are two environment variables that you may set before running the tests (mandatory for the Qt Creator integrated test runner).

    TEMP		< path to a temporary location with write permission for the output file >
    TEST_RESOURCES	< path to the location of the input MIDI files: source_directory/test/res/ for instance >

## License

Copyright (c) 2022-2024 Pedro López-Cabanillas.

Copyright (c) 2008-2024, The Android Open Source Project.

Copyright (c) 2004-2006 Sonic Network Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
