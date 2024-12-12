<?php require_once( "includes/components/login.php" ); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Strict//EN">

<html>
	<head>
		<?php
			$title = "Tabellen Erstellen";
			include_once( "includes/components/defhead.php" );
		?>
	</head>
	<body>
		<?php include( "includes/components/headerlines.php" ); ?>
		
		<?php
			require_once("includes/tools/createUserTables.php" );

			$dbConnect = openDatabase();
			if( !$dbConnect )
				echo "<p>Kann keine Verbindung zur Datenbank herstellen.</p>";
			else
			{
				echo "<p>Verbindung zur Datenbank OK.</p>";
				$error = createUserTables( $dbConnect, $database );

				$query = "create table item_tree ( ".
					"id				int				not null	primary key, ".
					"parentID		int				not null, ".
					"item_type		int				not null, ".
					"name			varchar(255), ".
					"description	varchar(4000), ".
					"ownerUser		int				not null references user_tab, ".
					"ownerGroup		int				not null references user_tab, ".
					"mode			int, ".
					"createBy		int				not null references user_tab, ".
					"createDate		int, ".
					"modifiedDate	int, ".
					"reservedBy		int ".
				")";
	
				$result = queryDatabase( $dbConnect, $query );
				if( !$result || is_object( $result ) )
				{
					$error .= "<p>item_tree konnte nicht erstellt werden.</p>\n";
					if( is_object( $result ) )
						$error .= "<p>". $result->errorText . "<br>" . $result->errorDetail . "</p>";
				}
	
				$query = "create unique index item_parentIdx on item_tree (parentID, name )";
				$result = queryDatabase( $dbConnect, $query );
				if( !$result || is_object( $result ) )
				{
					$error .= "<p>item_parentIdx konnte nicht erstellt werden.</p>\n";
					if( is_object( $result ) )
						$error .= "<p>". $result->errorText . "<br>" . $result->errorDetail . "</p>";
				}

				$query = "create table file_versions ( ".
					"id				int				not null	primary key, ".
					"itemID			int				not null	references item_tree, ".
					"createBy		int				not null	references user_tab, ".
					"mimeType		varchar(255), ".
					"description	varchar(4000), ".
					"createDate		int, ".
					"modifiedDate	int ".
				")";
				$result = queryDatabase( $dbConnect, $query );
				if( !$result || is_object( $result ) )
				{
					$error .= "<p>file_versions konnte nicht erstellt werden.</p>\n";
					if( is_object( $result ) )
						$error .= "<p>". $result->errorText . "<br>" . $result->errorDetail . "</p>";
				}

				$query = "alter table file_versions add ( ".
					"description	varchar(4000) ".
				")";
				$result = queryDatabase( $dbConnect, $query );
				if( !$result || is_object( $result ) )
				{
					$error .= "<p>file_versions konnte nicht aktualisiert werden.</p>\n";
					if( is_object( $result ) )
						$error .= "<p>". $result->errorText . "<br>" . $result->errorDetail . "</p>";
				}

				echo( $error );
			}
		?>

		<p>Tabellenerstellung fertig.</p>

		<?php include( "includes/components/footerlines.php" ); ?>
	</body>
</html>