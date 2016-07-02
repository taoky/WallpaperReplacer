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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Configuring settings.ini
	if (GetFileAttributes(L"settings.ini") == 0xFFFFFFFF) // settings.ini exists?
	{
		/* WritePrivateProfileString() will automatically create the settings.ini */
		DWORD snapshot = GetLastError();
		WritePrivateProfileString(TEXT("Settings"), 
								  TEXT("Log"), 
								  TEXT("\"WallpaperReplacer.log\""), 
								  TEXT("./settings.ini"));
		WritePrivateProfileString(TEXT("Settings"),
								  TEXT("Location"),
								  TEXT("\"\""),
								  TEXT("./settings.ini"));
		WritePrivateProfileString(TEXT("Settings"),
								  TEXT("Time"),
								  TEXT("300"), // 5 mins default
								  TEXT("./settings.ini"));
		if (snapshot != GetLastError()) MessageBox(NULL, L"There's something wrong while creating settings.ini. This program will exit now.", L"Error", MB_OK);
		else MessageBox(NULL, L"Please modify the settings.ini to configure WallpaperReplacer. This program will exit now.", L"Attention", MB_OK);
		return 0;
	}

	// Reading settings.ini
	GetPrivateProfileString()

	return 0;
}