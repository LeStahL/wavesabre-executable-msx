# wavesabre-executable-msx
WaveSabre executable music example code.

# Build instructions
* Clone the repository.
* Install CMake (from <https://cmake.org/download/>).
* Install Git (from <https://git-scm.com/download>).
* Install the Visual C++ Toolkit 2022 (from <https://aka.ms/vs/17/release/vs_BuildTools.exe>).
* Add the binary folders of both of those to your `PATH` variable.
* Run `git submodule update --init --recursive` in the source root of this repository.
* Create an out-of-source build folder, for example `build`.
* Run `cmake [SOURCE_ROOT] -A Win32`.
* Run `cmake --build . --config Release`. You might need to re-enter the command once.

# How to change the track
* Place a DAW project file containing your track in the `Track/` subdirectory (or anywhere else, if you must).
* Open CMakeLists.txt.
* Change the variables for `EXECUTABLE_NAME`, `TRACK_NAME`, `TRACK_AUTHOR`, `TRACK_PROJECT_FILE` or `N_RENDER_THREADS` according to your needs. Make sure to change `TRACK_PROJECT_FILE` to exactly the path you chose to save your project file to.

# Licenses
* wavesabre-executable-msx is GPLv3 and (c) 2022 Alexander Kraus <nr4@z10.info>; see LICENSE for details.
* WaveSabre is MIT and (c) <https://github.com/yupferris> and <https://github.com/djh0ffman> of <https://github.com/logicomacorp>. See WaveSabre/LICENSE for details.
