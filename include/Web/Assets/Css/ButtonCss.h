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

.btn:hover{
	background:#1d4ed8;
	transform:translateY(-2px);
	box-shadow:0 6px 18px rgba(37,99,235,.35);
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

.btn-success:hover{
	background:#15803d;
	box-shadow:0 6px 18px rgba(22,163,74,.35);
}

.btn-danger{
	background:#dc2626;
}

.btn-danger:hover{
	background:#b91c1c;
	box-shadow:0 6px 18px rgba(220,38,38,.35);
}

.btn-warning{
	background:#d97706;
}

.btn-warning:hover{
	background:#b45309;
	box-shadow:0 6px 18px rgba(217,119,6,.35);
}

.btn-secondary{
	background:#374151;
}

.btn-secondary:hover{
	background:#4b5563;
	box-shadow:0 6px 18px rgba(75,85,99,.35);
}

.btn-outline{
	background:transparent;
	border:1px solid #475569;
	color:#e5e7eb;
}

.btn-outline:hover{
	background:#1e293b;
	border-color:#38bdf8;
}

.btn-block{
	width:100%;
}

)rawliteral";

#endif
