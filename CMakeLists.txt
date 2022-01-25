# wavesabre-executable-msx
# Copyright (C) 2022  Alexander Kraus <nr4@z10.info>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.14)

project(wavesabre-executable-msx)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(DOWNLOAD_VST3SDK ON)
set(BUILD_PROJECT_MANAGER OFF)
set(BUILD_WAVESABRE_CONVERT_TESTS OFF)
add_subdirectory(WaveSabre)

# Enter information about your product here.
set(EXECUTABLE_NAME example-project CACHE STRING "Name of the executable.")
set(TRACK_NAME "Example track" CACHE STRING "Name of the track.")
set(TRACK_AUTHOR "Example author" CACHE STRING "Author of the track.")
set(TRACK_PROJECT_FILE "${PROJECT_SOURCE_DIR}/WaveSabre/Tests/WaveSabreConvertTests/Tests/Punqtured's Tune.xrns" CACHE PATH "Path to the track project file.")
set(N_RENDER_THREADS 8 CACHE INT "Number of threads to render your product with.")
set(PRECALC OFF CACHE BOOL "Configure if track will be precalced or computed while playing.")

add_custom_target(music.h ALL ConvertTheFuck ${TRACK_PROJECT_FILE} -h music.h)

add_executable(${EXECUTABLE_NAME} WIN32 Main.cpp)
set(WAVESABRE_VST_PLUGINS
	Adultery
	Cathedral
	Chamber
	Crusher
	Echo
	Falcon
	Leveller
	Scissor
	Slaughter
	Smasher
	Specimen
	Thunder
	Twister
)
target_link_libraries(${EXECUTABLE_NAME} 
	WaveSabreCore
	WaveSabrePlayerLib

	User32.lib
	Kernel32.lib
	Gdi32.lib
	Shell32.lib
	msvcrt.lib

	${WAVESABRE_VST_PLUGINS}
)

set(PRECALC_OPTION 0)
if(${PRECALC})
	set(PRECALC_OPTION 1)
endif()

target_compile_definitions(${EXECUTABLE_NAME} PUBLIC 
	EXECUTABLE_NAME="${EXECUTABLE_NAME}"
	TRACK_NAME_AUTHOR="${TRACK_NAME} by ${TRACK_AUTHOR}"
	WAVE_FILE_NAME="${EXECUTABLE_NAME}.wav"
	N_RENDER_THREADS=${N_RENDER_THREADS}
	PRECALC=${PRECALC_OPTION}
)

target_include_directories(${EXECUTABLE_NAME} PUBLIC 
    ${PROJECT_SOURCE_DIR}/WaveSabre/WaveSabreCore/include
    ${PROJECT_SOURCE_DIR}/WaveSabre/WaveSabrePlayerLib/include
)

target_compile_options(${EXECUTABLE_NAME} PUBLIC /MD)

add_dependencies(${EXECUTABLE_NAME} music.h)

set(SQUISHY_PATHS
    "${CMAKE_CURRENT_BINARY_DIR}/squishy-0.1.3/"
    "${CMAKE_CURRENT_BINARY_DIR}/squishy-0.2.0/"
)
find_program(SQUISHY NAMES squishy-x86.exe HINTS ${SQUISHY_PATHS})
if(${SQUISHY} MATCHES SQUISHY-NOTFOUND)
	message(STATUS "Squishy-x86 not present; will download.")
	file(DOWNLOAD http://logicoma.io/squishy/releases/squishy-0.1.3.zip squishy-0.1.3.zip SHOW_PROGRESS)
	file(ARCHIVE_EXTRACT INPUT squishy-0.1.3.zip DESTINATION squishy-0.1.3/ VERBOSE)
endif()
find_program(SQUISHY NAMES squishy-x86.exe HINTS ${SQUISHY_PATHS})
message(STATUS "Found Squishy: ${SQUISHY}")

add_custom_command(TARGET ${EXECUTABLE_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${SQUISHY} -i $<TARGET_FILE:${EXECUTABLE_NAME}> -o "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/${EXECUTABLE_NAME}-squished.exe" POST_BUILD)

add_custom_command(TARGET ${EXECUTABLE_NAME} WORKING_DIRECTORY $<TARGET_FILE_DIR:${EXECUTABLE_NAME}> 
    COMMAND ${CMAKE_COMMAND} -E tar cfv "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/${EXECUTABLE_NAME}-${TRACK_AUTHOR}.zip" --format=zip
        "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/${EXECUTABLE_NAME}-squished.exe"
        COMMENT "Packing release zip archive."
    POST_BUILD
)
