#pragma once

#include <Arduino.h>

const char STYLE_CSS[] PROGMEM = R"rawliteral(

*{
    margin:0;
    padding:0;
    box-sizing:border-box;
}

body{

    background:#111827;

    color:#f3f4f6;

    font-family:Arial,Helvetica,sans-serif;

    display:flex;

    justify-content:center;

    align-items:center;

    min-height:100vh;

}

.container{

    width:95%;

    max-width:700px;

}

.card{

    background:#1f2937;

    border-radius:16px;

    padding:24px;

    margin-bottom:20px;

    box-shadow:0 0 20px rgba(0,0,0,.3);

}

h1{

    text-align:center;

    margin-bottom:20px;

    color:#60a5fa;

}

h2{

    margin-bottom:15px;

}

.row{

    display:flex;

    justify-content:space-between;

    margin:12px 0;

}

.label{

    color:#9ca3af;

}

.value{

    color:white;

}

button{

    width:100%;

    padding:14px;

    border:none;

    border-radius:10px;

    cursor:pointer;

    font-size:16px;

    margin-top:15px;

    transition:.25s;

}

.primary{

    background:#2563eb;

    color:white;

}

.primary:hover{

    background:#1d4ed8;

}

.success{

    background:#059669;

    color:white;

}

.success:hover{

    background:#047857;

}

.progress{

    width:100%;

    height:24px;

    background:#374151;

    border-radius:30px;

    overflow:hidden;

    margin-top:20px;

}

.progress-bar{

    height:100%;

    width:0%;

    background:#22c55e;

    transition:.2s;

}

.status{

    margin-top:15px;

    text-align:center;

}

footer{

    text-align:center;

    margin-top:30px;

    color:#9ca3af;

    font-size:13px;

}

a{

    color:#60a5fa;

    text-decoration:none;

}

a:hover{

    text-decoration:underline;

}

)rawliteral";
