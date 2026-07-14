#ifndef TOAST_JS_H
#define TOAST_JS_H

#include <pgmspace.h>

const char TOAST_JS[] PROGMEM=R"rawliteral(

function toast(message,type="info")
{
	let container=$("toastContainer");
	if(!container)
	{
		container=document.createElement("div");
		container.id="toastContainer";
		document.body.appendChild(container);
	}
	const item=document.createElement("div");
	item.className="toast "+type;
	item.innerHTML=`
		<div class="toast-message">${message}</div>
	`;
	container.appendChild(item);
	requestAnimationFrame(()=>
	{
		item.classList.add("show");
	});
	setTimeout(()=>
	{
		item.classList.remove("show");
		setTimeout(()=>
		{
			item.remove();
		},300);
	},3000);
}

function toastSuccess(message)
{
	toast(message,"success");
}

function toastError(message)
{
	toast(message,"error");
}

function toastWarning(message)
{
	toast(message,"warning");
}

function toastInfo(message)
{
	toast(message,"info");
}

)rawliteral";

#endif
