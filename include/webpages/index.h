#pragma once

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>

<meta charset="UTF-8">

<meta name="viewport" content="width=device-width,initial-scale=1">

<title>Home Sweet Home</title>

<link rel="stylesheet" href="/app.css">

</head>

<body>

<div class="container">

<div class="card">

<h1 id="fwName">Home Sweet Home</h1>

<div class="row">
<div class="label">Firmware Version</div>
<div class="value" id="fwVersion">...</div>
</div>

<div class="row">
<div class="label">WiFi Status</div>
<div class="value">Connected</div>
</div>

<div class="row">
<div class="label">Device</div>
<div class="value">ESP32</div>
</div>

<button
class="primary"
onclick="location.href='/ota';">

Firmware Update

</button>

</div>

<footer>

Home Sweet Home © 2026

</footer>

</div>

<script src="/app.js"></script>

</body>

</html>
)rawliteral";
