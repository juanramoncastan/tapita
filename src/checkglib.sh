#!/bin/bash

# This script is called by makefile and it sets g_thread_support() function in "gthreadsupport.h" file
# to give threads support to glib older than 2.32 version.

OUTFILE="gthreadsupport.h"

GLIB_MAJOR_VERSION=$(pkg-config --modversion glib-2.0 | sed -e "s/\([0-9]\)\(\.[0-9]\{,2\}\)\(\..*\)/\1/" )
GLIB_MINOR_VERSION=$(pkg-config --modversion glib-2.0 | sed -e "s/\(${GLIB_MAJOR_VERSION}\.\)\([0-9]\{,2\}\)\(\..*\)/\2/" )
echo "// Checked glib version"  > ${OUTFILE}
echo -n "// " >> ${OUTFILE}
echo "Glib version ${GLIB_MAJOR_VERSION}.${GLIB_MINOR_VERSION}" | tee -a ${OUTFILE}
if [ "${GLIB_MAJOR_VERSION}" -ge "2" ] ; then
    if [ "${GLIB_MINOR_VERSION}" -ge "32" ] ; then
        echo "// It is not necessary extra code " >> ${OUTFILE}
        echo "void g_thread_support()" >> ${OUTFILE}
        echo "{" >> ${OUTFILE}
        echo "    ;" >> ${OUTFILE}
        echo "}" >> ${OUTFILE}
    else   
        echo  "// Necessary for glib older than version 2.32" >> ${OUTFILE}
        echo  "void g_thread_support()" >> ${OUTFILE}
        echo  "{" >> ${OUTFILE}
        echo  "    if (!g_thread_supported ()) g_thread_init (NULL);" >> ${OUTFILE}
        echo  "}" >> ${OUTFILE}
    fi
fi
