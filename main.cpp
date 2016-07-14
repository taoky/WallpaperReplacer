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
BOOL isnotXP = SupportJpgAsWallpaper();
HANDLE LogFile;
DWORD AvoidCrash = NULL;
// I don't know why I have to use this way to fill the 4th para of WriteFile(). But I know that if not, 
// this program could crash in Windows XP.
// It's too unelegant.

void WriteLog(LPSTR lpWord, HANDLE hFile)
{
	WriteFile(hFile, lpWord, strlen(lpWord), &AvoidCrash, NULL);	
}

void WriteLog(LPTSTR lpWord, HANDLE hFile)
{
	char Word[MAX];
	WideCharToMultiByte(CP_ACP, 0, lpWord, -1, Word, MAX, NULL, NULL); // Unicode to ANSI
	WriteFile(hFile, Word, strlen(Word), &AvoidCrash, NULL);
}

void WriteTimeToLog()
{
	SYSTEMTIME NowTime;
	TCHAR      ReturnTimeString[MAX];
	GetLocalTime(&NowTime);
	wsprintf(ReturnTimeString, L"%d Äê %d ÔÂ %d ÈÕ %d:%d:%d:%d ", NowTime.wYear, NowTime.wMonth, NowTime.wDay, NowTime.wHour, NowTime.wMinute, NowTime.wSecond, NowTime.wMilliseconds);
	WriteLog(ReturnTimeString, LogFile);
}

void CopyPic(LPTSTR FromPath, LPTSTR ToPath)
{
	LPTSTR List[] = {L"*.jpg", L"*.png", L"*.bmp"}; // Warning: DO NOT SUPPORT *.jpeg
	lstrcat(FromPath, L"\\");
	lstrcat(ToPath, L"\\");
	TCHAR tFromPath[MAX];

	WriteTimeToLog();
	WriteLog("Preparing to copy pictures. From: \r\n", LogFile);
	WriteLog(FromPath, LogFile);
	WriteLog("\r\n", LogFile);
	WriteLog("To: \r\n", LogFile);
	WriteLog(ToPath, LogFile);
	WriteLog("\r\n", LogFile);

	// Clearing all pictures in tmpdic.
	TCHAR tToPath[MAX];
	lstrcpy(tToPath, ToPath);
	lstrcat(tToPath, L"*");
	WIN32_FIND_DATA toDelPicData;
	HANDLE toDelPicFile = FindFirstFile(tToPath, &toDelPicData);
	WriteLog("Clearing pictures in tmpdic.\r\n", LogFile);
	if (toDelPicFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (lstrcmp(toDelPicData.cFileName, L"Flag"))
			{
				TCHAR DelFileName[MAX];
				lstrcpy(DelFileName, ToPath);
				lstrcat(DelFileName, toDelPicData.cFileName);

				WriteTimeToLog();
				WriteLog("Delete: ", LogFile);
				WriteLog(DelFileName, LogFile);
				WriteLog("\r\n", LogFile);

				DeleteFile(DelFileName);
			}
		} while (FindNextFile(toDelPicFile, &toDelPicData));
	}
	

	for (int i = 0; i < 3; i++)
	{
		lstrcpy(tFromPath, FromPath);
		lstrcat(tFromPath, List[i]);
		WIN32_FIND_DATA hFileData;
		HANDLE hFile = FindFirstFile(tFromPath, &hFileData);
		
		if (hFile == INVALID_HANDLE_VALUE)
		{
			WriteTimeToLog();
			WriteLog("Unable to find", LogFile);
			WriteLog(List[i], LogFile);
			WriteLog("Skip. \r\n", LogFile);
			continue;
		}
		do
		{
			if (hFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) // Avoid directory with name *.jpg
			{
				TCHAR FromFilePath[MAX], ToFilePath[MAX];
				wsprintf(FromFilePath, L"%s%s", FromPath, hFileData.cFileName);
				wsprintf(ToFilePath, L"%s%s", ToPath, hFileData.cFileName);

				WriteLog("From: \r\n", LogFile);
				WriteLog(FromFilePath, LogFile);
				WriteLog("\r\n", LogFile);
				WriteLog("To: \r\n", LogFile);
				WriteLog(ToFilePath, LogFile);
				WriteLog("\r\n", LogFile);
				CopyFile(FromFilePath, ToFilePath, FALSE);
				if (!isnotXP && wcsstr(_wcslwr(hFileData.cFileName), L".jpg") || wcsstr(_wcslwr(hFileData.cFileName), L".png"))
				{
					WriteLog("Converting", LogFile);
					System::String ^CLRFilePath = gcnew System::String(ToFilePath);
					Image^ image = Image::FromFile(CLRFilePath);
					image->Save(CLRFilePath + ".bmp", ImageFormat::Bmp);
				}
			}
		} while (FindNextFile(hFile, &hFileData));

	}
}

