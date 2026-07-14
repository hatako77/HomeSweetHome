#ifndef LOADER_CSS_H
#define LOADER_CSS_H

#include <pgmspace.h>

const char LOADER_CSS[] PROGMEM=R"rawliteral(

#loader{
	position:fixed;
	inset:0;
	display:none;
	align-items:center;
	justify-content:center;
	background:rgba(0,0,0,.45);
	backdrop-filter:blur(4px);
	z-index:5000;
}

.loader-box{
	display:flex;
	flex-direction:column;
	align-items:center;
	gap:18px;
	padding:30px;
	min-width:220px;
	background:#111827;
	border:1px solid #334155;
	border-radius:18px;
	box-shadow:0 20px 50px rgba(0,0,0,.45);
}

.loader-spinner{
	width:48px;
	height:48px;
	border:4px solid #334155;
	border-top-color:#38bdf8;
	border-radius:50%;
	animation:loader-spin .8s linear infinite;
}

#loaderText{
	font-size:15px;
	font-weight:500;
	color:#e5e7eb;
	text-align:center;
}

@keyframes loader-spin{

	from{
		transform:rotate(0deg);
	}

	to{
		transform:rotate(360deg);
	}

}

@media(max-width:768px){

	.loader-box{
		width:220px;
		padding:24px;
		border-radius:16px;
	}

	.loader-spinner{
		width:42px;
		height:42px;
	}

	#loaderText{
		font-size:14px;
	}

}

)rawliteral";

#endif
