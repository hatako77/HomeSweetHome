#pragma once

#include <Arduino.h>

const char OTA_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>

<meta charset="UTF-8">

<meta name="viewport" content="width=device-width,initial-scale=1">

<title>Firmware Update</title>

<link rel="stylesheet" href="/app.css">

</head>

<body>

<div class="container">

<div class="card">

<h1>Firmware Update</h1>

<div class="row">
<div class="label">Current Version</div>
<div class="value" id="fwVersion">...</div>
</div>

<div class="row">
<div class="label">Latest Version</div>
<div class="value" id="latestVersion">Unknown</div>
</div>

<button
class="primary"
onclick="checkUpdate();">

Check Update

</button>

<button
class="success"
id="btnUpdate"
onclick="startUpdate();"
disabled>

Install Update

</button>

<div class="progress">

<div
id="progressBar"
class="progress-bar">

</div>

</div>

<div
id="progressText"
class="status">

0%

</div>

<div
id="status"
class="status">

Ready

</div>

</div>

<div class="card">

<button
class="primary"
onclick="location.href='/';">

Back

</button>

</div>

<footer>

Home Sweet Home OTA

</footer>

</div>

<script src="/app.js"></script>

</body>

</html>
)rawliteral";