/* void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	
} */
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
		WritePrivateProfileString(TEXT("Settings"),
								  TEXT("Style"),
								  TEXT("Stretch"), // Center, Tile, Stretch supported in WinXP.
								  TEXT("./settings.ini"));
		if (snapshot != GetLastError()) MessageBox(NULL, TEXT("There's something wrong while creating settings.ini. This program will exit now."), TEXT("Error"), MB_OK);
		else MessageBox(NULL, TEXT("Please modify the settings.ini to configure WallpaperReplacer. This program will exit now."), TEXT("Attention"), MB_OK);
		return 0;
	}
	
	// Reading settings.ini
	unsigned int Time;
	TCHAR LogFilePath[MAX], PicFileDic[MAX], Style[MAX];
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
	GetPrivateProfileString(TEXT("Settings"),
							TEXT("Style"),
							NULL,
							Style,
							MAX,
							TEXT("./settings.ini"));
	Time = GetPrivateProfileInt(TEXT("Settings"), 
								TEXT("Time"), 
								-1, 
								TEXT("./settings.ini"));
	// Juding whether these strings are vaild.
	if (Time * 1000 <= 0 || GetFileAttributes(PicFileDic) != FILE_ATTRIBUTE_DIRECTORY ||
		(lstrcmp(Style, L"Center") && lstrcmp(Style, L"Tile") && lstrcmp(Style, L"Stretch")))
	{
		MessageBox(NULL, TEXT("Please modify the settings.ini correctly. This program will exit now."), TEXT("Warning"), MB_OK);
		return 1;
	}
	WallpaperStyle WStyle;
	if (!lstrcmp(Style, L"Center")) WStyle = Center;
	else if (!lstrcmp(Style, L"Tile")) WStyle = Tile;
	else WStyle = Stretch;

	HANDLE iniFile;
	LogFile = CreateFile(LogFilePath, 
						 // GENERIC_READ | GENERIC_WRITE, 
						 FILE_APPEND_DATA,
						 FILE_SHARE_READ, 
						 NULL, 
						 OPEN_ALWAYS, 
						 FILE_ATTRIBUTE_NORMAL, 
						 NULL);
	iniFile = CreateFile(TEXT("settings.ini"),
						 GENERIC_READ | GENERIC_WRITE, 
						 // FILE_APPEND_DATA,
						 FILE_SHARE_READ,
						 NULL,
						 OPEN_ALWAYS,
						 FILE_ATTRIBUTE_NORMAL,
						 NULL);
	if (LogFile == INVALID_HANDLE_VALUE)
		MessageBox(NULL, TEXT("There is something wrong with the Log file. No log will be recorded."), TEXT("Warning"), MB_OK);

	WriteLog("Welcome to use WallpaperReplacer. Time: ", LogFile);
	WriteTimeToLog();
	WriteLog("\r\n", LogFile);
	
	// Creating temporary directory in %TMP%
	TCHAR TMPDic[MAX];
	GetEnvironmentVariable(L"TMP", TMPDic, MAX);
	lstrcat(TMPDic, TEXT("\\WallpaperReplacer"));
	CreateDirectory(TMPDic, NULL); // Create a directory no matter exists or not
	
	TCHAR FlagPath[MAX];
	lstrcpy(FlagPath, TMPDic);
	lstrcat(FlagPath, TEXT("\\Flag"));
	HANDLE FlagFile = CreateFile(FlagPath, 
								 GENERIC_READ | GENERIC_WRITE,
							 	 FILE_SHARE_READ,
								 NULL,
								 OPEN_ALWAYS,
								 FILE_ATTRIBUTE_NORMAL,
								 NULL);
	if (FlagFile == INVALID_HANDLE_VALUE)
	{
		WriteTimeToLog();
		WriteLog("Serious Error: Unable to deal with FlagFile. Exit.\r\n", LogFile);
		return 0;
	}
	FILETIME iniLastChangeFileTime;
	GetFileTime(iniFile, NULL, NULL, &iniLastChangeFileTime);
	
	if (GetLastError() == ERROR_ALREADY_EXISTS) // FlagFile exists.
	{
		FILETIME FlagLastChangeFileTime;
		GetFileTime(FlagFile, NULL, NULL, &FlagLastChangeFileTime);
		LONG CompareResult = CompareFileTime(&iniLastChangeFileTime, &FlagLastChangeFileTime);
		if (CompareResult == 1) // ini changed after Flag
		{
			WriteTimeToLog();
			WriteLog("FlagFile has changed. Copying Pictures.\r\n", LogFile);
			SetFileTime(FlagFile, NULL, NULL, &iniLastChangeFileTime);
			CopyPic(PicFileDic, TMPDic);
		}
	}
	else
	{
		WriteFile(FlagFile, "!!!!!DO NOT EDIT!!!!!", 21, &AvoidCrash, NULL);
		SetFileTime(FlagFile, NULL, NULL, &iniLastChangeFileTime);
		WriteTimeToLog();
		WriteLog("FlagFile has set up. Copying Pictures.\r\n", LogFile);
		CopyPic(PicFileDic, TMPDic);
	}

	// Setting Timer and begin to change wallpaper
	/* SetTimer(NULL, 0, Time * 1000, &TimerProc);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_TIMER)
		{
			DispatchMessage(&msg);
		}
	} */
	WriteLog("Preparing to set wallpapers.\r\n", LogFile);
	while (1)
	{
		TCHAR SetPicPath[MAX];
		lstrcpy(SetPicPath, TMPDic);
		lstrcat(SetPicPath, L"\\*");
		WIN32_FIND_DATA PicData;
		HANDLE PicFile = FindFirstFile(SetPicPath, &PicData);
		  MessageBox(NULL, SetPicPath, L"SetPicPath", NULL);
		do
		{
			if (lstrcmp(PicData.cFileName, L"Flag") && 
				(wcsstr(_wcslwr(PicData.cFileName), L".jpg")) || wcsstr(_wcslwr(PicData.cFileName), L".bmp") || wcsstr(_wcslwr(PicData.cFileName), L".png"))
			{
				if (!isnotXP && (wcsstr(_wcslwr(PicData.cFileName), L".jpg") || wcsstr(_wcslwr(PicData.cFileName), L".png")))
					continue;
				TCHAR Path[MAX];
				wsprintf(Path, L"%s\\%s", TMPDic, PicData.cFileName);
				WriteLog(Path, LogFile);
				WriteLog("\r\n", LogFile);
				   MessageBox(NULL, Path, L"P", NULL);
				SetDesktopWallpaper(Path, WStyle);
				Sleep(Time * 1000);
			}
		} while (FindNextFile(PicFile, &PicData));
		
	}
	return 0;
}