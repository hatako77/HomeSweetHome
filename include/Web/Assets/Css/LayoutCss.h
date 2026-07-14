#ifndef LAYOUT_CSS_H
#define LAYOUT_CSS_H

#include <pgmspace.h>

const char LAYOUT_CSS[] PROGMEM=R"rawliteral(

.topbar{
	display:flex;
	align-items:center;
	justify-content:space-between;
	height:60px;
	padding:0 20px;
	background:#0f172a;
	border-bottom:1px solid #1f2937;
}

.logo{
	font-size:18px;
	font-weight:700;
	color:#38bdf8;
	display:flex;
	align-items:center;
	gap:10px;
}

.status{
	display:flex;
	align-items:center;
	gap:6px;
	font-size:13px;
}

.status.online{
	color:#22c55e;
}

.container{
	width:100%;
	height:calc(100vh - 60px);
	margin:0;
	padding:0;
}

.layout{
	display:flex;
	width:100%;
	height:100%;
}

#sidebar{
	width:260px;
	flex-shrink:0;
	padding:20px;
	border-right:1px solid rgba(255,255,255,.08);
	background:#0f172a;
	overflow-y:auto;
}

#content{
	flex:1;
	min-width:0;
	padding:30px;
	overflow:auto;
}

@media(max-width:768px){

	#sidebar{
		width:70px;
		padding:10px 6px;
	}

	#content{
		padding:16px;
	}

}

)rawliteral";

#endif
