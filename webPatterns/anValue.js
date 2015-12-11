var progressImg=null;

function myProgress(id,name) {
	this.iHEIGHT = 75;
	this.iWIDTH = 160;
	this.min = 0;
	this.max = 100;
	this.name = name;
	this.id = id;
	this.value = 50;
	this.ctx = null;
	this.canvas = null;
	this.colour =1;
	this.draw = function() {
		canvas = document.getElementById(this.id);
		if(canvas.getContext) {
			ctx = canvas.getContext("2d");
			ctx.rect(10,10,50,50);
			ctx.stroke();
		}
	};
	this.drawBase = function() {
		ctx.drawImage(progressImg, 0, 0, this.iWIDTH, this.iHEIGHT, 0, 0, this.iWIDTH, this.iHEIGHT);
	};
	this.drawValue = function() {
		ctx.fillStyle = "yellow";
		ctx.font = "10pt Arial";
		ctx.fillText(this.value.toFixed(1), 62, 64);
	};
	this.drawLimits = function() {
		ctx.fillStyle = "black";
		ctx.font = "7pt Arial";
		ctx.fillText(this.min, 14, 63);
		ctx.fillText(this.max, 118, 63);
	};
	this.drawName = function() {
		ctx.fillStyle = "black";
		ctx.font = "9pt Arial";
		ctx.fillText(this.name, 14, 20);
	};
	this.drawProgress = function() {
		var x1 = 10, // X position where the progress segment starts
        x2 = 150, // X position where the progress segment ends
        x3 = 0;
		var y = 75;
		if(this.colour==1) y=75;
		if(this.colour==2) y=90;
		if(this.colour==3) y=105;
        if(this.value<this.min) x3=0;
        else if(this.value>this.max) x3=x2-x1;
        else {
            var perc = (this.value - this.min)*1.0;
            perc = perc / (this.max - this.min);
            x3 = ((x2 - x1)  * perc);
        }
		ctx.drawImage(progressImg, 15, y, x3, 15, 15, 30, x3, 15);
	};
	this.drawImage = function() {
		canvas = document.getElementById(id);
		if(canvas.getContext) {
			ctx = canvas.getContext("2d");
			ctx.clearRect(0,0,160,100);
			// Draw the base image - no progress
			this.drawBase();
			this.drawLimits();
			this.drawName();
			this.drawProgress();
			this.drawValue();
		}
	};
};

function loadAin() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange=function() {
    if (xhttp.readyState == 4 && xhttp.status == 200) {
		var resp = xhttp.responseText;
		
		var pattern = /;/;
		var nameList = resp.split(pattern);
        //document.getElementById("debug").innerHTML = resp;
		var i,len;
		for (i = 0, len = nameList.length; i < len; i++)  {
			var str = nameList[i];
			if(str.length>0) {
				var idAndValue = /(.*)=(.*)/;
				var par;
				par = str.match(idAndValue);
				var anV = getAnVar(par[1]);
				if(anV!=null) {
					var aData = par[2];
					var adPattern = /_/;
					var dataList = aData.split(adPattern);
					anV.value = parseFloat(dataList[0]);
					anV.colour = parseInt(dataList[1]);
					anV.drawImage();
				}
			}			
		}
    }
  };
  xhttp.open("GET", "ain.txt?t=" + Math.random()+"&ob=obPageName", true);
  xhttp.send();
}

