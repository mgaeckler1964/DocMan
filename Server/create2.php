<?php 
	require_once( "includes/components/login.php" );

	$error = false;
	if( count( $_POST ) == 0 || $_FILES["versionFile"]["error"] == 1 || $_FILES["versionFile"]["error"] == 2 )
		$error = new errorClass( "File too large" );
	else if( $_FILES["versionFile"]["name"]  && !$_FILES["versionFile"]["tmp_name"] )
		$error = new errorClass( "Did not receive a file (" . $_FILES["versionFile"]["error"] . ")" );
	else
	{
		$parentid = $_POST["parentid"];
		$name = $_POST["folderName"];
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
		$itemType = $_POST["itemType"];
	}
	if( !$error && canWrite( $parentid ) )
	{
		$nextID = getNextID( $dbConnect, "item_tree", "id" );
		if( is_numeric( $nextID ) )
		{
			$queryResult = queryDatabase( 
				$dbConnect, 
				"insert into item_tree ".
				"( id, parentid, item_type, name, description, createdate, modifieddate, owneruser, ownergroup, mode, createby ) ".
				"values ".
				"( $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11 )", 
				array( $nextID, $parentid, $itemType, $name, $description, time(), time(), $owneruser, $ownergroup, $mode, $actUser['id']  )
			);
			if( $queryResult && !is_object($queryResult) )
			{
				if( $itemType == 1 )
				{
					$versionFile = $_FILES["versionFile"]["tmp_name"];
					if( $versionFile )
						$error = createFileVersion( $nextID, $versionFile, $_FILES['versionFile']['type'], $description );
				}
				
				if( !$error || !is_object( $error ) )
				{
					header("Location: index.php?ID=" . $parentid );
					exit();
				}
			}
			else
				$error = $queryResult;
		}
		else
			$error = $nextID;
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