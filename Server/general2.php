<?php
	require_once( "includes/components/login.php" );

	$error = false;
	if( count( $_POST ) == 0 || $_FILES["versionFile"]["error"] == 1 || $_FILES["versionFile"]["error"] == 2 )
		$error = new errorClass( "File too large" );
	else if( $_FILES["versionFile"]["name"]  && !$_FILES["versionFile"]["tmp_name"] )
		$error = new errorClass( "Did not receive a file (" . $_FILES["versionFile"]["error"] . ")" );
	else
	{
		$itemid = $_POST["ID"];
		$parentid = $_POST["parentid"];
		$name = $_POST["itemName"];
		$description = $_POST["description"];
		$owneruser =  $_POST["owneruser"];
		$ownergroup =  $_POST["ownergroup"];
		$mode = 0;
		if( array_key_exists( "userReadPerm", $_POST ) )
			$mode |= 0400;
		if( array_key_exists( "userWritePerm", $_POST ) )
			$mode |= 0200;
		if( array_key_exists( "groupReadPerm", $_POST ) )
			$mode |= 0040;
		if( array_key_exists( "groupWritePerm", $_POST ) )
			$mode |= 0020;
		if( array_key_exists( "otherReadPerm", $_POST ) )
			$mode |= 0004;
		if( array_key_exists( "otherWritePerm", $_POST ) )
			$mode |= 0002;
	}
	if( !$error && canWrite( $itemid ) )
	{
		$queryResult = queryDatabase( 
			$dbConnect, 
			"update item_tree set name = $1, description = $2, modifieddate = $3, owneruser = $4, ownergroup = $5, mode=$6 where id=$7", 
			array( $name, $description, time(), $owneruser, $ownergroup, $mode, $itemid )
		);
		if( $queryResult && !is_object( $queryResult ) )
		{
			$versionFile = $_FILES["versionFile"]["tmp_name"];
			if( $versionFile )
				$error = createFileVersion( $itemid, $versionFile, $_FILES['versionFile']['type'], $description );
			
			if( !$error || !is_object( $error ) )
			{
				header("Location: index.php?ID=" . $parentid );
				exit();
			}
		}
		else
			$error = $queryResult;
	}
	else if( !$error )
		$error = new errorClass( "Permission denied" );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Strict//EN">
<html>
	<head>
		<?php
			$title = APPLICATION_NAME . " - Fehler";
			include_once( "includes/components/defhead.php" );
		?>
	</head>
	<body>
		<?php 
			include( "includes/components/headerlines.php" ); 
			include( "includes/components/error.php" ); 
/*
			echo( "<pre>" );
			var_dump( $_POST );
			var_dump( $_FILES );
			echo( "</pre>" );
*/
			include( "includes/components/footerlines.php" );
		?>
	</body>
</html>