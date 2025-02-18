<?php include_once( "includes/components/login.php" ); ?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Strict//EN">

<html>
	<head>
		<?php
			$title = "Reservierung";
			include_once( "includes/components/defhead.php" );
		?>
	</head>
	<body>
		<?php 
			include( "includes/components/headerlines.php" ); 

			$id=$_GET['ID'];
			$itemData = getItemData( $id );
			if( is_array( $itemData ) && count( $itemData ) )
				if( !$itemData['reservedby'] )
					$error = reserve( $itemData );
				else
					$error = unreserve( $itemData );
			else
				$error = $itemData;
				
			if( $error !== false )
				include( "includes/components/error.php" );
			else
			{
				echo( "<p>Daten erfolgreich gespeichert</p>" );
				echo( "<p><a href='index.php?ID={$itemData['parentID']}'>&gt;&gt;&nbsp;Weiter</p>" );
			}
	

			include( "includes/components/footerlines.php" );
		?>
	</body>
</html>
		
