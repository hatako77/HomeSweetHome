#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <pgmspace.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<link rel="stylesheet" href="/style.css">

</head>
<body>

<header class="topbar">
    ...
</header>

<div class="layout">

    <aside id="sidebar"></aside>

    <main id="content">
    </main>

</div>

<script src="/js/state.js"></script>
<script src="/js/router.js"></script>
<script src="/js/app.js"></script>

<script src="/js/utils/dom.js"></script>
<script src="/js/utils/api.js"></script>
<script src="/js/utils/icons.js"></script>
<script src="/js/utils/websocket.js"></script>
<script src="/js/utils/svgicons.js"></script>

<script src="/js/components/sidebar.js"></script>
<script src="/js/components/dialog.js"></script>
<script src="/js/components/modal.js"></script>
<script src="/js/components/toast.js"></script>
<script src="/js/components/loader.js"></script>

<script src="/js/dashboard.js"></script>
<script src="/js/room.js"></script>
<script src="/js/channel.js"></script>
<script src="/js/scene.js"></script>
<script src="/js/schedule.js"></script>
<script src="/js/sensor.js"></script>
<script src="/js/ota.js"></script>
<script src="/js/settings.js"></script>
</body>
</html>

)rawliteral";

#endif
