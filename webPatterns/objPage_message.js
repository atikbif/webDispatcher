function clearMessage(id) {
	m = document.getElementById(id);
    if(m!=null) {
        m.innerText = "";
        m.className = "alert alert-info";
    }
}

function updMessage(id,message) {
	m = document.getElementById(id);
    if(m!=null) m.innerText = message;
}

function setMessageType(id,type) {
    m = document.getElementById(id);
    var cl = "alert ";
    if(type==1) cl+="alert-success";
    else if(type==2) cl+="alert-warning";
    else if(type==3) cl+="alert-danger";
    else cl+="alert-info";
    if(m!=null) m.className = cl;
}

function loadMessage() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange=function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
        var resp = xhttp.responseText;
        document.getElementById("debug").innerHTML = resp;
        var i;
        for(i=1;i<=10;i++) {clearMessage("mes"+i);}
        var pattern = /;/;
        var nameList = resp.split(pattern);
        var len = nameList.length;
        for (i=0;(i<len)&&(i<10);i++)  {
            var num = i+1;
            var str = nameList[i];
            if((str.length>0)){
                var textAndType = /(.*)=(.*)/;
                var par;
                par = str.match(textAndType);
                var type = parseInt(par[2]);
                updMessage("mes"+num,par[1]);
                setMessageType("mes"+(i+1),type);
            }
        }
    }
  };
  xhttp.open("GET", "message.txt?t=" + Math.random()+"&ob=obPageName", true);
  xhttp.send();
}
