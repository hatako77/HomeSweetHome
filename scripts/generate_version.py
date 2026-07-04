import os
import json
import hashlib
from datetime import datetime

# ==============================
# PlatformIO safe import
# ==============================
try:
    Import("env")
    PLATFORMIO = True
except:
    env = None
    PLATFORMIO = False

PROJECT_DIR = os.getcwd()

VERSION_FILE = os.path.join(PROJECT_DIR, "version.json")

# بهتره از env/platformio build dir استفاده نشه در CI
BUILD_DIR = os.path.join(PROJECT_DIR, ".pio", "build", "esp32dev")


# ==============================
# Load version
# ==============================
def load_version():
    if not os.path.exists(VERSION_FILE):
        return {
            "app": "HomeSweetHome",
            "version": "1.0.0",
            "build": 0
        }

    try:
        with open(VERSION_FILE, "r") as f:
            return json.load(f)
    except:
        return {
            "app": "HomeSweetHome",
            "version": "1.0.0",
            "build": 0
        }


# ==============================
# Save version
# ==============================
def save_version(data):
    with open(VERSION_FILE, "w") as f:
        json.dump(data, f, indent=4)


# ==============================
# SHA256
# ==============================
def calc_sha256(file_path):
    if not os.path.exists(file_path):
        return ""

    sha256 = hashlib.sha256()
    with open(file_path, "rb") as f:
        while True:
            chunk = f.read(4096)
            if not chunk:
                break
            sha256.update(chunk)

    return sha256.hexdigest()


# ==============================
# MAIN
# ==============================
def main():
    version = load_version()

    # افزایش build
    version["build"] += 1

    version_str = version["version"]

    firmware_name = f"HomeSweetHome_v{version_str}.bin"
    firmware_path = os.path.join(BUILD_DIR, "firmware.bin")

    version["sha256"] = calc_sha256(firmware_path)
    version["firmware"] = firmware_name

    # مهم: این URL باید با GitHub repo واقعی جایگزین شود
    version["url"] = (
        "https://github.com/YOUR_USERNAME/HomeSweetHome/releases/latest/download/"
        + firmware_name
    )

    version["update_required"] = False
    version["build_time"] = datetime.utcnow().isoformat() + "Z"

    save_version(version)

    print("✅ Version updated:", version)

    # فقط در PlatformIO
    if PLATFORMIO and env:
        env.Replace(PROJECTVERSION=version_str)


# ==============================
# RUN
# ==============================
main()
