#ifndef CARD_CSS_H
#define CARD_CSS_H

#include <pgmspace.h>

const char CARD_CSS[] PROGMEM=R"rawliteral(

.card{
	background:#111827;
	border:1px solid #1f2937;
	border-radius:16px;
	padding:20px;
	margin-bottom:20px;
	box-shadow:0 8px 24px rgba(0,0,0,.25);
	transition:.25s;
}

.card:hover{
	border-color:#38bdf8;
	box-shadow:0 12px 30px rgba(56,189,248,.18);
}

.card-title{
	font-size:18px;
	font-weight:600;
	margin-bottom:16px;
	color:#f8fafc;
}

.card-subtitle{
	font-size:13px;
	color:#94a3b8;
	margin-bottom:18px;
}

.card-header{
	display:flex;
	align-items:center;
	justify-content:space-between;
	margin-bottom:18px;
	padding-bottom:12px;
	border-bottom:1px solid #1f2937;
}

.card-body{
	display:flex;
	flex-direction:column;
	gap:16px;
}

.card-footer{
	display:flex;
	justify-content:flex-end;
	gap:10px;
	margin-top:20px;
	padding-top:16px;
	border-top:1px solid #1f2937;
}

.info{
	display:flex;
	flex-direction:column;
	gap:10px;
}

.info-row{
	display:flex;
	justify-content:space-between;
	align-items:center;
	padding:8px 0;
	border-bottom:1px solid rgba(255,255,255,.05);
}

.info-row:last-child{
	border-bottom:none;
}

.info-label{
	color:#94a3b8;
	font-size:13px;
}

.info-value{
	font-weight:600;
	color:#f8fafc;
}

.grid{
	display:grid;
	grid-template-columns:repeat(auto-fill,minmax(220px,1fr));
	gap:20px;
}

@media(max-width:768px){

	.card{
		padding:16px;
		border-radius:14px;
	}

	.grid{
		grid-template-columns:1fr;
		gap:16px;
	}

	.card-footer{
		flex-direction:column;
	}

	.card-footer .btn{
		width:100%;
	}

}

)rawliteral";

#endif
