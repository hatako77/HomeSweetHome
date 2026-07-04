#ifndef STYLE_CSS_H
#define STYLE_CSS_H

#include <pgmspace.h>

const char STYLE_CSS[] PROGMEM = R"rawliteral(
body {
  font-family: Arial;
  background: #111;
  color: #fff;
  margin: 0;
  padding: 0;
}

.container {
  padding: 20px;
}

h1 {
  color: #4CAF50;
}

.card {
  background: #222;
  padding: 15px;
  margin: 10px 0;
  border-radius: 10px;
}

button {
  padding: 10px;
  margin: 5px;
  border: none;
  border-radius: 5px;
  background: #4CAF50;
  color: white;
}
)rawliteral";

#endif
