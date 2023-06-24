#ifndef _CCONF_H
#define _CCONF_H
// cconf.h.in is processed by cmake to generate cconf.h
// Replaces autotools' config.h.in + config.h combo.

//@varname@ or ${varname} will be replaced by the contents of the cmake variable varname
//# cmakedefine generates the appropriate #define as required

//#cmakedefine HAVE_LIBREADLINE

//#cmakedefine HAVE_GETTIMEOFDAY
//#cmakedefine HAVE_STRCASECMP
//#cmakedefine HAVE_MS_KBHIT
//#cmakedefine HAVE_MS_GETCH
//#cmakedefine HAVE_ALARM
//#cmakedefine HAVE_SELECT

//#cmakedefine USE_RCFILE
//#cmakedefine USE_INIFILE

#define PACKAGE_VERSION "@PKGVERSION@"
#define SCANTOOL_PROGNAME "@SCANTOOL_PROGNAME@"
#define DIAG_TEST_PROGNAME "@DIAG_TEST_PROGNAME@"
#define PROJECT_NAME "@PROJECT_NAME@"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define PACKAGE_VERSION_VALUE ${PKGVERSIONMAJOR_VALUE},${PKGVERSIONMINOR_VALUE},0,0
    #define PACKAGE_VERSION_STR "@PKGVERSION@\0"
    #define PROJECT_NAME_STR "@PROJECT_NAME@\0" 
    #define SCANTOOL_PROGNAME_EXE "@SCANTOOL_PROGNAME@.exe\0"
    #define DIAG_TEST_PROGNAME_EXE "@DIAG_TEST_PROGNAME@.exe\0"
#endif //Windows

#endif //ifndef _cconf_h
