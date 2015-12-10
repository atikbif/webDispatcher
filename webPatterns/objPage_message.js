function clearMessage(id) {
	m = document.getElementById(id);
    m.innerText = "";
}

function updMessage(id,message) {
	m = document.getElementById(id);
    m.innerText = message;
}

function setMessageType(id,type) {
    m = document.getElementById(id);
    var cl = "alert ";
    if(type==1) cl+="alert-success";
    else if(type==2) cl+="alert-warning";
    else if(type==3) cl+="alert-danger";
    else cl+="alert-info";
    m.class = cl;
}
