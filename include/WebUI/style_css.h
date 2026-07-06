#ifndef STYLE_CSS_H
#define STYLE_CSS_H

#include <pgmspace.h>

const char STYLE_CSS[] PROGMEM = R"rawliteral(

body {
  margin: 0;
  font-family: 'Segoe UI', Arial;
  background: #0b1220;
  color: #e5e7eb;
}

/* TOP BAR */
.topbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 14px 18px;
  background: #0f172a;
  border-bottom: 1px solid #1e293b;
  position: sticky;
  top: 0;
}

.logo {
  font-size: 18px;
  font-weight: bold;
  color: #38bdf8;
}

.status {
  font-size: 12px;
  display: flex;
  align-items: center;
  gap: 5px;
}

.status.online {
  color: #22c55e;
}

/* LAYOUT */
.container {
  padding: 15px;
  max-width: 900px;
  margin: auto;
}

/* CARDS */
.card {
  background: #111827;
  padding: 16px;
  border-radius: 14px;
  margin-bottom: 15px;
  border: 1px solid #1f2937;
  transition: 0.2s;
}

.card:hover {
  transform: translateY(-2px);
}

.glow {
  box-shadow: 0 0 15px rgba(56,189,248,0.08);
}

h2 {
  margin-top: 0;
  color: #f9fafb;
  font-size: 16px;
}

/* INFO */
.info {
  margin: 10px 0;
  font-size: 14px;
  color: #cbd5e1;
  display: flex;
  flex-direction: column;
  gap: 6px;
}

/* BUTTON */
.btn {
  display: inline-block;
  margin-top: 10px;
  padding: 10px 14px;
  background: linear-gradient(135deg, #38bdf8, #2563eb);
  color: white;
  border-radius: 10px;
  text-decoration: none;
  font-size: 13px;
  font-weight: bold;
}

/* GRID */
.grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(130px, 1fr));
  gap: 10px;
}

/* IO CARD */
.io-card {
  background: #111827;
  border: 1px solid #1f2937;
  border-radius: 12px;
  padding: 10px;
  text-align: center;
  transition: 0.2s;
}

.io-card:hover {
  transform: scale(1.05);
  border-color: #38bdf8;
}

.io-title {
  font-size: 12px;
  margin-bottom: 8px;
  color: #cbd5e1;
}

.io-btn {
  padding: 6px 10px;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  font-size: 12px;
  color: white;
  background: #22c55e;
}

.io-off {
  background: #ef4444 !important;
}

.section-title {
  margin: 18px 0 10px;
  font-size: 15px;
  color: #f1f5f9;
}

)rawliteral";

#endif
