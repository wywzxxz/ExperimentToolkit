if (!include) 
function include(jsurl)
{
	if (!include.map) include.map={};
	if (include.map[jsurl]) return;include.map[jsurl]=true;
	if (jsurl == null || typeof(jsurl) != 'string') return;
	//$.ajaxSetup({ cache : true });
	var type=/\.[^\.]+/.exec(jsurl.trim());
	if (!type || type=="")	type=".js";	
	if (type==".css")
	{
		var style = document.createElement('link');
		style.href=jsurl;
		style.rel='stylesheet';
		style.type='text/css';
		document.getElementsByTagName('HEAD').item(0).appendChild(style);
	}
	else{
		if (include.debug)
		{
			var script = document.createElement('script');
			script.type = 'text/javascript';
			script.charset = 'utf-8';
			script.src = jsurl;
			document.getElementsByTagName('HEAD').item(0).appendChild(script);
			return;
		}	
		try{
			var xmlhttp;
			if (window.XMLHttpRequest)	xmlhttp   =   new   XMLHttpRequest();//isIE   =   false;    
			if (window.ActiveXObject)   xmlhttp   =   new   ActiveXObject("Microsoft.XMLHTTP");//isIE   =   true;       
			xmlhttp.open("GET",jsurl, false);   
			//xmlhttp.SetRequestHeader("Content-Type","text/html; charset=Shift_JIS")   
			xmlhttp.send(null);   
			var result = xmlhttp.status;   			
			console.log(result);
			var text;if(result==0 || result==200)  text=xmlhttp.responseText;
			else throw "failed to load "+jsurl+"(status "+result+")";
			xmlhttp = null;  			
			//( window.execScript || function( data ) {window[ "eval" ].call( window, data );} )(text);	
			var script = document.createElement('script');
			script.type = 'text/javascript';
			//script.charset = 'utf-8';
			//script.src = jsurl;
			script.innerHTML ="/*"+jsurl+"*/\n"+ text;
			document.getElementsByTagName('HEAD').item(0).appendChild(script);
		}catch(e)
		{
			console.log("include failed on"+jsurl,e);
		}
	}
	
	
 }
