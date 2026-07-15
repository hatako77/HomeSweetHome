#ifndef LOADER_JS_H
#define LOADER_JS_H

#include <pgmspace.h>

const char LOADER_JS[] PROGMEM=R"rawliteral(

function showLoader(text="Loading...")
{
	let loader=$("loader");
	if(!loader)
	{
		loader=document.createElement("div");
		loader.id="loader";
		loader.innerHTML=`
			<div class="loader-box">
				<div class="loader-spinner"></div>
				<div id="loaderText">${text}</div>
			</div>
		`;
		document.body.appendChild(loader);
	}
	else
	{
		$("loaderText").innerText=text;
	}
	loader.style.display="flex";
}

function hideLoader()
{
	const loader=$("loader");
	if(loader)
		loader.style.display="none";
}

function loaderText(text)
{
	if($("loaderText"))
		$("loaderText").innerText=text;
}

)rawliteral";

#endif
