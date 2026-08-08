#ifndef BASE_CSS_H
#define BASE_CSS_H

#include <pgmspace.h>

const char BASE_CSS[] PROGMEM=R"rawliteral(



.running-scenes
{
    display:flex;
    align-items:center;
    gap:8px;
}
.running-scene
{
    width:42px;
    display:flex;
    flex-direction:column;
    align-items:center;
}
.running-scene-icon
{
	fill: white;
    width:34px;
    height:34px;

    border-radius:8px;

    background:#2d2d2d;

    display:flex;
    align-items:center;
    justify-content:center;

    color:#fff;
}
.running-scene-progress
{
    width:100%;
    height:3px;

    margin-top:3px;

    background:#444;
    border-radius:3px;

    overflow:hidden;
}
.running-scene-fill
{
    height:100%;
    width:0%;

    background:#4caf50;

    transition:width .4s linear;
}
.scene-progress
{
    width:100%;
    height:1.2rem;

    margin-top:8px;

    background:#2d2d2d;

    border-radius:1rem;

    overflow:hidden;

    display:none;
}

.scene-progress-bar
{
    width:0%;

    height:100%;

    background:#36d64d;

    transition:width .4s linear;
}
*{
	box-sizing:border-box;
	margin:0;
	padding:0;
}
.icon-picker{
    display:grid;
    grid-template-columns:repeat(4,1fr);
    gap:10px;
}

.icon-item{
    display:flex;
    justify-content:center;
    align-items:center;
    height:56px;
    border:1px solid var(--border);
    border-radius:12px;
    cursor:pointer;
    transition:.2s;
}

.icon-item:hover{
    background:var(--hover);
}

.icon-item.selected{
    border:2px solid var(--primary);
    background:rgba(59,130,246,.15);
}

html{
    height:100%;
	direction:rtl;
	font-size:16px;
	font-family: "Vazirmatn", sans-serif;
}

body{
    min-height:100vh;
    margin:0;
    background:#0b1220;
    color:#e5e7eb;
    overflow-y:auto;
    overflow-x:hidden;
}


.svg-icon{
display:inline-flex;
align-items:center;
justify-content:center;

color:inherit;
flex-shrink:0;
}

.svg-icon svg{
    width: 100%;
    height: 100%;
}

.channel-icon{
display:flex;
align-items:center;
justify-content:center;
width:42px;
height:42px;
border-radius:12px;
background:var(--surface-2);
color:var(--primary);
}
a{
	color:inherit;
	text-decoration:none;
}

button{
	font:inherit;
	border:none;
	outline:none;
	cursor:pointer;
}

input,
select,
textarea{
	font:inherit;
	outline:none;
}

h1,
h2,
h3,
h4,
h5,
h6{
	font-weight:600;
}
.scene-info
{
    display:flex;
    flex-direction:column;
    gap:8px;
}

.scene-progress
{
    display:flex;
    align-items:center;
    gap:8px;
}

.scene-progress-track
{
    flex:1;
    height:8px;

    background:#2c2c2c;

    border-radius:999px;

    overflow:hidden;
}

.scene-progress-bar
{
    height:100%;

    background:#22c55e;

    width:0;

    transition:width .35s;
}

.status.running
{
    color:#22c55e;

    font-weight:600;
}

.status.running i
{
    font-size:8px;

    margin-right:6px;
}

.status.idle
{
    color:#888;
}

)rawliteral";

#endif
