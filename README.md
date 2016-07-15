# WallpaperReplacer
## Brief Description
A simple program to help change wallpapers at regular intervals for Windows XP (and above). 

This project is a C++/CLR code and is created and compiled by Visual Studio 2015 so it is necessary to install:

[Microsoft Visual C++ 2015 Redistributable](https://www.microsoft.com/en-us/download/details.aspx?id=48145)

and

[Microsoft .Net Framework 4](https://www.microsoft.com/en-us/download/details.aspx?id=17851)
## History
I had never touched Win32 programming before this summer vocation. 

However, when in school, Jin Ouliang had a desire to change the wallpaper of our classroom's computer. Windows XP is installed so there isn't any built-in function to change the wallpapers automatically. There are some apps online but they are not so trustworthy. For this reason I write this small program to aid changing wallpapers.

(There are many dirty hacks in the code. Sorry for that.)

OS: Windows XP and above (theoretically, but I only tested on Windows XP and Windows 10)
## Configuration
After WallpaperReplacer is run for the first time, it will create a **settings.ini** in its folder and open a dialog box to ask you to edit it.
### Format of this **settings.ini** file
it looks like this:
```
[Settings]
Log=WallpaperReplacer.log
Location=C:\Users\lenovo\Documents\test
Time=300
Style=Stretch
```

Log: Where you want to put the log file.

Location: Where the pictures are.

Time: How long the time between each picture.

Style: You can choose **Tile**, **Center** or **Stretch**.

After you have edited **settings.ini** properly, WallpaperReplacer will run smoothly. If not, WallpaperReplacer will report an error to you and stop running.

### TMP directory
After editing **settings.ini** properly, WallpaperReplacer will copy all picture files (*.bmp, *.jpg, *.png) to `%TMP%\WallpaperReplacer\`. You need to make sure that the environment variable `%TMP%` is correct.

If you are running Windows XP, WallpaperReplacer will help you convert the pictures to *.bmp in the TMP directory, because Windows XP itself DOES NOT support *.jpg or *.png as wallpapers.

WallpaperReplacer will create a file named `Flag` in the TMP directory. **DO NOT EDIT** it unless you want to change pictures in the TMP directory. When `Flag` does not exist, WallpaperReplacer will delete all files and copy pictures in the TMP directory.
## Others
### Why using .Net in the project?
I need to use .Net to convert pictures to .bmp in the Windows XP environment. It's easier to use .Net to convert pictures than in other ways.
### Compling in other versions of Visual Studio
Though not tested, it is proper to compile in at least Visual Studio 2010 in all probability. Please pay attention that you need to change these options:

`Common Language Runtime Support (/clr)` in General.

`Platform Toolset: Visual Studio 2015 - Windows XP (v140_xp)` in General (If you are using VS 2012 or above)

`Stack Reserve Size: 20000000` in Linker -> System
## Acknowledge
This project uses [a sample code](https://code.msdn.microsoft.com/CppSetDesktopWallpaper-eb969505) from Microsoft, and it is in Apache License.

Thank you.
