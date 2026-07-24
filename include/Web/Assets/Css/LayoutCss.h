#ifndef LAYOUT_CSS_H
#define LAYOUT_CSS_H

#include <pgmspace.h>

const char LAYOUT_CSS[] PROGMEM=R"rawliteral(

.topbar{
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    z-index: 1000;

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
    margin-top:60px;   /* ارتفاع TopBar */
}

.layout{
    display:flex;
    align-items:flex-start;
}

#sidebar{
    position:fixed;

    top:60px;
    left:0;

    width:260px;
    height:calc(100vh - 60px);

    overflow-y:auto;

    padding:20px;

    background:#0f172a;
    border-right:1px solid rgba(255,255,255,.08);

    z-index:999;
}

#content{
    flex:1;
	margin-top: 60px;
    margin-left:260px;

    padding:30px;

    min-width:0;
}

@media(max-width:768px){

    #sidebar{
        width:70px;
        top:60px;
        left:0;
        height:calc(100vh - 60px);
        padding:10px 6px;
    }

    #content{
        margin-left:70px;
        padding:16px;
    }

}

)rawliteral";

#endif
