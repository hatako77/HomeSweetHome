import json
import os
import hashlib
from datetime import datetime

PROJECT_DIR = os.path.dirname(os.path.dirname(__file__))
VERSION_FILE = os.path.join(PROJECT_DIR, "version.json")
BUILD_DIR = os.path.join(PROJECT_DIR, ".pio", "build", "esp32dev")

def load_version():
    if not os.path.exists(VERSION_FILE):
        return {
            "app": "HomeSweetHome",
            "version": "1.0.0",
            "build": 1
        }
    with open(VERSION_FILE, "r") as f:
        return json.load(f)

def save_version(data):
    with open(VERSION_FILE, "w") as f:
        json.dump(data, f, indent=4)

def calc_sha256(file_path):
    sha256 = hashlib.sha256()
    with open(file_path, "rb") as f:
        while chunk := f.read(4096):
            sha256.update(chunk)
    return sha256.hexdigest()

def main():
    version = load_version()

    # افزایش build number
    version["build"] += 1

    # ساخت نسخه string
    version_str = version["version"]
    firmware_name = f"HomeSweetHome_v{version_str}.bin"

    # پیدا کردن فایل firmware
    firmware_path = os.path.join(BUILD_DIR, "firmware.bin")

    if os.path.exists(firmware_path):
        version["sha256"] = calc_sha256(firmware_path)
    else:
        version["sha256"] = ""

    version["firmware"] = firmware_name
    version["url"] = f"https://github.com/YOUR_USERNAME/HomeSweetHome/releases/latest/download/{firmware_name}"
    version["update_required"] = False
    version["build_time"] = datetime.utcnow().isoformat()

    save_version(version)

    print("Version updated:")
    print(json.dumps(version, indent=4))

if __name__ == "__main__":
    main()
