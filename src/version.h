#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR 2
#define VERSION_MINOR 1
#define VERSION_PATCH 21

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define VERSION_STRING TOSTRING(VERSION_MAJOR) "." \
                       TOSTRING(VERSION_MINOR) "." \
                       TOSTRING(VERSION_PATCH)

extern const char* FIRMWARE_VERSION;
extern const char* BUILD_DATE;
extern const char* BUILD_TIME;

#endif
