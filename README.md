# AlgoVi

This is an initial README for AlgoVi project.

## Overview

Algovi is a software for testing your algorithms. It is a custom command line tool with a lot of functions, which are based on ACM-ICPC problem styles.

Currently available functions:
* Run source code from command line
* Handle tests and run source code on them
* Test your source code on test archive
* Debug your code using colorized output

Main features:
* Automated code compilation
* Parse tests from websites (now is codeforces only)
* Ability to use custom parser for specific website
* Ability to use custom debug program
* Automated tester with useful information
* Handle WA, TLE, RE, AC verdicts
* Builtin testlib checkers
* Ability to use custom checker
* Ability to read test archive from folder
* Ability to save test archive to folder

## Installing

You can use AlgoVi on Linux, Windows and MacOS.
Tested on Ubuntu 14.04, 16.04 and Windows 7.
WARNING: Development still in active process, so master can be partly broken.
Dependencies: Qt, Boost, python

To build project you need to launch build.py script

On linux:
```./build.py```.

On windows: ```build.py -t win```.

After build process final application will be on ```$ALGOVI_PATH$/app/linux[win]-Release/bin/algovi[.exe]```

## Possible problems

Can be a lot of. Sorry for that. This readme file is just a first one.

## Support

If you have any questions or bugs to report, feel free to write email on support@algovi.freshdesk.com

