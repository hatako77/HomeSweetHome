#ifndef DIALOG_CSS_H
#define DIALOG_CSS_H

#include <pgmspace.h>

const char DIALOG_CSS[] PROGMEM=R"rawliteral(

#dialogOverlay{
	position:fixed;
	inset:0;
	display:flex;
	align-items:center;
	justify-content:center;
	background:rgba(0,0,0,.55);
	backdrop-filter:blur(6px);
	opacity:0;
	visibility:hidden;
	transition:.2s;
	z-index:1000;
}

#dialogOverlay.show{
	opacity:1;
	visibility:visible;
}

.dialog{
	width:min(500px,calc(100vw - 30px));
	background:#111827;
	border:1px solid #334155;
	border-radius:18px;
	box-shadow:0 20px 60px rgba(0,0,0,.45);
	overflow:hidden;
	transform:scale(.95);
	transition:.2s;
}

#dialogOverlay.show .dialog{
	transform:scale(1);
}

.dialog-header{
	display:flex;
	align-items:center;
	justify-content:space-between;
	padding:18px 22px;
	background:#0f172a;
	border-bottom:1px solid #1f2937;
}

.dialog-title{
	font-size:1rem;
	font-weight:600;
	color:#f8fafc;
}

.dialog-close{
	width:36px;
	height:36px;
	display:flex;
	align-items:center;
	justify-content:center;
	border:none;
	border-radius:10px;
	background:transparent;
	color:#94a3b8;
	cursor:pointer;
	transition:.2s;
}

.dialog-close:hover{
	background:#1e293b;
	color:#fff;
}

.dialog-body{
	padding:22px;
	max-height:70vh;
	overflow:auto;
}

.dialog-footer{
	display:flex;
	justify-content:flex-end;
	gap:10px;
	padding:18px 22px;
	border-top:1px solid #1f2937;
	background:#0f172a;
}

@media(max-width:768px){

	.dialog{
		width:calc(100vw - 20px);
		border-radius:16px;
	}

	.dialog-header{
		padding:16px;
	}

	.dialog-body{
		padding:16px;
	}

	.dialog-footer{
		padding:16px;
		flex-direction:column;
	}

	.dialog-footer .btn{
		width:100%;
	}

}

)rawliteral";

#endif
