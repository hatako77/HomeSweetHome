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
/* ---------- ROOM ---------- */

.room-card{
    background:#111827;
    border:1px solid #1f2937;
    border-radius:16px;
    margin-bottom:18px;
    overflow:hidden;
}

.room-header{
    display:flex;
    justify-content:space-between;
    align-items:center;
    padding:14px 18px;
    background:#0f172a;
    border-bottom:1px solid #1f2937;
}

.room-name{
    font-size:17px;
    font-weight:600;
}

.room-count{
    font-size:12px;
    color:#94a3b8;
}

/* ---------- CHANNEL LIST ---------- */

.channel-list{
    padding:12px;
    min-height:50px;
}

/* ---------- CHANNEL ---------- */

.channel{
    display:flex;
    justify-content:space-between;
    align-items:center;

    padding:12px;
    margin-bottom:10px;

    border-radius:12px;

    background:#1e293b;

    cursor:pointer;

    transition:.2s;
}

.channel:hover{
    transform:translateX(3px);
}

.channel-left{
    display:flex;
    align-items:center;
    gap:12px;
}

.channel-icon{
    width:38px;
    height:38px;

    border-radius:10px;

    display:flex;
    align-items:center;
    justify-content:center;

    background:#334155;
}

.channel-name{
    font-size:14px;
}

.channel-state{

    width:14px;
    height:14px;

    border-radius:50%;

    background:#ef4444;
}

.channel.on .channel-state{

    background:#22c55e;

    box-shadow:0 0 12px #22c55e;
}

.dragging{

    opacity:.5;

}

.channel-list.drag-over{

    outline:2px dashed #38bdf8;

    border-radius:12px;

}
/* ===========================
   ROOM GRID
=========================== */

#roomsContainer{

display:grid;

grid-template-columns:repeat(auto-fit,minmax(360px,1fr));

gap:20px;

align-items:start;

}


/* ===========================
   ROOM CARD
=========================== */

.room-card{

background:#111827;

border:1px solid #1f2937;

border-radius:18px;

overflow:hidden;

transition:.25s;

box-shadow:0 8px 24px rgba(0,0,0,.25);

}

.room-card:hover{

transform:translateY(-4px);

border-color:#38bdf8;

box-shadow:0 12px 28px rgba(56,189,248,.18);

}

.room-header{

display:flex;

justify-content:space-between;

align-items:center;

padding:16px 18px;

background:#0f172a;

border-bottom:1px solid #1f2937;

}

.room-name{

display:flex;

align-items:center;

gap:10px;

font-size:17px;

font-weight:600;

}

.room-count{

font-size:12px;

padding:4px 10px;

border-radius:30px;

background:#1e293b;

color:#94a3b8;

}


/* ===========================
   CHANNEL LIST
=========================== */

.channel-list{

padding:12px;

display:flex;

flex-direction:column;

gap:10px;

min-height:80px;

}


/* ===========================
   CHANNEL CARD
=========================== */

.channel{

display:flex;

justify-content:space-between;

align-items:center;

padding:14px;

border-radius:14px;

background:#0b1220;

cursor:pointer;

transition:.2s;

border:1px solid transparent;

}

.channel:hover{

transform:scale(1.02);

border-color:#38bdf8;

}

.channel-left{

display:flex;

align-items:center;

gap:14px;

}

.channel-icon{

width:42px;

height:42px;

border-radius:12px;

background:#1e293b;

display:flex;

align-items:center;

justify-content:center;

}

.channel-icon svg{

width:22px;

height:22px;

fill:#94a3b8;

}

.channel-name{

font-size:15px;

font-weight:500;

}

.channel-state{

width:14px;

height:14px;

border-radius:50%;

background:#ef4444;

transition:.25s;

}

.channel.on .channel-state{

background:#22c55e;

box-shadow:0 0 14px #22c55e;

}

.dragging{

opacity:.4;

transform:rotate(3deg);

}
.channel-right{
    display:flex;
    align-items:center;
    gap:10px;
}

.channel-type{
    font-size:12px;
    color:#64748b;
    margin-top:4px;
}

.channel.on{

    background:linear-gradient(
        90deg,
        rgba(34,197,94,.12),
        rgba(17,24,39,1)
    );

}

.channel.on .channel-icon{

    background:#14532d;

}

.channel.on .channel-icon svg{

    fill:#22c55e;

}

.channel{

    user-select:none;

}

.channel:active{

    transform:scale(.97);

}

)rawliteral";

#endif
