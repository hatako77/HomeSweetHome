#ifndef SIDEBAR_CSS_H
#define SIDEBAR_CSS_H

#include <pgmspace.h>

const char SIDEBAR_CSS[] PROGMEM=R"rawliteral(

.sidebar-menu{
	display:flex;
	flex-direction:column;
	gap:8px;
}

.menu-item{
	display:flex;
	align-items:center;
	gap:14px;
	width:100%;
	height:52px;
	padding:0 18px;
	border-radius:14px;
	background:#1f2535;
	color:#fff;
	cursor:pointer;
	transition:.2s;
	user-select:none;
}

.menu-item:hover{
	background:#2b3248;
	transform:translateX(3px);
}

.menu-item.active{
	background:#3d7bfd;
	box-shadow:0 6px 18px rgba(61,123,253,.35);
}

.menu-item i{
	width:20px;
	text-align:center;
	font-size:1rem;
	flex-shrink:0;
}

.menu-item span{
	white-space:nowrap;
	overflow:hidden;
	text-overflow:ellipsis;
}

@media(max-width:768px){

	.menu-item{
		width:56px;
		height:56px;
		padding:0;
		justify-content:center;
		gap:0;
	}

	.menu-item span{
		display:none;
	}
	.menu-item .svg-icon {
		display:block;
	}

	.menu-item i{
		width:auto;
		margin:0;
		font-size:1rem;
	}

}

)rawliteral";

#endif
