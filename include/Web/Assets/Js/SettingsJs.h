#ifndef SETTINGS_JS_H
#define SETTINGS_JS_H

#include <pgmspace.h>

const char SETTINGS_JS[] PROGMEM=R"rawliteral(

async function showSettings()
{
	$("content").innerHTML=`
		<h2>Settings</h2>
		<div class="card">
			<div class="info">
				<div>
					<label>Device Name</label>
					<input id="deviceName" class="input">
				</div>
				<div>
					<label>WiFi SSID</label>
					<input id="wifiSsid" class="input">
				</div>
				<div>
					<label>WiFi Password</label>
					<input id="wifiPassword" class="input" type="password">
				</div>
				<div style="margin-top:20px;">
					<button class="btn" onclick="saveSettings()">
						<i class="fa-solid fa-floppy-disk"></i>
						Save
					</button>
				</div>
			</div>
		</div>
	`;

	loadSettings();
}

async function loadSettings()
{
	const data=await apiGet("/api/settings");
	if(!data)
		return;

	if($("deviceName"))
		$("deviceName").value=data.deviceName??"";

	if($("wifiSsid"))
		$("wifiSsid").value=data.ssid??"";

	if($("wifiPassword"))
		$("wifiPassword").value=data.password??"";
}

async function saveSettings()
{
	const body={
		deviceName:$("deviceName").value,
		ssid:$("wifiSsid").value,
		password:$("wifiPassword").value
	};

	const result=await apiPost("/api/settings",body);

	if(result&&result.success)
		alert("Settings Saved");
	else
		alert("Save Failed");
}

)rawliteral";

#endif
