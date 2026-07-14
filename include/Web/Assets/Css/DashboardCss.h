#ifndef DASHBOARD_CSS_H
#define DASHBOARD_CSS_H

#include <pgmspace.h>

const char DASHBOARD_CSS[] PROGMEM=R"rawliteral(

.dashboard-grid{
	display:grid;
	grid-template-columns:repeat(auto-fill,minmax(220px,1fr));
	gap:20px;
	margin-top:24px;
}

.dashboard-card{
	background:#111827;
	border:1px solid #1f2937;
	border-radius:18px;
	padding:24px;
	min-height:120px;
	transition:.25s;
	box-shadow:0 8px 24px rgba(0,0,0,.25);
}

.dashboard-card:hover{
	transform:translateY(-4px);
	border-color:#38bdf8;
	box-shadow:0 12px 28px rgba(56,189,248,.18);
}

.dashboard-card h3{
	margin:0;
	font-size:15px;
	font-weight:500;
	color:#94a3b8;
}

.dashboard-card .value{
	margin-top:18px;
	font-size:34px;
	font-weight:700;
	color:#fff;
}

.dashboard-card .icon{
	width:52px;
	height:52px;
	display:flex;
	align-items:center;
	justify-content:center;
	border-radius:14px;
	background:#1e293b;
	margin-bottom:16px;
	font-size:22px;
	color:#38bdf8;
}

.dashboard-card.primary{
	border-color:#2563eb;
}

.dashboard-card.success{
	border-color:#22c55e;
}

.dashboard-card.warning{
	border-color:#f59e0b;
}

.dashboard-card.danger{
	border-color:#ef4444;
}

@media(max-width:768px){

	.dashboard-grid{
		grid-template-columns:1fr;
		gap:16px;
	}

	.dashboard-card{
		padding:18px;
	}

	.dashboard-card .value{
		font-size:28px;
	}

}

)rawliteral";

#endif
