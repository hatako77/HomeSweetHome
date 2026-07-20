#ifndef BASE_CSS_H
#define BASE_CSS_H

#include <pgmspace.h>

const char BASE_CSS[] PROGMEM=R"rawliteral(

*{
	box-sizing:border-box;
	margin:0;
	padding:0;
}

html{
	height:100%;
}

body{
	min-height:100%;
	font-family:'Segoe UI',Arial,sans-serif;
	background:#0b1220;
	color:#e5e7eb;
	overflow:hidden;
}
.svg-icon{
display:inline-flex;
align-items:center;
justify-content:center;
width:22px;
height:22px;
color:inherit;
flex-shrink:0;
}

.svg-icon svg{
width:100%;
height:100%;
fill:none;
stroke:currentColor;
stroke-width:2;
stroke-linecap:round;
stroke-linejoin:round;
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

)rawliteral";

#endif
