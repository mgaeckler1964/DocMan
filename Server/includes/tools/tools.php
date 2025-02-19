<?php
	function getItemData( $itemid )
	{
		global $dbConnect;
		
		if( $itemid >= 0 )
		{
			$queryResult = queryDatabase( $dbConnect, "select * from item_tree where id=$1", array( $itemid ) );
			if( $queryResult && !is_object( $queryResult ) )
			{
				$itemData = fetchQueryRow( $queryResult );
				if( !$itemData )
					$itemData = new errorClass( "Item not found" );
				else if( !canRead( $itemData ) )
					$itemData = new errorClass( "Permission Denied" );
			}
			else
				$itemData = $queryResult;
		}
		else
			$itemData = false;

		return $itemData;
	}
	function getItemPath( $itemData )
	{
		if( !is_array( $itemData ) )
			$itemData = getItemData( $itemData );

		if( is_array( $itemData ) && count( $itemData ) )
		{
			if( $itemData['parentid'] >= 0 )
				$itemPath = getItemPath( $itemData['parentid'] );
			else
				$itemPath = array();

			$itemPath[] = $itemData;
		}
		else
			$itemPath = array();

		return $itemPath;
	}

	function getItemContent( $itemid )
	{
		global $dbConnect;

		if( $itemid > 0 && !canRead( $itemid ) )
			$itemContent = new errorClass( "Permission Denied" );
		else
		{
			$queryResult = queryDatabase( $dbConnect, "select * from item_tree where parentId = $1 order by name", array( $itemid ) );
			if( $queryResult && !is_object( $queryResult ) )
			{
				$itemContent = array();
				while( $record = fetchQueryRow( $queryResult ) )
					$itemContent[] = $record;
			}
			else
				$itemContent = $queryResult;
		}
		
		return $itemContent;
	}
	function getItemVersions( $itemid )
	{
		global $dbConnect;

		if( $itemid > 0 && !canRead( $itemid ) )
			$itemVersions = new errorClass( "Permission Denied" );
		else
		{
			$queryResult = queryDatabase( $dbConnect, "select * from file_versions where itemId = $1 order by id desc", array( $itemid ) );
			if( $queryResult && !is_object( $queryResult ) )
			{
				$itemVersions = array();
				while( $record = fetchQueryRow( $queryResult ) )
					$itemVersions[] = $record;
			}
			else
				$itemVersions = $queryResult;
		}
		
		return $itemVersions;
	}
	function deleteItem( $itemid )
	{
		global $dbConnect;
		
		$result = false;
		if( canWrite( $itemid ) )
		{
			$itemContent = getItemContent( $itemid );
			if( $itemContent && !is_object( $itemContent ) )
			{
				forEach( $itemContent as $record )
				{
					$result = deleteItem( $record['id'] );
					if( $result )
						break;
				}
			}
			$versionTable = getItemVersions( $itemid );
			if( is_array( $versionTable ) )
			{
				forEach( $versionTable as $versionEntry )
				{
					unlink( STORAGE_PATH . $versionEntry['id'] . ".dat" );
				}
			}
			else 
				$result = $versionTable;

			if( !$result )
			{
				$result = queryDatabase( $dbConnect, "delete from file_versions where itemid=$1", array( $itemid ) );
				if( !is_object( $result ) )
					$result = false;
			}
			if( !$result )
			{
				$result = queryDatabase( $dbConnect, "delete from item_tree where id=$1", array( $itemid ) );
				if( !is_object( $result ) )
					$result = false;
			}
		}
		else
			$result = new errorClass( "Permission Denied" );
		
		return $result;
	}
	function deleteFileVersion( $itemid, $versionID )
	{
		global $dbConnect;
		
		$result = false;
		if( canWrite( $itemid ) )
		{
			$result = queryDatabase( $dbConnect, "delete from file_versions where id=$1", array( $versionID ) );
			if( !is_object( $result ) )
			{
				$result = false;
				unlink( STORAGE_PATH . $versionID . ".dat" );
			}
		}
		else
			$result = new errorClass( "Permission Denied" );
		
		return $result;
	}
	function createFileVersion( $itemid, $versionFile, $mimeType, $description, $modTime=false )
	{
		global $dbConnect, $actUser;

		if( canWrite( $itemid ) )
		{
			$versionID = getNextID( $dbConnect, "file_versions", "id" );
			if( $versionID && !is_object( $versionID ) )
			{
				if( !$modTime )
					$modTime = time();
					
				$result = queryDatabase( 
					$dbConnect, 
					"insert into file_versions ( id, itemid, createby, mimetype, createdate, modifieddate, description ) values ( $1, $2, $3, $4, $5, $6, $7 )",
					array( $versionID, $itemid, $actUser['id'], $mimeType, time(), $modTime, $description )
				);
				if( !is_object( $result ) )
				{
					$destFile = STORAGE_PATH . $versionID . ".dat";
					if( is_uploaded_file( $versionFile ) )
						$result = move_uploaded_file( $versionFile, $destFile );
					else
						$result = rename( $versionFile, $destFile );

					if( !$result )
						$result = new errorClass( "Failed to copy file" );
					else
						$result = false;
				}
			}
			else
				$result = $versionID;
		}
		else
			$result = new errorClass( "Permission Denied" );
	
		return $result;		
	}
	function updateFileVersion( $itemid, $versionID, $mimeType, $description )
	{
		global $dbConnect, $actUser;

		if( canWrite( $itemid ) )
		{
			$result = queryDatabase( 
				$dbConnect, 
				"update file_versions set mimetype = $1, description = $2 where id = $3",
					array( $mimeType, $description, $versionID )
				);
			if( !is_object( $result ) )
				$result = false;
		}
		else
			$result = new errorClass( "Permission Denied" );
	
		return $result;		
	}
	function getFileVersion( $itemid, $versionID )
	{
		global $dbConnect;
		
		if( $itemid > 0 && !canRead( $itemid ) )
			$result = new errorClass( "Permission Denied" );
		else
		{
			if( $versionID <= 0 )
				
				$queryResult = queryDatabase( 
					$dbConnect, 
					"select * ".
					"from file_versions ".
					"where itemId = $1 ".
					"and id = ( ".
						"select max( id ) from file_versions where itemid = $2 ".
					")",
					array( $itemid, $itemid )
				);
			else
				$queryResult = queryDatabase( 
					$dbConnect, 
					"select * ".
					"from file_versions ".
					"where itemId = $1 and id = $2",
					array( $itemid, $versionID )
				);
				
			if( $queryResult && !is_object($queryResult) )
				$result = fetchQueryRow( $queryResult );
			else
				$result = $queryResult;
		}

		return $result;
	}

	function reserve( $itemData )
	{
		global $dbConnect, $actUser;

		if( is_array( $itemData ) || $itemData >= 0 )
		{
			if( !is_array( $itemData ) )
				$itemData = getItemData( $itemData );

			if( is_array( $itemData ) )
			{
				if( canReserve( $itemData ) )
				{
					queryDatabase( 
						$dbConnect, 
						"update item_tree set reservedby = $1 where id = $2", 
						array( $actUser['id'], $itemData['id'] )
					);
					$result = false;
				}
				else
					$result = new errorClass( "Permission Denied" );
			}
			else
				$result = $itemData;
		}
		else
			$result = new errorClass( "Bad data" );

		return $result;
	}
	function unreserve( $itemData )
	{
		global $dbConnect, $actUser;

		if( is_array( $itemData ) || $itemData >= 0 )
		{
			if( !is_array( $itemData ) )
				$itemData = getItemData( $itemData );

			if( is_array( $itemData ) )
			{
				if( canUnreserve( $itemData ) )
				{
					queryDatabase( 
						$dbConnect, 
						"update item_tree set reservedby = null where id = $1", 
						array( $itemData['id'] )
					);
					$result = false;
				}
				else
					$result = new errorClass( "Permission Denied" );
			}
			else
				$result = $itemData;
		}
		else
			$result = new errorClass( "Bad data" );

		return $result;
	}

	function canRead( $itemData )
	{
		global $actUser;

		if( $actUser['administrator'] )
			$result = true;
		else
		{
			$userId = $actUser['id'];
			$result = false;
			if( !is_array( $itemData ) )
			{
				$itemData = getItemData( $itemData );
				if( is_array( $itemData ) && count( $itemData ) )
					$result = true;
			}
			else if( count( $itemData ) )
			{
				if( $userId == $itemData['owneruser'] && ($itemData['mode'] & 0400) )
					$result = true;
				else if( $itemData['mode'] & 0004 )
					$result = true;
				else if( $itemData['mode'] & 0040 )
				{
					$memberShips = getAllMemberships( $userId );
					if( is_array( $memberShips ) && array_search( $itemData['ownergroup'], $memberShips ) !== false )
						$result = true;
				}
			}
		}

		return $result;
	}
	function canWrite( $itemData )
	{
		global $actUser;

		if( $actUser['administrator'] )
			$result = true;
		else if( is_array( $itemData ) || $itemData >= 0 )
		{
			$userId = $actUser['id'];
			$result = false;
			if( !is_array( $itemData ) )
				$itemData = getItemData( $itemData );

			if( is_array( $itemData ) )
			{
				if( $itemData['reservedby'] )
					$result = ($itemData['reservedby'] == $userId);
				else if( $userId == $itemData['owneruser'] && ($itemData['mode'] & 0200) )
					$result = true;
				else if( $itemData['mode'] & 0002 )
					$result = true;
				else if( $itemData['mode'] & 0020 )
				{
					$memberShips = getAllMemberships( $userId );
					if( is_array( $memberShips ) && array_search( $itemData['ownergroup'], $memberShips ) !== false )
						$result = true;
				}
				
			}
		}
		else
			$result = false;

		return $result;
	}
	function canReserve( $itemData )
	{
		global $actUser;

		if( is_array( $itemData ) || $itemData >= 0 )
		{
			if( !is_array( $itemData ) )
				$itemData = getItemData( $itemData );

			if( is_array( $itemData ) && count( $itemData ) && !$itemData['reservedby'] && $itemData['item_type'] != TYPE_FOLDER )
				$result = canWrite( $itemData );
			else
				$result = false;
		}
		else
			$result = false;

		return $result;
	}
	function canUnReserve( $itemData )
	{
		global $actUser;

		if( is_array( $itemData ) || $itemData >= 0 )
		{
			$userID = $actUser['id'];
			if( !is_array( $itemData ) )
				$itemData = getItemData( $itemData );

			if( is_array( $itemData ) && count( $itemData ) )
			{
				if( $itemData['reservedby'] == $userID ) 
					$result = true;
				else if( $itemData['reservedby'] && $actUser['administrator'] )
					$result = true;
				else
					$result = false;
			}
			else
				$result = false;
		}
		else
			$result = false;

		return $result;
	}
?>