#ifndef BUTTON_CSS_H
#define BUTTON_CSS_H

#include <pgmspace.h>

const char BUTTON_CSS[] PROGMEM=R"rawliteral(

.btn{
	display:inline-flex;
	align-items:center;
	justify-content:center;
	gap:8px;
	padding:10px 18px;
	border:none;
	border-radius:12px;
	background:#2563eb;
	color:#fff;
	font-size:14px;
	font-weight:600;
	cursor:pointer;
	transition:.2s;
	user-select:none;
}


.btn:active{
	transform:scale(.98);
}

.btn:disabled{
	opacity:.5;
	cursor:not-allowed;
	box-shadow:none;
	transform:none;
}

.btn i{
	font-size:14px;
}

.btn-success{
	background:#16a34a;
}

.btn-danger{
	background:#dc2626;
}

.btn-warning{
	background:#d97706;
}


.btn-secondary{
	background:#374151;
}


.btn-outline{
	background:transparent;
	border:1px solid #475569;
	color:#e5e7eb;
}


.btn-block{
	width:100%;
}

)rawliteral";

#endif
