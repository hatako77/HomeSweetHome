#ifndef OTA_JS_H
#define OTA_JS_H

#include <pgmspace.h>

const char OTA_JS[] PROGMEM=R"rawliteral(

function showOTA()
{
	$("content").innerHTML=`
		<h2>Firmware Update</h2>
		<div class="card">
			<div class="info">
				<div>
					Current Version :
					<b id="otaCurrent">-</b>
				</div>
				<div>
					Latest Version :
					<b id="otaRemote">-</b>
				</div>
				<div>
					Status :
					<b id="otaState">Idle</b>
				</div>
			</div>
			<progress id="otaProgress" value="0" max="100" style="width:100%;margin-top:15px;"></progress>
			<div style="display:flex;justify-content:space-between;margin-top:10px;">
				<span id="otaPercent">0%</span>
				<span id="otaSpeed">0 KB/s</span>
				<span id="otaEta">0 s</span>
			</div>
			<div style="display:flex;gap:10px;margin-top:20px;">
				<button class="btn" onclick="checkOTA()">
					Check
				</button>
				<button class="btn" onclick="startOTA()">
					Update
				</button>
			</div>
		</div>
	`;
	wsSend("ota","check");
}

function checkOTA()
{
	wsSend("ota","check");
}

function startOTA()
{
	wsSend("ota","start");
}

function updateOTA(data)
{
	if($("otaState"))
		$("otaState").innerText=data.state??"-";

	if($("otaCurrent"))
		$("otaCurrent").innerText=data.current??"-";

	if($("otaRemote"))
		$("otaRemote").innerText=data.remote??"-";

	if($("otaProgress"))
		$("otaProgress").value=data.percent??0;

	if($("otaPercent"))
		$("otaPercent").innerText=(data.percent??0)+"%";

	if($("otaSpeed"))
		$("otaSpeed").innerText=((data.speed??0).toFixed(1))+" KB/s";

	if($("otaEta"))
		$("otaEta").innerText=(data.eta??0)+" s";
}

)rawliteral";

#endif
