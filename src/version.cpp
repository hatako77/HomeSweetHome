#include <Arduino.h>
#include "version.h"
#include "../include/version.h"

String getVersion()
{
    return FW_VERSION;
}

String getFirmwareURL()
{
    return String("https://github.com/")
            + GITHUB_OWNER
            + "/"
            + GITHUB_REPO
            + "/releases/latest/download/firmware.bin";
}

String getVersionURL()
{
    return String("https://github.com/")
            + GITHUB_OWNER
            + "/"
            + GITHUB_REPO
            + "/releases/latest/download/version.json";
}
