#ifndef APP_JS_H
#define APP_JS_H

#include <pgmspace.h>

const char APP_JS[] PROGMEM = R"rawliteral(
'use strict';

window.App={
	version:'2.0.0',
	ws:null,
	api:{},
	dom:{},
	ui:{}
};

window.addEventListener('DOMContentLoaded',async()=>{
	if(typeof initDom==='function')
		initDom();

	if(typeof initIcons==='function')
		initIcons();

	if(typeof initSidebar==='function')
		initSidebar();

	if(typeof initDialog==='function')
		initDialog();

	if(typeof initModal==='function')
		initModal();

	if(typeof initToast==='function')
		initToast();

	if(typeof initLoader==='function')
		initLoader();

	if(typeof initApi==='function')
		initApi();

	if(typeof initWebSocket==='function')
		initWebSocket();

	if(typeof loadDashboard==='function')
		initDashboard();
});

)rawliteral";

#endif
