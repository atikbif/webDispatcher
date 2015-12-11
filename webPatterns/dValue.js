function setOn(id) {
	din = document.getElementById(id);
	if(din!=null) {
		din.src="on.png";
		din.alt="вкл";
	}
}

function setOff(id) {
	din = document.getElementById(id);
	if(din!=null) {
		din.src="off.png";
		din.alt="выкл";
	}
}

function loadDin() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange=function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
		var resp = xhttp.responseText;
		
		var pattern = /;/;
		var nameList = resp.split(pattern);
        //document.getElementById("debug").innerHTML = resp;
		var i,len;
		var res=" ";
		for (i = 0, len = nameList.length; i < len; i++)  {
			var str = nameList[i];
			if(str.length>0) {
				var idAndValue = /(.*)=(.*)/;
				var par;
				par = str.match(idAndValue);
				if(par[2]=="1") setOn(par[1]);
				else if(par[2]=="0") setOff(par[1]);
			}			
		}
    }
  };
  xhttp.open("GET", "din.txt?t=" + Math.random()+"&ob=obPageName", true);
  xhttp.send();
}
