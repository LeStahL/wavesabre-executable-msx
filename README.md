# wavesabre-executable-msx
WaveSabre executable music example code.

# Build instructions
* Clone the repository.
* Install CMake (from <https://cmake.org/download/>).
* Install Git (from <https://git-scm.com/download>).
* Install the Visual C++ Toolkit 2022 (from <https://aka.ms/vs/17/release/vs_BuildTools.exe>).
* Install the .NET Framework Developer pack 4.6.1 (from <https://www.microsoft.com/de-de/download/details.aspx?id=49978>).
* Add the binary folders of all of those to your `PATH` variable.
* Run `git submodule update --init --recursive` in the source root of this repository.
* Create an out-of-source build folder, for example `build`.
* Run `cmake [SOURCE_ROOT] -A Win32`.
* Run `cmake --build . --config Release`. You might need to re-enter the command once.
* In the folder `[BUILD_FOLDER]/Release` you can now find your binaries and a release archive.

# How to change the track
* Place a DAW project file containing your track in the `Track/` subdirectory (or anywhere else, if you must).
* Either open CMakeLists.txt and change the variables for `EXECUTABLE_NAME`, `TRACK_NAME`, `TRACK_AUTHOR`, `TRACK_PROJECT_FILE`, `PRECALC` or `N_RENDER_THREADS` according to your needs. Make sure to change `TRACK_PROJECT_FILE` to exactly the path you chose to save your project file to, or override them by supplying `-DEXECUTABLE_NAME="YourName"` in the command line of the first CMake command above.

# Licenses
* wavesabre-executable-msx is GPLv3 and (c) 2022 Alexander Kraus <nr4@z10.info>; see LICENSE for details.
* WaveSabre is MIT and (c) <https://github.com/yupferris> and <https://github.com/djh0ffman> of <https://github.com/logicomacorp>. See WaveSabre/LICENSE for details.
