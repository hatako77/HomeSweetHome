
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

async function checkVersion() {

  try {
    let res = await fetch('/version');
    let data = await res.json();

    document.getElementById("current").innerText = data.version;

    let remoteRes = await fetch('/ota-version');
    let remoteData = await remoteRes.json();

    document.getElementById("remote").innerText = remoteData.version;

    // ساده‌ترین مقایسه (فرض: format = x.y.z)
    if (isNewer(remoteData.version, data.version)) {
      document.getElementById("updateBtn").disabled = false;
    } else {
      document.getElementById("updateBtn").disabled = true;
    }

  } catch (e) {
    document.getElementById("status").innerText = "Version check failed";
  }
}

// =======================
// Version Compare
// =======================
function isNewer(remote, current) {

  let r = remote.split(".").map(Number);
  let c = current.split(".").map(Number);

  for (let i = 0; i < Math.max(r.length, c.length); i++) {
    let rv = r[i] || 0;
    let cv = c[i] || 0;

    if (rv > cv) return true;
    if (rv < cv) return false;
  }

  return false;
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

  let p = 0;
  let interval = setInterval(() => {
    if (p < 90) {
      p += 5;
      progress.style.width = p + "%";
    }
  }, 300);

  try {
    await fetch('/update');

    clearInterval(interval);
    progress.style.width = "100%";

    status.innerText = "Update finished. Restarting...";

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

// Init
checkVersion();

</script>

</body>
</html>
)rawliteral";

#endif
