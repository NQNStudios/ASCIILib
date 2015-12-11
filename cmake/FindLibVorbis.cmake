# Locate Vorbis library
# This module defines
# VORBIS_LIBRARY, the name of the library to link against
# VORBIS_FOUND, if false, do not try to link to libvorbis
# VORBIS_INCLUDE_DIR, where to find header files

FIND_PATH (VORBIS_INCLUDE_DIR vorbisfile.h
    HINTS
    ${ENV}
    PATH_SUFFIXES include/vorbis include
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local/include/vorbis
    /usr/include/vorbis
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
    )

MESSAGE ("VORBIS_INCLUDE_DIR is ${VORBIS_INCLUDE_DIR}")

FIND_LIBRARY (VORBIS_LIBRARY
    NAMES vorbis
    HINTS
    ${ENV}
    PATH_SUFFIXES lib64 lib
    PATHS
    /sw
    /opt/local
    /opt/csw
    /opt
    )

MESSAGE ("VORBIS_LIBRARY is ${VORBIS_LIBRARY}")

SET (VORBIS_FOUND "NO")
IF (VORBIS_LIBRARY)
    SET (VORBIS_FOUND "YES")
ENDIF (VORBIS_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS (Vorbis
    REQUIRED_VARS VORBIS_LIBRARY VORBIS_INCLUDE_DIR)
