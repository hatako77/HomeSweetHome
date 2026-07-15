#ifndef RESPONSIVE_CSS_H
#define RESPONSIVE_CSS_H

#include <pgmspace.h>

const char RESPONSIVE_CSS[] PROGMEM=R"rawliteral(

@media(max-width:1200px){
	body{
		    height: 70%;

	}
	#roomsContainer{
		grid-template-columns:repeat(auto-fit,minmax(320px,1fr));
	}

	.dashboard-grid{
		grid-template-columns:repeat(auto-fill,minmax(200px,1fr));
	}

}

@media(max-width:992px){

	#content{
		padding:20px;
	}

	.room-card{
		border-radius:16px;
	}

	.card{
		padding:18px;
	}

}

@media(max-width:768px){

	.topbar{
		padding:0 14px;
	}

	.logo{
		font-size:16px;
	}

	.status{
		font-size:11px;
	}

	.layout{
		min-height:calc(100vh - 60px);
	}

	#sidebar{
		width:70px;
		padding:10px 6px;
	}

	#content{
		padding:16px;
	}

	.dashboard-grid{
		grid-template-columns:1fr;
		gap:16px;
	}

	#roomsContainer{
		grid-template-columns:1fr;
		gap:16px;
	}

	.channel-list{
		grid-template-columns:repeat(2,minmax(0,1fr));
	}

	.grid{
		grid-template-columns:1fr;
	}

}

@media(max-width:480px){

	.channel-list{
		grid-template-columns:1fr;
	}

	.tile{
		height:110px;
	}

	.tile-icon{
		width:44px;
		height:44px;
	}

	.tile-name{
		font-size:12px;
	}

	.btn{
		width:100%;
	}

}

)rawliteral";

#endif
