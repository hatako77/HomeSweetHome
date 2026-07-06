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

<header class="topbar">
  <div class="logo">🏠 HomeSweetHome</div>
  <div class="status" id="status">● Online</div>
</header>

<main class="container">

<!-- System Card -->
<section class="card glass">
  <h2>System Status</h2>

  <div class="row">
    <span>Device:</span>
    <span>Running</span>
  </div>

  <div class="row">
    <span>Firmware:</span>
    <span id="version">...</span>
  </div>

  <a class="btn" href="/ota">Firmware Update</a>
</section>

<!-- Devices -->
<section>
  <h2 class="section-title">Devices</h2>
  <div id="ioContainer" class="grid"></div>
</section>

</main>

<script src="/app.js"></script>

</body>
</html>
)rawliteral";

#endif
