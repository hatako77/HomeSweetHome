#ifndef DIALOG_JS_H
#define DIALOG_JS_H

#include <pgmspace.h>

const char DIALOG_JS[] PROGMEM=R"rawliteral(

function initDialog(title,content,buttons="")
{
	closeDialog();

	const overlay=document.createElement("div");
	overlay.id="dialogOverlay";

	overlay.innerHTML=`
		<div class="dialog">
			<div class="dialog-header">
				<div class="dialog-title">${title}</div>
				<button class="dialog-close" onclick="closeDialog()">
					<i class="fa-solid fa-xmark"></i>
				</button>
			</div>
			<div class="dialog-body">
				${content}
			</div>
			<div class="dialog-footer">
				${buttons}
			</div>
		</div>
	`;

	document.body.appendChild(overlay);

	requestAnimationFrame(()=>
	{
		overlay.classList.add("show");
	});
}

function closeDialog()
{
	const dialog=$("dialogOverlay");

	if(!dialog)
		return;

	dialog.classList.remove("show");

	setTimeout(()=>
	{
		dialog.remove();
	},200);
}

function confirmDialog(title,message,onYes)
{
	showDialog(
		title,
		`<p>${message}</p>`,
		`
		<button class="btn" onclick="closeDialog()">
			Cancel
		</button>
		<button class="btn" id="dialogYes">
			OK
		</button>
		`
	);

	$("dialogYes").onclick=()=>
	{
		closeDialog();

		if(onYes)
			onYes();
	};
}

function alertDialog(title,message)
{
	showDialog(
		title,
		`<p>${message}</p>`,
		`
		<button class="btn" onclick="closeDialog()">
			OK
		</button>
		`
	);
}

)rawliteral";

#endif
