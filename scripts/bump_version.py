import json
import sys
import os

VERSION_FILE = "version.json"

def load():
    with open(VERSION_FILE, "r") as f:
        return json.load(f)

def save(data):
    with open(VERSION_FILE, "w") as f:
        json.dump(data, f, indent=4)

def bump(version, bump_type):
    major, minor, patch = map(int, version.split("."))

    if bump_type == "major":
        major += 1
        minor = 0
        patch = 0

    elif bump_type == "minor":
        minor += 1
        patch = 0

    elif bump_type == "patch":
        patch += 1

    return f"{major}.{minor}.{patch}"

def main():
    bump_type = sys.argv[1]

    data = load()
    old_version = data["version"]

    new_version = bump(old_version, bump_type)

    data["version"] = new_version
    data["build"] += 1

    save(data)

    print(f"Bumped: {old_version} -> {new_version}")

if __name__ == "__main__":
    main()
