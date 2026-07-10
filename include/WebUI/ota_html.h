#ifndef OTA_HTML_H
#define OTA_HTML_H

const char OTA_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<meta charset="UTF-8">

<title>HomeSweetHome OTA</title>

<style>

*{
    box-sizing:border-box;
}

body{

    margin:0;

    background:#111;

    color:white;

    font-family:Arial,Helvetica,sans-serif;
}

.container{

    width:420px;

    max-width:95%;

    margin:40px auto;

    background:#1d1d1d;

    border-radius:12px;

    padding:25px;

    box-shadow:0 0 20px rgba(0,0,0,.4);
}

h2{

    margin-top:0;

    text-align:center;

    color:#00d26a;
}

.row{

    display:flex;

    justify-content:space-between;

    margin:10px 0;

    font-size:15px;
}

.label{

    color:#bbbbbb;
}

.value{

    color:white;

    font-weight:bold;
}

.progress-bg{

    margin-top:20px;

    width:100%;

    height:26px;

    background:#333;

    border-radius:13px;

    overflow:hidden;
}

.progress{

    width:0%;

    height:100%;

    background:#00d26a;

    transition:.2s;
}

.center{

    text-align:center;
}

.big{

    font-size:24px;

    margin-top:10px;
}

.small{

    color:#aaa;

    font-size:14px;
}

button{

    width:100%;

    margin-top:25px;

    border:none;

    border-radius:8px;

    padding:14px;

    font-size:16px;

    cursor:pointer;

    background:#00d26a;

    color:white;
}

button:disabled{

    background:#555;

    cursor:not-allowed;
}

.error{

    color:#ff5b5b;

    margin-top:15px;

    text-align:center;

    font-weight:bold;
}

.ok{

    color:#00d26a;

    margin-top:15px;

    text-align:center;

    font-weight:bold;
}

.hidden{

    display:none;
}

</style>

</head>

<body>

<div class="container">

<h2>HomeSweetHome OTA</h2>

<div class="row">
<div class="label">Current Version</div>
<div class="value" id="current">-</div>
</div>

<div class="row">
<div class="label">Latest Version</div>
<div class="value" id="remote">-</div>
</div>

<div class="row">
<div class="label">Status</div>
<div class="value" id="state">Idle</div>
</div>

<div class="progress-bg">

<div class="progress" id="progress"></div>

</div>

<div class="center big" id="percent">

0%

</div>

<div class="center small" id="size">

0 / 0 MB

</div>

<div class="center small" id="speed">

0 KB/s

</div>

<div class="center small" id="eta">

ETA ---

</div>

<div id="message" class="center small">

Checking...

</div>

<div id="error" class="error hidden"></div>

<div id="success" class="ok hidden">

Firmware updated successfully.<br>

Restarting...

</div>

<button id="updateBtn" onclick="startOTA()" disabled>

Update Firmware

</button>
<script>
<script>

let socket;
let rebootTimer = null;

//==========================
function connectWS()
{
    const protocol =
        location.protocol === "https:"
        ? "wss://"
        : "ws://";

    socket = new WebSocket(
        protocol + location.host + "/ws"
    );

    socket.onopen = () =>
    {
        console.log("OTA WebSocket Connected");

        socket.send(JSON.stringify({
            cmd:"ota.check"
        }));
    };

    socket.onclose = () =>
    {
        console.log("OTA WebSocket Closed");

        setTimeout(
            connectWS,
            2000
        );
    };

    socket.onmessage = (event)=>
    {
        let msg;

        try
        {
            msg = JSON.parse(event.data);
        }
        catch
        {
            return;
        }

        if(msg.type=="hello")
            return;

        if(msg.type!="ota")
            return;

        updateOTA(msg);
    };
}

//==========================
function updateOTA(s)
{
    document.getElementById("state").innerHTML =
        s.state;

    document.getElementById("progress").style.width =
        s.percent + "%";

    document.getElementById("percent").innerHTML =
        s.percent + "%";

    let down =
        (s.downloaded/1024/1024).toFixed(2);

    let total =
        (s.total/1024/1024).toFixed(2);

    document.getElementById("size").innerHTML =
        down + " MB / " + total + " MB";

    document.getElementById("speed").innerHTML =
        s.speed.toFixed(1) + " KB/s";

    document.getElementById("eta").innerHTML =
        s.eta > 0
        ? "ETA : " + s.eta + " sec"
        : "";

    document.getElementById("message").innerHTML =
        s.state;

    if(s.current)
        document.getElementById("current").innerHTML =
            s.current;

    if(s.remote)
        document.getElementById("remote").innerHTML =
            s.remote;

    if(s.error!="")
    {
        document
            .getElementById("error")
            .classList
            .remove("hidden");

        document
            .getElementById("error")
            .innerHTML =
            s.error;

        document
            .getElementById("updateBtn")
            .disabled = false;

        return;
    }

    if(s.state=="Update Available")
    {
        document
            .getElementById("updateBtn")
            .disabled = false;

        document
            .getElementById("message")
            .innerHTML =
            "New firmware available";
    }

    if(s.state=="Up To Date")
    {
        document
            .getElementById("updateBtn")
            .disabled = true;

        document
            .getElementById("message")
            .innerHTML =
            "Firmware is up to date";
    }
    if(s.finished && s.success)
    {
        document
            .getElementById("success")
            .classList
            .remove("hidden");

        let sec = 5;

        rebootTimer = setInterval(()=>
        {
            document
                .getElementById("message")
                .innerHTML =
                "Restarting in " + sec + " sec";

            sec--;

            if(sec < 0)
            {
                clearInterval(rebootTimer);

                location.href = "/";
            }

        },1000);

        return;
    }
}

//==========================
function startOTA()
{
    document
        .getElementById("updateBtn")
        .disabled = true;

    document
        .getElementById("error")
        .classList
        .add("hidden");

    document
        .getElementById("success")
        .classList
        .add("hidden");

    socket.send(JSON.stringify({
        cmd:"ota.update"
    }));
}
//==========================
connectWS();

</script>


</div>

</body>

</html>

)rawliteral";

#endif
