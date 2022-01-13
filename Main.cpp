/* wavesabre-executable-msx
* Copyright (C) 2022  Alexander Kraus <nr4@z10.info>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "WaveSabrePlayerLib.h"
using namespace WaveSabrePlayerLib;
#include "music.h"

IPlayer *player;

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include <commctrl.h>

HWND hWnd,
	hPlayButton,
	hWriteWavButton,
	hProgressBar,
	hTrackbar,
	hExitButton,
	hTimeElapsed;

#define PLAY_BUTTON 1
#define SAVE_WAV_BUTTON 2
#define TRACK_BAR 4
#define PROGRESS_BAR 8
#define TIMER 16
#define EXIT_BUTTON 32

void drawProgressBar(double progress, void *data)
{
	SendMessage(hProgressBar, PBM_SETPOS, 100 * progress, 0);
	if(progress == 1.) 
	{
		EnableWindow(hWriteWavButton, TRUE);
		EnableWindow(hPlayButton, TRUE);
	}
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT id = LOWORD(wParam);
	double time;
	int seconds, minutes, hours;
	char timerText[64];
	
	switch(uMsg)
	{
		case WM_COMMAND:
			switch(id)
			{
				case PLAY_BUTTON:
					player->Play();
					EnableWindow(hPlayButton, FALSE);
					break;

				case SAVE_WAV_BUTTON:
					WavWriter(&Song, N_RENDER_THREADS).Write(WAVE_FILE_NAME, drawProgressBar, nullptr);
					EnableWindow(hWriteWavButton, FALSE);
					break;

				case EXIT_BUTTON:
					PostQuitMessage(0);
					break;
			}
			break;
			
		case WM_CLOSE:
			ExitProcess(0);
			break;

		case WM_TIMER:
			time = player->GetSongPos();
			if(time >= player->GetLength()) KillTimer(hWnd, TIMER);
			seconds = time;
			minutes = seconds / 60;
			hours = minutes / 60;
			SendMessage(hTrackbar, TBM_SETPOS, (WPARAM) TRUE, (LPARAM) time);
			sprintf(timerText, "%02d:%02d:%02d", hours, minutes - hours * 60, seconds - minutes * 60 - hours * 3600);
			SetWindowText(hTimeElapsed, (LPCSTR)timerText); 
			break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = &DialogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)EXECUTABLE_NAME;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);
	
	hWnd = CreateWindowEx(0, wc.lpszClassName, (LPCSTR)EXECUTABLE_NAME, WS_OVERLAPPEDWINDOW, 200, 200, 339, 150, NULL, NULL, hInstance, 0);
	
	CreateWindow(WC_STATIC, (LPCSTR)"", WS_VISIBLE | WS_CHILD | SS_LEFT, 0,0,321,180, hWnd, NULL, hInstance, NULL);
	CreateWindow(WC_STATIC, TRACK_NAME_AUTHOR, WS_VISIBLE | WS_CHILD | SS_LEFT, 10,10,301,20, hWnd, NULL, hInstance, NULL);
	
	hTimeElapsed = CreateWindow(WC_STATIC, (LPCSTR)"Loading...", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 53, 70, 20, hWnd, NULL, hInstance, NULL);

	hPlayButton = CreateWindow(WC_BUTTON, (LPCSTR)"Play", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 77, 100, 25, hWnd, (HMENU)PLAY_BUTTON, hInstance, NULL);
	hWriteWavButton = CreateWindow(WC_BUTTON, (LPCSTR)"Write .wav", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 111, 77, 99, 25, hWnd, (HMENU)SAVE_WAV_BUTTON, hInstance, NULL);
	hExitButton = CreateWindow(WC_BUTTON, (LPCSTR)"Exit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 211, 77, 100, 25, hWnd, (HMENU)EXIT_BUTTON, hInstance, NULL);
	EnableWindow(hPlayButton, FALSE);
	EnableWindow(hWriteWavButton, FALSE);
	
	hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, (LPSTR)NULL, WS_VISIBLE | WS_CHILD, 10, 30, 301, 20, hWnd, (HMENU)PROGRESS_BAR, hInstance, NULL);
	SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100)); 
	SendMessage(hProgressBar, PBM_SETSTEP, (WPARAM)1, 0);

	hTrackbar = CreateWindowEx(0, TRACKBAR_CLASS, (LPCSTR)"Music Trackbar", WS_CHILD | WS_VISIBLE, 80, 53, 238, 20, hWnd, (HMENU) TRACK_BAR, hInstance, NULL); 
	SendMessage(hTrackbar, TBM_SETTICFREQ, 10, NULL);
	EnableWindow(hTrackbar, false);

	ShowWindow(hWnd, TRUE);
	UpdateWindow(hWnd);

	player = new PreRenderPlayer(&Song, 8, drawProgressBar, nullptr);
	SendMessage(hTrackbar, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(0, player->GetLength()));

	MSG msg = {0};
	SetTimer(hWnd, TIMER, 100, (TIMERPROC) NULL); 
	
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}