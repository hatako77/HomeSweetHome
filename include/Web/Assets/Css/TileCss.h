#ifndef TILE_CSS_H
#define TILE_CSS_H

#include <pgmspace.h>

const char TILE_CSS[] PROGMEM=R"rawliteral(

.tile{
	display:flex;
	flex-direction:column;
	align-items:center;
	justify-content:space-between;
	height:130px;
	padding:16px;
	border-radius:18px;
	background:#0b1220;
	border:1px solid transparent;
	cursor:pointer;
	transition:.25s;
	user-select:none;
}

.tile:hover{
	transform:translateY(-4px);
	border-color:#38bdf8;
}

.tile:active{
	transform:scale(.97);
}

.tile-icon{
	width:56px;
	height:56px;
	display:flex;
	align-items:center;
	justify-content:center;
	border-radius:16px;
	background:#1e293b;
	transition:.25s;
}

.tile-icon svg{
	width:30px;
	height:30px;
	fill:#94a3b8;
	transition:.25s;
}

.tile-name{
	margin-top:12px;
	font-size:14px;
	font-weight:600;
	text-align:center;
	word-break:break-word;
}

.tile-bottom{
	display:flex;
	align-items:center;
	gap:8px;
	margin-top:12px;
}

.tile-label{
	font-size:12px;
	color:#94a3b8;
	transition:.25s;
}

.tile.on{
	background:linear-gradient(180deg,#14532d,#0b1220);
	border-color:#22c55e55;
}

.tile.on .tile-icon{
	background:#22c55e22;
}

.tile.on .tile-icon svg{
	fill:#22c55e;
}

.tile.on .tile-label{
	color:#22c55e;
	font-weight:600;
}

.tile.off .tile-icon svg{
	fill:#94a3b8;
}

.tile.disabled{
	opacity:.45;
	pointer-events:none;
}

@media(max-width:768px){

	.tile{
		height:120px;
		padding:12px;
	}

	.tile-icon{
		width:48px;
		height:48px;
	}

	.tile-icon svg{
		width:26px;
		height:26px;
	}

	.tile-name{
		font-size:13px;
	}

}

)rawliteral";

#endif
