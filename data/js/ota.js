//====================== data/js/ota.js ======================

let currentVersion="";

fetch("/api/version")
.then(r=>r.json())
.then(j=>{

currentVersion=j.version;

document.getElementById("currentVersion").innerHTML=currentVersion;

});

fetch("/api/check")
.then(r=>r.json())
.then(j=>{

if(j.update){

document.getElementById("latestVersion").innerHTML=j.latest;

document.getElementById("autoBtn").disabled=false;

}

else{

document.getElementById("latestVersion").innerHTML="Latest";

}

});

function setProgress(value,text){

document.getElementById("progress").style.width=value+"%";

document.getElementById("percent").innerHTML=value+" %";

document.getElementById("status").innerHTML=text;

}

function startCountdown(){

let sec=5;

document.getElementById("status").innerHTML="Update completed successfully.<br>ESP32 is restarting...";

let timer=setInterval(()=>{

document.getElementById("countdown").innerHTML=

"Redirecting in "+sec+" second(s)...";

sec--;

if(sec<0){

clearInterval(timer);

location="/";

}

},1000);

}

function autoUpdate(){

setProgress(5,"Connecting GitHub");

fetch("/api/update",{

method:"POST"

})

.then(()=>{

setProgress(100,"Done");

startCountdown();

});

}

function manualUpdate(){

let file=document.getElementById("firmware").files[0];

if(!file)return;

let xhr=new XMLHttpRequest();

xhr.upload.onprogress=function(e){

if(e.lengthComputable){

let p=Math.round(e.loaded/e.total*100);

setProgress(p,"Uploading Firmware");

}

}

xhr.onload=function(){

setProgress(100,"Completed");

startCountdown();

}

let form=new FormData();

form.append("update",file);

xhr.open("POST","/api/update");

xhr.send(form);

}
