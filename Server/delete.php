<?php include_once( "includes/components/login.php" ); ?>
<?php
	if( array_key_exists( "ID", $_GET ) )
		$id = $_GET["ID"];
	else
		$id = -1;
		
	if( $id >= 0 )
	{
		$itemData = getItemData( $id );
		if( is_array( $itemData ) )
		{
			if( canWrite( $itemData ) )
			{
				$error = deleteItem( $id );
				if( !$error )
				{
					header( "Location: index.php?ID=" . $itemData['parentid'] );
					exit();
				}
			}
			else
				$error = new errorClass( "Permission denied" );
		}
		else
			$error = $itemData;
	}
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

