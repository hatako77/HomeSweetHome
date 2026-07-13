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

<div class="layout">

    <aside id="sidebar"></aside>

    <section id="content"></section>

</div>
</main>

<script src="/app.js"></script>

</body>
</html>

)rawliteral";

#endif
