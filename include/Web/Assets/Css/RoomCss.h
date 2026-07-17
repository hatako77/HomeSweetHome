#ifndef ROOM_CSS_H
#define ROOM_CSS_H

#include <pgmspace.h>

const char ROOM_CSS[] PROGMEM=R"rawliteral(

#roomsContainer{
	display:grid;
	grid-template-columns:repeat(auto-fit,minmax(360px,1fr));
	gap:20px;
	align-items:start;
}
.room-actions{
    display:flex;
    align-items:center;
    gap:8px;
}

.icon-btn{
    width:32px;
    height:32px;
    border:none;
    border-radius:8px;
    cursor:pointer;
    display:flex;
    align-items:center;
    justify-content:center;
}

.icon-btn.danger{
    color:#e74c3c;
}

.room-card{
	background:#111827;
	border:1px solid #1f2937;
	border-radius:18px;
	overflow:hidden;
	transition:.25s;
	box-shadow:0 8px 24px rgba(0,0,0,.25);
}

.room-card:hover{
	transform:translateY(-4px);
	border-color:#38bdf8;
	box-shadow:0 12px 28px rgba(56,189,248,.18);
}

.room-header{
	display:flex;
	align-items:center;
	justify-content:space-between;
	padding:16px 18px;
	background:#0f172a;
	border-bottom:1px solid #1f2937;
}

.room-name{
	display:flex;
	align-items:center;
	gap:10px;
	font-size:17px;
	font-weight:600;
}

.room-count{
	padding:4px 10px;
	border-radius:999px;
	background:#1e293b;
	color:#94a3b8;
	font-size:12px;
	font-weight:600;
}

.channel-list{
	display:grid;
	grid-template-columns:repeat(auto-fill,minmax(130px,1fr));
	gap:12px;
	padding:14px;
	min-height:90px;
}

.channel{
	display:flex;
	align-items:center;
	justify-content:space-between;
	padding:14px;
	border-radius:14px;
	background:#0b1220;
	border:1px solid transparent;
	cursor:pointer;
	transition:.2s;
	user-select:none;
}

.channel:hover{
	transform:scale(1.02);
	border-color:#38bdf8;
}

.channel:active{
	transform:scale(.97);
}

.channel-left{
	display:flex;
	align-items:center;
	gap:14px;
}

.channel-right{
	display:flex;
	align-items:center;
	gap:10px;
}

.channel-icon{
	width:42px;
	height:42px;
	display:flex;
	align-items:center;
	justify-content:center;
	border-radius:12px;
	background:#1e293b;
}

.channel-icon svg{
	width:22px;
	height:22px;
	fill:#94a3b8;
}

.channel-name{
	font-size:15px;
	font-weight:500;
}

.channel-type{
	margin-top:4px;
	font-size:12px;
	color:#64748b;
}

.channel-state{
	width:14px;
	height:14px;
	border-radius:50%;
	background:#ef4444;
	transition:.25s;
}

.channel.on{
	background:linear-gradient(90deg,rgba(34,197,94,.12),#111827);
}

.channel.on .channel-icon{
	background:#14532d;
}

.channel.on .channel-icon svg{
	fill:#22c55e;
}

.channel.on .channel-state{
	background:#22c55e;
	box-shadow:0 0 14px #22c55e;
}

.dragging{
	opacity:.45;
	transform:rotate(3deg);
}

.channel-list.drag-over{
	outline:2px dashed #38bdf8;
	outline-offset:4px;
	border-radius:14px;
}

@media(max-width:768px){

	#roomsContainer{
		grid-template-columns:1fr;
		gap:16px;
	}

	.room-header{
		padding:14px;
	}

	.channel-list{
		grid-template-columns:repeat(2,minmax(0,1fr));
		gap:10px;
		padding:10px;
	}

}

)rawliteral";

#endif
