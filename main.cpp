#include <Windows.h>
#include "Wallpaper.h" // From M$
#using <system.drawing.dll> // C++/CLR
using namespace std;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

// Using M$'s sample to solve it.
// Attention: XP doesn't support JPG or PNG. So it is necessary to convert those formats to BMP.
// Here I am using C++/CLR
// Necessary to install VC++ 2015 Library and .NET 4.0

const int MAX = 50000;

#pragma unmanaged
// This pragma is used to set breakpoint in unmanaged (Not CLR) C++ code.

void WriteLog(LPSTR lpWord, HANDLE hFile)
{
	WriteFile(hFile, lpWord, strlen(lpWord), NULL, NULL);
}

void WriteTimeToLog(HANDLE hFile)
{
	SYSTEMTIME NowTime;
	char       ReturnTimeString[MAX];
	GetLocalTime(&NowTime);

	// I don't want to use ANSI but I have to compromise. Unicode just brings me too much trouble.
	wsprintfA(ReturnTimeString, ("%d Äê %d ÔÂ %d ÈÕ %d:%d:%d:%d "), NowTime.wYear, NowTime.wMonth, NowTime.wDay, NowTime.wHour, NowTime.wMinute, NowTime.wSecond, NowTime.wMilliseconds);
	WriteLog(ReturnTimeString, hFile);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Configuring settings.ini
	if (GetFileAttributes(TEXT("settings.ini")) == 0xFFFFFFFF) // settings.ini exists?
	{
		/* WritePrivateProfileString() will automatically create the settings.ini */
		DWORD snapshot = GetLastError();
		WritePrivateProfileString(TEXT("Settings"), 
								  TEXT("Log"), 
								  TEXT("WallpaperReplacer.log"), 
								  TEXT("./settings.ini"));
		WritePrivateProfileString(TEXT("Settings"),
								  TEXT("Location"),
								  TEXT(""),
								  TEXT("./settings.ini"));
		WritePrivateProfileString(TEXT("Settings"),
								  TEXT("Time"),
								  TEXT("300"), // 5 mins default
								  TEXT("./settings.ini"));
		if (snapshot != GetLastError()) MessageBox(NULL, TEXT("There's something wrong while creating settings.ini. This program will exit now."), TEXT("Error"), MB_OK);
		else MessageBox(NULL, TEXT("Please modify the settings.ini to configure WallpaperReplacer. This program will exit now."), TEXT("Attention"), MB_OK);
		return 0;
	}
	
	// Reading settings.ini
	int Time;
	TCHAR LogFilePath[MAX], PicFileDic[MAX];
	GetPrivateProfileString(TEXT("Settings"),
							TEXT("Log"),
							NULL,
							LogFilePath,
							MAX,
							TEXT("./settings.ini"));
	GetPrivateProfileString(TEXT("Settings"),
							TEXT("Location"),
							NULL,
							PicFileDic,
							MAX,
							TEXT("./settings.ini"));
	Time = GetPrivateProfileInt(TEXT("Settings"), 
								TEXT("Time"), 
								-1, 
								TEXT("./settings.ini"));
	// Juding whether these strings are vaild.
	if (Time <= 0 || GetFileAttributes(PicFileDic) != FILE_ATTRIBUTE_DIRECTORY)
	{
		MessageBox(NULL, TEXT("Please modify the settings.ini correctly. This program will exit now."), TEXT("Warning"), MB_OK);
		return 1;
	}

	HANDLE LogFile;
	LogFile = CreateFile(LogFilePath, 
						 // GENERIC_READ | GENERIC_WRITE, 
						 FILE_APPEND_DATA,
						 FILE_SHARE_READ, 
						 NULL, 
						 OPEN_ALWAYS, 
						 FILE_ATTRIBUTE_NORMAL, 
						 NULL);
	if (LogFile == INVALID_HANDLE_VALUE)
		MessageBox(NULL, TEXT("There is something wrong with the Log file. No log will be recorded."), TEXT("Warning"), MB_OK);

	WriteLog("Welcome to use WallpaperReplacer. Time: ", LogFile);
	WriteTimeToLog(LogFile);
	WriteLog("\r\n", LogFile);
	
	// Creating temporary directory in %TMP%
	TCHAR TMPDic[MAX];
	GetEnvironmentVariable(L"TMP", TMPDic, MAX);
	lstrcat(TMPDic, TEXT("\\WallpaperReplacer"));
	CreateDirectory(TMPDic, NULL); // Create a directory no matter exists or not
	


	return 0;
}