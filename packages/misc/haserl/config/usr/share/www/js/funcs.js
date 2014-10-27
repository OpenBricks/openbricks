      var panels = new Array('panel1', 'panel2', 'panel3', 'panel4', 'panel5', 'panel6', 'panel7');
      var selectedTab = null;
      function showPanel(tab, name)
      {
        if (selectedTab) 
        {
          selectedTab.style.backgroundColor = '';
          selectedTab.style.paddingTop = '';
          selectedTab.style.marginTop = '4px';
        }
        selectedTab = tab;
        selectedTab.style.backgroundColor = 'white';
        selectedTab.style.paddingTop = '6px';
        selectedTab.style.marginTop = '0px';        
        for(i = 0; i < panels.length; i++)
        {
          document.getElementById(panels[i]).style.display = (name == panels[i]) ? 'block':'none';
        }
        return false;
      }




    function validateIP (IPvalue) {
        errorString = "";
        IPString = "IP address ";

        var ipPattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
        var ipArray = IPvalue.match(ipPattern);

        if (IPvalue == "0.0.0.0")
            errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
        else if (IPvalue == "255.255.255.255")
            errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
        if (ipArray == null)
            errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
        else {
            for (i = 0; i < 4; i++) {
                thisSegment = ipArray[i];
                if (thisSegment > 255) {
                    errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
                    i = 4;
                }
                if ((i == 0) && (thisSegment > 255)) {
                    errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
                    i = 4;
                  }
               }
        }
        if (errorString != ""){
            alert (errorString);
            return false;
        }
        else
            return true;
    }

    function validateIPz (IPvalue) {
        errorString = "";
        IPString = "IP address ";

        var ipPattern = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
        var ipArray = IPvalue.match(ipPattern);

        if (IPvalue == "255.255.255.255")
            errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
        if (ipArray == null)
            errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
        else {
            for (i = 0; i < 4; i++) {
                thisSegment = ipArray[i];
                if (thisSegment > 255) {
                    errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
                    i = 4;
                }
                if ((i == 0) && (thisSegment > 255)) {
                    errorString = errorString + IPString + ': '+IPvalue+' is not a valid IP address.';
                    i = 4;
                  }
               }
        }
        if (errorString != ""){
            alert (errorString);
            return false;
        }
        else
            return true;
    }


    function validateMask(mask) {
        mask = mask.replace( /\s/g, ""); //remove spaces for checking
        var re = /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/;
        if (re.test(mask)) {
            //split into units with dots "."
            var parts = mask.split(".");
            var validOctets = new Array();
    	    validOctets[0] = 255;
    	    validOctets[1] = 254;
    	    validOctets[2] = 252;
    	    validOctets[3] = 248;
    	    validOctets[4] = 240;
    	    validOctets[5] = 224;
    	    validOctets[6] = 192;
    	    validOctets[7] = 128;
    	    validOctets[8] = 0;
            var valid = 0;
            //if the first octet of the mask is zero
            if (parseInt(parseFloat(parts[0])) == 0)
                return false;
            //if the last octet of the mask is 255
            if (parseInt(parseFloat(parts[3])) == 255)
                return false;
            // check for valid octets
            for (var i=0; i<parts.length; i++) {
            	var octet = parseInt(parseFloat(parts[i]));
            	valid = 0;
            	for (var j=0 ; j<validOctets.length; j++){
            		if (octet == validOctets[j]){
            			valid = 1;
            			break; 
            			}
            	}
            	if ( valid == 0 )
            		return false;	
            }
            // check for bit holes in the mask
            var begin = 0;
            valid = 1;
            for (var i=3; i>=0; i--) {
            	var octet = parseInt(parseFloat(parts[i]));
            	if (begin == 0)
            		if ( octet == 0 )
            			continue;
            		else {
            			begin = 1;
            			continue;
            		}
            	else if ( octet != 255 ){
            		valid = 0;
            		break;
            	}
            }
        	if (valid == 0 )
        		return false;
        	return true;
        } 
        else 
            return false;
    }

    function validateMAC(macaddr) {
        errorString = "";
		macaddr = "AC:DE:48:" + macaddr ;
        MACString = "MAC address ";
        var reg1 = /^[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}$/;
        if (reg1.test(macaddr)) {
            var answer = confirm ("This device will reboot now. If it cannot get an IP address later, try to delete the old IP reservation in your DHCP server. Do you want continue?");
            if (answer)
                return true;
            else
                return false;

       } else {
          errorString = errorString + MACString + ': '+macaddr+' is not a valid MAC address.';
          alert (errorString);
          return false;
       }
    }


