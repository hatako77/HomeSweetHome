window.addEventListener("DOMContentLoaded", async () =>
{
    initDom();
    initIcons();
    initSidebar();
    initDialog();
    initModal();
    initToast();
    initLoader();
    initApi();
    initWebSocket();

    await Router.navigate("dashboard");

    hideLoader();
});
