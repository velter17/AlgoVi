#
# This file is part of the Novile Editor
#
# This program is free software licensed under the GNU LGPL. You can
# find a copy of this license in LICENSE in the top directory of
# the source code.
#
# Copyright 2013    Illya Kovalevskyy   <illya.kovalevskyy@gmail.com>
#

# Versions are set = major.minor.patch
set(NOVILE_VERSION_MAJOR "0")
set(NOVILE_VERSION_MINOR "5")
set(NOVILE_VERSION_PATCH "0")
set(NOVILE_VERSION ${NOVILE_VERSION_MAJOR}.${NOVILE_VERSION_MINOR}.${NOVILE_VERSION_PATCH})


# library is based on webkit widgets + its deps
find_package(Qt5WebKitWidgets REQUIRED)

add_subdirectory(src)

add_subdirectory(example)
