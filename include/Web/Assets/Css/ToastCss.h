#ifndef TOAST_CSS_H
#define TOAST_CSS_H

#include <pgmspace.h>

const char TOAST_CSS[] PROGMEM=R"rawliteral(

#toastContainer{
	position:fixed;
	top:20px;
	right:20px;
	display:flex;
	flex-direction:column;
	gap:12px;
	z-index:3000;
	pointer-events:none;
}

.toast{
	min-width:280px;
	max-width:420px;
	padding:14px 18px;
	border-radius:14px;
	background:#1e293b;
	color:#fff;
	border-left:5px solid #38bdf8;
	box-shadow:0 12px 28px rgba(0,0,0,.35);
	transform:translateX(120%);
	opacity:0;
	transition:.25s;
	pointer-events:auto;
}

.toast.show{
	transform:translateX(0);
	opacity:1;
}

.toast-message{
	font-size:14px;
	line-height:1.5;
}

.toast.success{
	border-left-color:#22c55e;
}

.toast.error{
	border-left-color:#ef4444;
}

.toast.warning{
	border-left-color:#f59e0b;
}

.toast.info{
	border-left-color:#38bdf8;
}

@media(max-width:768px){

	#toastContainer{
		left:10px;
		right:10px;
		top:10px;
	}

	.toast{
		min-width:auto;
		max-width:none;
		width:100%;
	}

}

)rawliteral";

#endif
