import os
import json
import hashlib
from datetime import datetime

Import("env")

PROJECT_DIR = os.getcwd()

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
    if not os.path.exists(file_path):
        return ""

    sha256 = hashlib.sha256()
    with open(file_path, "rb") as f:
        while chunk := f.read(4096):
            sha256.update(chunk)
    return sha256.hexdigest()

def main():
    version = load_version()

    version["build"] += 1

    firmware_path = os.path.join(BUILD_DIR, "firmware.bin")

    version["sha256"] = calc_sha256(firmware_path)

    version_str = version["version"]
    version["firmware"] = f"HomeSweetHome_v{version_str}.bin"
    version["url"] = "https://github.com/YOUR_USERNAME/HomeSweetHome/releases/latest/download/" + version["firmware"]
    version["update_required"] = False
    version["build_time"] = datetime.utcnow().isoformat()

    save_version(version)

    print("Version updated:", version)

main()
