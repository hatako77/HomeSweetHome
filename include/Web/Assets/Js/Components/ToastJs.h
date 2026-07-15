#ifndef TOAST_JS_H
#define TOAST_JS_H

#include <pgmspace.h>

const char TOAST_JS[] PROGMEM = R"rawliteral(

function initToast()
{
    if($("toastContainer"))
        return;

    const container = create("div");
    container.id = "toastContainer";

    document.body.appendChild(container);
}

function showToast(message, type = "info")
{
    const container = $("toastContainer");

    if(!container)
        return;

    const toast = create("div", "toast " + type);

    toast.innerHTML = `
        <div class="toast-message">
            ${message}
        </div>
    `;

    container.appendChild(toast);

    requestAnimationFrame(() =>
    {
        toast.classList.add("show");
    });

    setTimeout(() =>
    {
        toast.classList.remove("show");

        setTimeout(() =>
        {
            toast.remove();
        },300);

    },3000);
}

function toastSuccess(message)
{
    showToast(message, "success");
}

function toastError(message)
{
    showToast(message, "error");
}

function toastWarning(message)
{
    showToast(message, "warning");
}

function toastInfo(message)
{
    showToast(message, "info");
}

)rawliteral";

#endif
