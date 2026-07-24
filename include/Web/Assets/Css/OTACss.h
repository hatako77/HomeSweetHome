#ifndef OTA_CSS_H
#define OTA_CSS_H

#include <pgmspace.h>

const char OTA_CSS[] PROGMEM=R"rawliteral(

.ota-card{
	background:#111827;
	border:1px solid #1f2937;
	border-radius:18px;
	padding:24px;
	box-shadow:0 8px 24px rgba(0,0,0,.25);
}

.ota-header{
	display:flex;
	align-items:center;
	justify-content:space-between;
	margin-bottom:20px;
	padding-bottom:14px;
	border-bottom:1px solid #1f2937;
}

.ota-title{
	font-size:1rem;
	font-weight:600;
	color:#f8fafc;
}

.ota-version{
	font-size:1rem;
	color:#94a3b8;
}

.ota-info{
	display:grid;
	grid-template-columns:repeat(auto-fit,minmax(180px,1fr));
	gap:16px;
	margin:20px 0;
}

.ota-item{
	background:#0f172a;
	border:1px solid #1f2937;
	border-radius:14px;
	padding:14px;
}

.ota-item-label{
	font-size:0.8rem;
	color:#94a3b8;
	margin-bottom:8px;
}

.ota-item-value{
	font-size:1rem;
	font-weight:600;
	color:#f8fafc;
}

.ota-progress{
	width:100%;
	height:14px;
	margin:20px 0;
	border:none;
	border-radius:999px;
	overflow:hidden;
	background:#1e293b;
}

.ota-progress::-webkit-progress-bar{
	background:#1e293b;
}

.ota-progress::-webkit-progress-value{
	background:#22c55e;
	border-radius:999px;
}

.ota-progress::-moz-progress-bar{
	background:#22c55e;
	border-radius:999px;
}

.ota-actions{
	display:flex;
	gap:12px;
	margin-top:24px;
}

.ota-status{
	margin-top:18px;
	padding:12px 16px;
	border-radius:12px;
	background:#0f172a;
	color:#cbd5e1;
	font-size:0.8rem;
}

@media(max-width:768px){

	.ota-card{
		padding:18px;
	}

	.ota-info{
		grid-template-columns:1fr;
		gap:12px;
	}

	.ota-actions{
		flex-direction:column;
	}

	.ota-actions .btn{
		width:100%;
	}

}

)rawliteral";

#endif
