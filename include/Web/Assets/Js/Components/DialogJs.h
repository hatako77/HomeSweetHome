#ifndef DIALOG_JS_H
#define DIALOG_JS_H

#include <pgmspace.h>

const char DIALOG_JS[] PROGMEM = R"rawliteral(

window.Dialog = {};

function initDialog()
{
}
Dialog.form = function(options)
{
    Dialog.open({
        title: options.title,
        content: options.content,
        footer:`
            <button class="btn" id="dialogCancel">
                Cancel
            </button>

            <button class="btn btn-primary" id="dialogSave">
                ${options.saveText ?? "Save"}
            </button>
        `
    });

    $("dialogCancel").onclick = () =>
    {
        Dialog.close();

        if(options.onCancel)
            options.onCancel();
    };

    $("dialogSave").onclick = async () =>
    {
        if(options.onSave)
        {
            const result = await options.onSave();

            if(result === false)
                return;
        }

        Dialog.close();
    };
}
Dialog.open = function(options)
{
    Dialog.close();

    const overlay = create("div");
    overlay.id = "dialogOverlay";
    overlay.innerHTML = `
        <div class="dialog">

            <div class="dialog-header">

                <div class="dialog-title">
                    ${options.title ?? ""}
                </div>

                <button class="dialog-close" id="dialogClose">
                    <i class="fa-solid fa-xmark"></i>
                </button>

            </div>

            <div class="dialog-body" id="dialogBody">
                ${options.content ?? ""}
            </div>

            <div class="dialog-footer">

                ${options.footer ?? ""}

            </div>

        </div>
    `;
    document.body.appendChild(overlay);
    
    const first = overlay.querySelector(
        "input,select,textarea"
    );
    
    if(first)
    {
        requestAnimationFrame(() =>
        {
            first.focus();
    
            if(first.select)
                first.select();
        });
    }

    $("dialogClose").onclick = () =>
    {
        Dialog.close();

        if(options.onCancel)
            options.onCancel();
    };

    requestAnimationFrame(() =>
    {
        overlay.classList.add("show");
    });
}

Dialog.close = function()
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

Dialog.alert = function(title,message)
{
    Dialog.open({

        title,

        content:`
            <p>${message}</p>
        `,

        footer:`
            <button class="btn btn-primary" id="dialogOk">
                OK
            </button>
        `

    });

    $("dialogOk").onclick=()=>
    {
        Dialog.close();
    };
}

Dialog.confirm=function(title,message,onYes)
{
    Dialog.open({

        title,

        content:`
            <p>${message}</p>
        `,

        footer:`
            <button class="btn" id="dialogCancel">
                Cancel
            </button>

            <button class="btn btn-primary" id="dialogYes">
                OK
            </button>
        `

    });

    $("dialogCancel").onclick=()=>
    {
        Dialog.close();
    };

    $("dialogYes").onclick=()=>
    {
        Dialog.close();

        if(onYes)
            onYes();
    };
}

Dialog.prompt=function(options)
{
    Dialog.open({

        title:options.title,

        content:`

            <input
                id="dialogInput"
                class="textbox"
                placeholder="${options.placeholder ?? ""}"
                value="${options.value ?? ""}"
            >

        `,

        footer:`

            <button class="btn" id="dialogCancel">
                Cancel
            </button>

            <button class="btn btn-primary" id="dialogSave">
                Save
            </button>

        `

    });

    const input=$("dialogInput");

    input.focus();
    input.select();

    input.onkeydown=(e)=>
    {
        if(e.key==="Enter")
            $("dialogSave").click();

        if(e.key==="Escape")
            $("dialogCancel").click();
    };

    $("dialogCancel").onclick=()=>
    {
        Dialog.close();
    };

    $("dialogSave").onclick=()=>
    {
        const value=input.value.trim();

        if(options.required!==false && value==="")
        {
            input.focus();
            return;
        }

        Dialog.close();

        if(options.onSave)
            options.onSave(value);
    };
}

)rawliteral";

#endif
