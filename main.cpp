#include <Windows.h>
#include "Wallpaper.h" // From M$
// #include "FreeImage/FreeImage.h"
// #pragma comment(lib, "FreeImage/FreeImage.lib") // Support JPG and PNG (Convert to BMP)
// #include <GdiPlus.h>
//#pragma comment(lib, "Gdiplus.lib")
#using <system.drawing.dll>
using namespace std;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

// Using M$'s sample to solve it.
// Attention: XP doesn't support JPG or PNG. So it is necessary to convert those formats to BMP.
// Here I am using C++/CLR

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WallpaperStyle style;
	style = Center;
	wchar_t a[] = L"C:\\Users\\lenovo\\Documents\\WallpaperReplacer\\test.jpg";
	HRESULT hr = SetDesktopWallpaper(a, style);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error", L"a", MB_OK);
	}
	Image^ image = Image::FromFile("C:\\Users\\lenovo\\Documents\\WallpaperReplacer\\test.jpg");
	image->Save("test.png", ImageFormat::Png);
	image->Save("test123.bmp", ImageFormat::Bmp);


	return 0;
}