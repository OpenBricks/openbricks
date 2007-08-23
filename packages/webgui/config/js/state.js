var request = false;

function getState() {
	var stateUrl = "/cgi-bin/state.cgi";
	if (window.XMLHttpRequest) {
		request = new XMLHttpRequest(); 
	} else if (window.ActiveXObject) {
		request = new ActiveXObject("Microsoft.XMLHTTP"); 
	} 
	request.onreadystatechange = writeState;
	request.open('GET', stateUrl, true); 
	request.send(null); 
	setTimeout("getState()",5000);
}

function writeState() {
	if (request.readyState == 4) {
		if (request.status == 200) {
			document.getElementById('headPlayerState').innerHTML = request.responseText;
		} else {
			document.getElementById('headPlayerState').innerHTML = "GeeXBoX Open Media Center";
		}
	}
}