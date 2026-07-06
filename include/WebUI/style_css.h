#ifndef STYLE_CSS_H
#define STYLE_CSS_H

#include <pgmspace.h>

const char STYLE_CSS[] PROGMEM = R"rawliteral(

body {
  margin: 0;
  font-family: Arial, sans-serif;
  background: #0f1115;
  color: #eaeaea;
}

/* Top Bar */
.topbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 15px 20px;
  background: #151922;
  position: sticky;
  top: 0;
  border-bottom: 1px solid #222;
}

.logo {
  font-size: 18px;
  font-weight: bold;
  color: #4CAF50;
}

.status {
  font-size: 12px;
  color: #4CAF50;
}

/* Layout */
.container {
  padding: 15px;
  max-width: 900px;
  margin: auto;
}

/* Cards */
.card {
  background: #1a1f2b;
  padding: 15px;
  border-radius: 12px;
  margin-bottom: 15px;
  border: 1px solid #222;
}

.glass {
  background: rgba(26, 31, 43, 0.8);
  backdrop-filter: blur(10px);
}

/* Text */
h2 {
  margin-top: 0;
  color: #ffffff;
}

.section-title {
  margin: 20px 0 10px;
}

/* Rows */
.row {
  display: flex;
  justify-content: space-between;
  padding: 6px 0;
  font-size: 14px;
  color: #bbb;
}

/* Button */
.btn {
  display: inline-block;
  margin-top: 10px;
  padding: 10px 14px;
  background: #4CAF50;
  color: white;
  text-decoration: none;
  border-radius: 8px;
  text-align: center;
  font-size: 14px;
}

/* IO Grid */
.grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(120px, 1fr));
  gap: 10px;
}

/* IO Card (for JS devices) */
.io-card {
  background: #1a1f2b;
  padding: 10px;
  border-radius: 10px;
  text-align: center;
  border: 1px solid #222;
  transition: 0.2s;
}

.io-card:hover {
  transform: scale(1.03);
}

.io-btn {
  margin-top: 8px;
  padding: 6px 10px;
  border: none;
  border-radius: 6px;
  cursor: pointer;
  background: #4CAF50;
  color: white;
  font-size: 12px;
}

.io-off {
  background: #d9534f !important;
}

)rawliteral";

#endif
