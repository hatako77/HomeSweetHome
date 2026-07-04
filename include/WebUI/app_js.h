#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>

const char APP_JS[] PROGMEM = R"rawliteral(

function toggleRelay(id) {
    fetch("/relay?ch=" + id)
      .then(res => res.text())
      .then(data => console.log(data));
}

function loadVersion() {
    fetch("/version")
      .then(res => res.json())
      .then(data => {
          document.getElementById("version").innerText =
            "Version: " + data.version + " (build " + data.build + ")";
      });
}

loadVersion();

)rawliteral";

#endif
