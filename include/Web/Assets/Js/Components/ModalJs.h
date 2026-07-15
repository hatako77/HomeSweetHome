#ifndef MODAL_JS_H
#define MODAL_JS_H

#include <pgmspace.h>

const char MODAL_JS[] PROGMEM = R"rawliteral(

function initModal()
{
    if($("modal"))
        return;

    const modal = create("div");
    modal.id = "modal";

    document.body.appendChild(modal);
}

function showModal(title, content)
{
    const modal = $("modal");

    if(!modal)
        return;

    modal.innerHTML = `
        <div class="modal-overlay" id="modalOverlay"></div>

        <div class="modal-window">

            <div class="modal-header">

                <div class="modal-title">
                    ${title}
                </div>

                <button class="modal-close" id="modalClose">
                    <i class="fa-solid fa-xmark"></i>
                </button>

            </div>

            <div class="modal-body">
                ${content}
            </div>

        </div>
    `;

    $("modalOverlay").onclick = closeModal;
    $("modalClose").onclick = closeModal;

    requestAnimationFrame(() =>
    {
        modal.classList.add("show");
    });
}

function closeModal()
{
    const modal = $("modal");

    if(!modal)
        return;

    modal.classList.remove("show");

    setTimeout(() =>
    {
        modal.innerHTML = "";
    },200);
}

function modalVisible()
{
    const modal = $("modal");

    return modal ? modal.classList.contains("show") : false;
}

)rawliteral";

#endif
