var anVarCount=1;
var anVar = [];
var tmr;

function imageProgressLoad() {
    for(var i = 0; i < anVarCount; i++) anVar[i].drawImage();
}

function setObTime(t) {
    te = document.getElementById("obTime");
    if(te!=null) {
        te.innerText = t;
    }
}

function setObColour(c) {
    tc = document.getElementById("obStat");
    if(tc!=null) {
        var cl = "panel ";
        if(c==0) cl+="panel-default";
        else if(c==1) cl+="panel-success";
        else if(c==2) cl+="panel-warning";
        else if(c==3) cl+="panel-danger";
        else cl+="panel-default";
        tc.className = cl;
    }
}

function getAnVar(id) {
	for(var i = 0; i < anVarCount; i++) if(anVar[i].id==id) return anVar[i];
	return null;
}

function loadStatus() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange=function() {
        if (xhttp.readyState == 4 && xhttp.status == 200) {
            var resp = xhttp.responseText;
            //document.getElementById("debug").innerHTML = resp;
            var timeAndType = /(.*)=(.*)/;
            var par = resp.match(timeAndType);
            var type = parseInt(par[2]);
            setObTime(par[1]);
            setObColour(type);
        }
    };
    xhttp.open("GET", "status.txt?t=" + Math.random()+"&ob=obPageName", true);
    xhttp.send();
}

function loadData() {
	loadDin();
	loadAin();
    loadMessage();
    loadStatus();
}



function init() {
    // Create the image resource
    progressImg = new Image();
    progressImg.onload = function () {
        anVar[0] = new myProgress("ain1","ainName");
        imageProgressLoad();
    }
    progressImg.src = "rect.png";
	loadData();
    tmr = setInterval(loadData, 2000);
}
