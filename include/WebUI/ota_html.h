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
