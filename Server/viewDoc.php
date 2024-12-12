<?php include_once( "includes/components/login.php" ); ?>
<?php
	$itemID = $_GET['ID'];
	if( array_key_exists( 'versionID', $_GET ) )
		$versionID = $_GET['versionID'];
	else
		$versionID = -1;
	
	$itemData = getItemData( $itemID );
	if( is_array( $itemData ) )
	{
		$version = getFileVersion( $itemID, $versionID );
		if( is_array( $version ) )
		{
			$fileName = STORAGE_PATH . $version['id'] .".dat";
		
			header('Content-Type: ' . $version['mimeType'] );
			header('Content-Disposition: attachment; filename='. $itemData['name'] );
			header('Content-Transfer-Encoding: binary');
			header('Content-Length: ' . filesize($fileName));
			
			readFile( $fileName );
			exit();
		}
		else
			$error = $version;
	}
	else
		$error = $itemData;
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
			include( "includes/components/footerlines.php" );
		?>
	</body>
</html>

