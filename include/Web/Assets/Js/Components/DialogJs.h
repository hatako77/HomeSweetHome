#ifndef DIALOG_JS_H
#define DIALOG_JS_H

#include <pgmspace.h>

const char DIALOG_JS[] PROGMEM = R"rawliteral(

function initDialog()
{
}

function showDialog(title, content, buttons = "")
{
    closeDialog();

    const overlay = create("div");
    overlay.id = "dialogOverlay";

    overlay.innerHTML = `
        <div class="dialog">

            <div class="dialog-header">

                <div class="dialog-title">
                    ${title}
                </div>

                <button class="dialog-close" id="dialogClose">
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

    $("dialogClose").onclick = closeDialog;

    requestAnimationFrame(() =>
    {
        overlay.classList.add("show");
    });
}

function closeDialog()
{
    const dialog = $("dialogOverlay");

    if(!dialog)
        return;

    dialog.classList.remove("show");

    setTimeout(() =>
    {
        dialog.remove();
    },200);
}

function confirmDialog(title, message, onYes)
{
    showDialog(
        title,
        `<p>${message}</p>`,
        `
        <button class="btn" id="dialogCancel">
            Cancel
        </button>

        <button class="btn btn-primary" id="dialogYes">
            OK
        </button>
        `
    );

    $("dialogCancel").onclick = closeDialog;

    $("dialogYes").onclick = () =>
    {
        closeDialog();

        if(onYes)
            onYes();
    };
}

function alertDialog(title, message)
{
    showDialog(
        title,
        `<p>${message}</p>`,
        `
        <button class="btn btn-primary" id="dialogOk">
            OK
        </button>
        `
    );

    $("dialogOk").onclick = closeDialog;
}

)rawliteral";

#endif
