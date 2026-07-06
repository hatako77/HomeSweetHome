#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<meta charset="UTF-8">

<meta name="viewport" content="width=device-width, initial-scale=1">

<title>HomeSweetHome</title>

<link rel="stylesheet" href="/style.css">

</head>

<body>

<div class="container">

<h1>🏠 HomeSweetHome</h1>

<div class="card">

<h2>System</h2>

<p>Device is running</p>

<p id="version">Version: ...</p>

<p>
<a href="/ota">Firmware Update</a>
</p>

</div>

<h2>Devices</h2>

<div id="ioContainer"></div>

</div>

<script src="/app.js"></script>

</body>

</html>

)rawliteral";

#endif
