#pragma once

#include <Arduino.h>

class GitHub
{
public:

    GitHub();

    bool begin();

    bool check();

    String latestVersion() const;

    String firmwareURL() const;

    String changelogURL() const;

    String versionURL() const;

    String lastError() const;

private:

    String _latestVersion;

    String _lastError;

    bool downloadVersionFile(String &json);

};
