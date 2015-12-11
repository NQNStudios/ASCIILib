# Locate OGG library
# This module defines
# OGG_LIBRARY, the name of the library to link against
# OGG_FOUND, if false, do not try to link to libogg
# OGG_INCLUDE_DIR, where to find ogg.h

FIND_PATH (OGG_INCLUDE_DIR ogg.h
    HINTS
    ${ENV}
    PATH_SUFFIXES include/ogg include
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include/ogg
    /usr/include/ogg
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
    )

MESSAGE ("OGG_INCLUDE_DIR is ${OGG_INCLUDE_DIR}")

FIND_LIBRARY (OGG_LIBRARY
    NAMES ogg
    HINTS
    ${ENV}
    PATH_SUFFIXES lib64 lib
    PATHS
    /sw
    /opt/local
    /opt/csw
    /opt
    )

MESSAGE ("OGG_LIBRARY is ${OGG_LIBRARY}")

SET (OGG_FOUND "NO")
IF (OGG_LIBRARY)
    SET (OGG_FOUND "YES")
ENDIF (OGG_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS (OGG
    REQUIRED_VARS OGG_LIBRARY OGG_INCLUDE_DIR)
