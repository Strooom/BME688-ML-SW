This script runs before platformio runs
* it gets the latest release from git, eg "v1.2.3" and splits that into separate numbers main, minor, patch
* gets the latest commit hash from git
* creates a timestamp

Then it writes all this info into buildinfo.cpp file so the firmware 'knows' what version, buildtype, buildtime etc it is.
