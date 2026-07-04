#ifndef OTA_HTML_H
#define OTA_HTML_H

const char OTA_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>OTA Update</title>

  <style>
    body {
      font-family: Arial;
      background: #111;
      color: #fff;
      text-align: center;
      padding: 30px;
    }

    .box {
      max-width: 500px;
      margin: auto;
      background: #1e1e1e;
      padding: 20px;
      border-radius: 10px;
    }

    button {
      padding: 12px 20px;
      margin-top: 20px;
      border: none;
      border-radius: 8px;
      background: #00c853;
      color: white;
      font-size: 16px;
      cursor: pointer;
    }

    button:disabled {
      background: #555;
      cursor: not-allowed;
    }

    .bar {
      width: 100%;
      background: #333;
      border-radius: 10px;
      overflow: hidden;
      margin-top: 20px;
    }

    .progress {
      height: 20px;
      width: 0%;
      background: #00c853;
      transition: width 0.3s;
    }

    .hidden {
      display: none;
    }

    .info {
      margin-top: 10px;
      font-size: 14px;
      color: #aaa;
    }
  </style>
</head>

<body>

<div class="box">

  <h2>HomeSweetHome OTA</h2>

  <p>Current Version: <span id="current">-</span></p>
  <p>Server Version: <span id="remote">-</span></p>

  <button id="updateBtn" onclick="startUpdate()" disabled>Update Firmware</button>

  <div class="bar hidden" id="bar">
    <div class="progress" id="progress"></div>
  </div>

  <p class="info" id="status"></p>

</div>

<script>

let currentVersion = "";
let remoteVersion = "";

// =======================
// Load version info
// =======================
async function checkVersion() {
  let res = await fetch('/version');
  let data = await res.json();

  currentVersion = data.version;
  document.getElementById("current").innerText = currentVersion;

  // fake server version check (replace with real API if needed)
  let remote = await fetch('/update'); // فقط برای trigger نیست، بهتره endpoint جدا داشته باشی
}

// =======================
// Start OTA
// =======================
async function startUpdate() {

  document.getElementById("updateBtn").disabled = true;
  document.getElementById("bar").classList.remove("hidden");

  let progress = document.getElementById("progress");
  let status = document.getElementById("status");

  status.innerText = "Updating...";

  // fake progress animation (ESP32 doesn't stream progress by default)
  let p = 0;
  let interval = setInterval(() => {
    if (p < 90) {
      p += 5;
      progress.style.width = p + "%";
    }
  }, 300);

  try {
    let res = await fetch('/update');
    let text = await res.text();

    clearInterval(interval);
    progress.style.width = "100%";

    status.innerText = "Update finished. Restarting...";

    // 5 sec countdown
    let t = 5;
    let timer = setInterval(() => {
      status.innerText = "Rebooting in " + t + "s...";
      t--;

      if (t < 0) {
        clearInterval(timer);
        window.location.href = "/";
      }
    }, 1000);

  } catch (e) {
    clearInterval(interval);
    status.innerText = "Update failed!";
  }
}

// =======================
// Init
// =======================
checkVersion();

</script>

</body>
</html>
)rawliteral";

#endif
