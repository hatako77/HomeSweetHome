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
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.0/css/all.min.css">

</head>
<script src="https://cdn.jsdelivr.net/npm/sortablejs@1.15.2/Sortable.min.js"></script>
<script src="/app.js"></script>
<body>

<!-- TOP BAR -->
<header class="topbar">
  <div class="logo">
    <i class="fa-solid fa-house"></i> HomeSweetHome
  </div>

  <div class="status online">
    <i class="fa-solid fa-circle"></i> Online
  </div>
</header>

<main class="container">

<!-- SYSTEM CARD -->
<section class="card glow">
  <h2><i class="fa-solid fa-microchip"></i> System</h2>

  <div class="info">
    <div><i class="fa-solid fa-bolt"></i> Device: Running</div>
  </div>

  <a class="btn" href="/ota">
    <i class="fa-solid fa-upload"></i> Firmware Update
  </a>
</section>

<!-- IO DEVICES -->
<!-- ROOMS -->
<section>

    <h2 class="section-title">
        <i class="fa-solid fa-house"></i>
        Rooms
    </h2>

    <div id="roomsContainer"></div>

</section>
</main>

<script src="/app.js"></script>

</body>
</html>

)rawliteral";

#endif
