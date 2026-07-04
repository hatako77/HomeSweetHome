#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>HomeSweetHome</title>
  <link rel="stylesheet" href="/style.css">
</head>
<body>

  <div class="container">
    <h1>🏠 HomeSweetHome</h1>

    <div class="card">
      <h2>Status</h2>
      <p>Device is running...</p>
      <p id="version"></p>
    </div>

    <div class="card">
      <h2>Controls</h2>
      <button onclick="toggleRelay(1)">Relay 1</button>
      <button onclick="toggleRelay(2)">Relay 2</button>
    </div>

  </div>

<script src="/app.js"></script>

</body>
</html>
)rawliteral";

#endif
