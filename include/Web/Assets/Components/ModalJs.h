#ifndef MODAL_JS_H
#define MODAL_JS_H

#include <pgmspace.h>

const char MODAL_JS[] PROGMEM=R"rawliteral(

function openModal(title,content)
{
	let modal=$("modal");

	if(!modal)
	{
		modal=document.createElement("div");
		modal.id="modal";
		document.body.appendChild(modal);
	}

	modal.innerHTML=`
		<div class="modal-overlay" onclick="closeModal()"></div>
		<div class="modal-window">
			<div class="modal-header">
				<div class="modal-title">${title}</div>
				<button class="modal-close" onclick="closeModal()">
					<i class="fa-solid fa-xmark"></i>
				</button>
			</div>
			<div class="modal-body">
				${content}
			</div>
		</div>
	`;

	requestAnimationFrame(()=>
	{
		modal.classList.add("show");
	});
}

function closeModal()
{
	const modal=$("modal");

	if(!modal)
		return;

	modal.classList.remove("show");

	setTimeout(()=>
	{
		modal.innerHTML="";
	},200);
}

function modalVisible()
{
	const modal=$("modal");
	if(!modal)
		return false;
	return modal.classList.contains("show");
}

)rawliteral";

#endif
