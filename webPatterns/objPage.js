var anVarCount=1;
var anVar = [];

function imageProgressLoad() {
    for(var i = 0; i < anVarCount; i++) anVar[i].drawImage();
}

function init() {
    // Create the image resource
    progressImg = new Image();
    progressImg.onload = function () {
        anVar[0] = new myProgress("ain1","ainName");
        imageProgressLoad();
    }
    progressImg.src = "rect.png";
}
