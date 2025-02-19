<?php
	include_once( "includes/components/login.php" );

	if( !isset( $error ) )
	{
		if( array_key_exists( "ID", $_GET ) )
			$id = $_GET["ID"];
		else
			$id = -1;
			
		if( $id=="" )
			$id = -1;

		if( $id >= 0 )
		{
			$itemData = getItemData( $id );
			if( !is_array( $itemData ) || !count($itemData) )
				$error = $itemData;
		}
	}
	if( !isset( $error ) )
	{
		$itemContent = getItemContent( $id );
		if( !is_array( $itemContent ) )
			$error = $itemContent;
	}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Strict//EN">

<html>
	<head>
		<?php
			$title = APPLICATION_NAME;
			include_once( "includes/components/defhead.php" );
		?>
	</head>
	<body>
		<?php
			include( "includes/components/headerlines.php" );

			if( !isset( $error ) )
			{
				if( is_file( "templates/index.html" ) )
					include_once( "templates/index.html" );
	
				if( $id >= 0 )
				{
					echo( "<p><a href='index.php?ID={$itemData['parentid']}'>Nach oben</a></p>\n" );
					$itemPath = getItemPath( $itemData );
					if( count( $itemPath ) )
					{
						array_splice ( $itemPath, -1 );
						echo( "<p><a href='index.php'>&gt;&gt;&nbsp;Home</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n" );
						forEach( $itemPath as $parentData )
							echo( "<a href='index.php?ID={$parentData['id']}'>&gt;&gt;&nbsp;{$parentData['name']}</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n" );
						echo( "<a href='#_top'>&gt;&gt;&nbsp;{$itemData['name']}</a>\n" );
						echo( "</p>" );
					}
				}
					
				echo( "<table>" );
				echo( "<tr><th>Typ</th><th>Name</th><th>Erstellt am</th><th>Geändert am</th><th>Funktion</th></tr>\n" );
				foreach( $itemContent as $record )
				{
					if( canRead( $record ) )
					{
						echo( "<tr>" );	
						echo( "<td>" );
						if( $record['reservedby'] )
							echo( "<img src='support/reserved.gif'>" );
						if( $record['item_type'] == 0 )
							echo( "<img src='support/folder.gif'>" );
						else if( $record['item_type'] == 1 )
							echo( "<img src='support/doc.gif'>" );
						else
							echo( "<img src='support/crypto_file.gif'>" );
						
						echo( "</td>" );
						if( $record['item_type'] == 0 )
							$viewer='index.php';
						else if( $record['item_type'] == 1 )
							$viewer='viewDoc.php';
						else
							$viewer=false;
						
						if( $record['createdate'] )
							$record['createdate'] = formatTimeStamp($record['createdate']);
	
						if( $record['modifieddate'] )
							$record['modifieddate'] = formatTimeStamp($record['modifieddate']);
	
						if( $viewer )
							echo( "<td><a href='{$viewer}?ID={$record['id']}'>{$record['name']}</a></td>" );
						else
							echo( "<td>{$record['name']}</td>" );
						echo( "<td>{$record['createdate']}</td>" );
						echo( "<td>{$record['modifieddate']}</td>" );
						echo( "<td>" );
						echo( "<a href='general.php?ID={$record['id']}'>&gt;&gt;&nbsp;General</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" );
						if( canWrite( $record ) )
						{
							echo( "<a href='delete.php?ID={$record['id']}' onClick='if( confirm( \"Wirklich?\" ) ) return true; else return false;'>&gt;&gt;&nbsp;Löschen</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" );
							echo( "<a href='reserve.php?ID={$record['id']}'>&gt;&gt;&nbsp;" );
							if( $record['reservedby'] )
								echo( "Reservierung aufheben" );
							else
								echo( "Reservieren" );
							echo( "</a>" );
						}
						echo( "</td>" );
						echo( "</tr>\n" );
					}
				}
				echo( "</table>" );

				if( canWrite( $id ) )
					echo( "<p><a href='create.php?parentid=$id&itemType=0'>&gt;&gt;&nbsp;Neuer Ordner</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='create.php?parentid=$id&itemType=1'>&gt;&gt;&nbsp;Neue Datei</a></p>\n" );
			}
			else
			{
				echo( "<p>Kann Tabelle nicht lesen.</p>" );
				include "includes/components/error.php";
			}

			include( "includes/components/footerlines.php" );
		 ?>
	</body>
</html>
		
