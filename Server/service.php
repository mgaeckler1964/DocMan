<?php
	require_once( "includes/tools/config.php" );
	require_once( "includes/tools/database.php" );
	require_once( "includes/tools/commontools.php" );
	require_once( "includes/tools/tools.php" );

	if( isset( $_SERVER['REDIRECT_HTTP_AUTHORIZATION'] ) )
	{
		$tmpArray =	explode(':', base64_decode(substr($_SERVER['REDIRECT_HTTP_AUTHORIZATION'], 6)));
		list($_SERVER['PHP_AUTH_USER'], $_SERVER['PHP_AUTH_PW']) = $tmpArray;
	}
  	
  	
  	if( !isset($_SERVER['PHP_AUTH_USER']) 
	||  !isset($_SERVER['PHP_AUTH_PW']) )
	{
    	header('WWW-Authenticate: Basic realm="DocMan Service"');
	    header('HTTP/1.0 401 Unauthorized');
	    echo( "<pre>" );
/*
	    var_dump( $_SERVER );
	    var_dump($_SERVER['REDIRECT_HTTP_AUTHORIZATION']);
	    var_dump(substr($_SERVER['REDIRECT_HTTP_AUTHORIZATION'], 6));
	    var_dump(base64_decode(substr($_SERVER['REDIRECT_HTTP_AUTHORIZATION'], 6)));
	    var_dump(explode(':', base64_decode(substr($_SERVER['REDIRECT_HTTP_AUTHORIZATION'], 6))));
*/

    	echo 'Please login, to proceed';
//	    var_dump($tmpArray);
	    echo( "</pre>" );
    	exit();
	} 

	$dbConnect = openDatabase();
	if( !$dbConnect )
	{
		echo "Database Error";
		exit();
	}
	
	if( array_key_exists( "lastUserID", $_COOKIE ) )
		$lastUserID = $_COOKIE["lastUserID"];
	else
		$lastUserID = -1;

	$actUser = loginUser( $dbConnect, $_SERVER['PHP_AUTH_USER'], $_SERVER['PHP_AUTH_PW'], $lastUserID );
	if( is_string( $actUser ) )
	{
    	header('WWW-Authenticate: Basic realm="DocMan Service"');
	    header('HTTP/1.0 401 Unauthorized');

	    echo( "<pre>" );
//	    var_dump( $_SERVER );
//	    var_dump($tmpArray);
    	echo 'Please login, to proceed, bad user or password';
	    echo( "</pre>" );
    	exit();
	}

	if( $lastUserID != $actUser['id'] )
		setcookie( "lastUserID", $actUser['id'], 0, "/" );

	ini_set( "display_errors", "off" );
	$debug = false;

	//call library 
	require_once ('lib/nusoap.php'); 
	//using soap_server to create server object 

	$server = new soap_server(); 

	$namespace = "urn:mgDocMan";
	$serviceName = "DocManService";
	$callType = "rpc";
	$callType = "document";
	
	$server->wsdl->schemaTargetNamespace = $namespace;

	$server->configureWSDL( $serviceName, $namespace );
	

	// create the functions 
	function sGetUserTable(  )
	{
		global $dbConnect, $actUser;

		if( !$actUser['administrator'] )
			$userTab = new errorClass( "Permission denied" );
		else
			$userTab = getDbUserTable();
		
		if( is_array( $userTab ) )
			return array( "theUserTable" => $userTab );
		else if( is_object( $userTab ) )
			return new nusoap_fault('Client','DocMan', $userTab->errorText, $userTab->errorDetail ); 
		else
			return new nusoap_fault('Client','DocMan', "Unkown Error" ); 
	} 
	function sUpdateUser( $theUser )
	{
		global $dbConnect, $database, $actUser;

		if( !$actUser['administrator'] )
			$error = new errorClass( "Permission denied" );
		else
		{
			if( $theUser['id'] )
			{
				$queryResult = queryDatabase( 
					$dbConnect,
					"update user_tab " .
					"set nachname = $1," .
						"vorname = $2," .
						"strasse = $3," .
						"postfach = $4," .
						"land = $5," .
						"plz = $6," .
						"ort = $7, " .
						"email = $8, " .
						"administrator = $9, " .
						"guest = $10, ".
						"loginEnabled = $11 " .
					"where id = $12",
					array( 
						$theUser['nachname'], $theUser['vorname'],
						$theUser['strasse'], $theUser['postfach'],
						$theUser['land'], $theUser['plz'], $theUser['ort'],
						$theUser['email'],
						$theUser['administrator'], $theUser['guest'], $theUser['loginEnabled'],
						$theUser['id']
					)
				);
				if( !$queryResult || is_object($queryResult) )
					$error = $queryResult;
			}
			else
			{
				$id = getNextID( $dbConnect, "user_tab", "id" );
				if( is_numeric( $id ) )
				{
					$result = queryDatabase( $dbConnect,
						"insert into user_tab (" .
							"id, nachname, vorname, strasse, postfach, land, plz, ort, email, administrator, guest, loginEnabled " .
						")" .
						"values" .
						"(" .
							"$1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12" .
						")",
						array( 
							$id,
							$theUser['nachname'], $theUser['vorname'],
							$theUser['strasse'], $theUser['postfach'],
							$theUser['land'], $theUser['plz'], $theUser['ort'],
							$theUser['email'],
							$theUser['administrator'], $theUser['guest'], $theUser['loginEnabled']
						)
					);
					if( is_object( $result ) )
						$error = $result;
					else
						$theUser['id'] = $id;
				}
				else
					$error = $id;
			}
		}
		if( isset( $error ) && is_object( $error ) )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "theUserID" => $theUser['id'] );
	} 
	function sDeleteUser( $theUserID )
	{
		global $dbConnect, $actUser;

		$error = deleteUser( $theUserID );

		if( isset( $error ) && is_object( $error ) )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "theUserID" => $theUserId );
	} 


	function sGetItemContent( $itemID )
	{
		global $dbConnect, $actUser;

		$itemContent = getItemContent( $itemID );
		
		if( is_array( $itemContent ) )
			return array( "theItemContent" => $itemContent );
		else if( is_object( $itemContent ) )
			return new nusoap_fault('Client','DocMan', $itemContent->errorText, $itemContent->errorDetail ); 
		else
			return new nusoap_fault('Client','DocMan', "Unkown Error" ); 
	} 

	function sUpdateItem( $itemData )
	{
		global $dbConnect, $actUser;
		
		$error = false;

		if( canWrite( $itemData ) )
		{
			$itemID = $itemData['id'];

			if( $itemID )
				$queryResult = queryDatabase( 
					$dbConnect, 
					"update item_tree set name = $1, description = $2, modifieddate = $3, ownerUser = $4, ownerGroup = $5, mode=$6 where id=$7", 
					array( $itemData['name'], $itemData['description'], time(), $itemData['ownerUser'], $itemData['ownerGroup'], $itemData['mode'], $itemID )
				);
			else
			{
				$itemID = getNextID( $dbConnect, "item_tree", "id" );
				if( is_numeric( $itemID ) )
				{
					$parentData = getItemData( $itemData['parentID'] );
					if( !$parentData )
					{
						$parentData = array();
						$parentData['mode'] = 0600;
						$parentData['ownerUser'] = $actUser['id'];
						$parentData['ownerGroup'] = 0;
					}
					if( is_array( $parentData ) )
					{
						$queryResult = queryDatabase( 
							$dbConnect, 
							"insert into item_tree ".
							"( id, parentID, item_type, name, description, createdate, modifieddate, ownerUser, ownerGroup, mode, createBy ) ".
							"values ".
							"( $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11 )", 
							array( 
								$itemID, $itemData['parentID'], 
								$itemData['item_type'], $itemData['name'], $itemData['description'], 
								time(), time(), 
								$parentData['ownerUser'], $parentData['ownerGroup'], $parentData['mode'], 
								$actUser['id'] 
							)
						);
					}
					else
						$queryResult = $parentData;
				}
				else
					$queryResult = $itemID;
			}
			if( is_object( $queryResult ) )
				$error = $queryResult;
//				$error = new errorClass( "Cannot Update", $itemData );
		}
		else
			$error = new errorClass( "Permission denied" );

		if( $error )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "theItemID" => $itemID );
	}

	function sDeleteItem( $itemID )
	{
		global $dbConnect;

		$error = false;
		$itemData = getItemData( $itemID );
		if( is_array( $itemData ) )
		{
			if( count( $itemData ) )
			{
				if( canWrite( $itemData ) )
				{
					$itemID = $itemData['id'];
					$error = deleteItem( $itemID );
				}
				else
					$error = new errorClass( "Permission denied" );
			}
		}

		if( $error )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "theItemID" => $itemID );
	}

	function sAddVersion( $itemID, $mimeType, $description, $encodedData, $modTime )
	{
		$error = false;

		if( canWrite( $itemId ) )
		{
			$location = STORAGE_PATH . "upload.tmp";		// Mention where to upload the file
			$current = file_get_contents($location);		// Get the file content. This will create an empty file if the file does not exist     
			$current = base64_decode($encodedData);			// Now decode the content which was sent by the client     
			file_put_contents($location, $current);			// Write the decoded content in the file mentioned at particular location      
			
			$error = createFileVersion( $itemID, $location, $mimeType, $description, $modTime );
		}
		else
			$error = new errorClass( "Permission denied" );

		if( $error )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "theItemID" => $itemID );
	}
	function sUpdateVersion( $itemID, $versionID, $mimeType, $description )
	{
		$error = updateFileVersion( $itemID, $versionID, $mimeType, $description );

		if( $error )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "theItemID" => $itemID );
	}
	function sDeleteVersion( $itemID, $versionID )
	{
		$error = deleteFileVersion( $itemID, $versionID );

		if( $error )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "theItemID" => $itemID );
	}
	function sGetVersions( $itemID, $versionID )
	{
		if( $versionID >= 0 )
		{
			$itemVersions = getFileVersion( $itemID, $versionID );
			if( !$itemVersions )
				$itemVersions = array();
			else if( !is_object($itemVersions) )
				$itemVersions = array( $itemVersions );
		}
		else
			$itemVersions = getItemVersions( $itemID );
			
		if( is_object( $itemVersions ) )
			return new nusoap_fault('Client','DocMan', $itemVersions->errorText, $itemVersions->errorDetail ); 
		else
			return array( "theItemVersions" => $itemVersions );
	}

	function sCanReserve( $itemID )
	{
		if( canReserve( $itemID ) )
			return array( "canReserve" => 1 );
		else
			return array( "canReserve" => 0 );
	}
	function sCanUnreserve( $itemID )
	{
		if( canUnreserve( $itemID ) )
			return array( "canUnreserve" => 1 );
		else
			return array( "canUnreserve" => 0 );
	}
	function sReserve( $itemID )
	{
		global $actUser;

		$error = reserve( $itemID );
		if( is_object( $error ) )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "reservedBy" => $actUser['id'] );
	}
	function sUnreserve( $itemID )
	{
		$error = unreserve( $itemID );
		if( is_object( $error ) )
			return new nusoap_fault('Client','DocMan', $error->errorText, $error->errorDetail ); 
		else
			return array( "error" => 0 );
	}
	//Create the complex types
	$server->wsdl->addComplexType(
		'UserRecord','complexType','struct','sequence','',
		array(
			'id' => array('name' => 'id','type' => 'xsd:int'),
			'nachname' => array('name' => 'nachname','type' => 'xsd:string'),
			'vorname' => array('name' => 'vorname','type' => 'xsd:string'),
			'strasse' => array('name' => 'strasse','type' => 'xsd:string'),
			'postfach' => array('name' => 'postfach','type' => 'xsd:string'),
			'land' => array('name' => 'land','type' => 'xsd:string'),
			'plz' => array('name' => 'plz','type' => 'xsd:string'),
			'ort' => array('name' => 'ort','type' => 'xsd:string'),
			'email' => array('name' => 'email','type' => 'xsd:string'),
			'password' => array('name' => 'password','type' => 'xsd:string'),
			'guest' => array('name' => 'guest','type' => 'xsd:string'),
			'loginEnabled' => array('name' => 'loginEnabled','type' => 'xsd:string'),
			'administrator' => array('name' => 'administrator','type' => 'xsd:string'),
			'is_group' => array('name' => 'is_group','type' => 'xsd:string')
		)
	);
	$server->wsdl->addComplexType(
		'UserTable','complexType','array','all', '' // 'SOAP-ENC:Array'
		, array( 'item' => array( 'name' => 'item', 'type' => 'UserRecord' ) )
		, array()
		,'UserRecord'
	);

	$server->wsdl->addComplexType(
		'ItemRecord','complexType','struct','sequence','',
		array(
			'id' => array('name' => 'id','type' => 'xsd:int'),
			'parentID' => array('name' => 'parentID','type' => 'xsd:int'),
			'item_type' => array('name' => 'item_type','type' => 'xsd:int'),
			'name' => array('name' => 'name','type' => 'xsd:string'),
			'description' => array('name' => 'description','type' => 'xsd:string'),
			'ownerUser' => array('name' => 'ownerUser','type' => 'xsd:int'),
			'ownerGroup' => array('name' => 'ownerGroup','type' => 'xsd:int'),
			'mode' => array('name' => 'mode','type' => 'xsd:int'),
			'createBy' => array('name' => 'createBy','type' => 'xsd:int'),
			'createdate' => array('name' => 'createdate','type' => 'xsd:int'),
			'modifieddate' => array('name' => 'modifieddate','type' => 'xsd:int'),
			'reservedBy' => array('name' => 'reservedBy','type' => 'xsd:int')
		)
	);
	$server->wsdl->addComplexType(
		'ItemContent','complexType','array','all', '' // 'SOAP-ENC:Array'
		, array( 'item' => array( 'name' => 'item', 'type' => 'ItemRecord' ) )
		, array()
		,'ItemRecord'

	);

	$server->wsdl->addComplexType(
		'VersionRecord','complexType','struct','sequence','',
		array(
			'id' => array('name' => 'id','type' => 'xsd:int'),
			'itemID' => array('name' => 'itemID','type' => 'xsd:int'),
			'createBy' => array('name' => 'createBy','type' => 'xsd:int'),
			'mimeType' => array('name' => 'mimeType','type' => 'xsd:string'),
			'description' => array('name' => 'description','type' => 'xsd:string'),
			'createdate' => array('name' => 'createdate','type' => 'xsd:int'),
			'modifieddate' => array('name' => 'modifieddate','type' => 'xsd:int')
		)
	);
	$server->wsdl->addComplexType(
		'VersionTable','complexType','array','all', '' // 'SOAP-ENC:Array'
		, array( 'item' => array( 'name' => 'item', 'type' => 'VersionRecord' ) )
		, array()
		,'VersionRecord'

	);
	//Register our methods
	$server->register(
		// method name:
		'sGetUserTable',
		// parameter list:
		array(),
		// return value(s):
		array('theUserTable'=>'UserTable'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Get Table Of Users'
	);

	$server->register(
		// method name:
		'sUpdateUser',
		// parameter list:
		array( 'theUser' => 'UserRecord' ),
		// return value(s):
		array('theUserID'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Create or update a User'
	);
	$server->register(
		// method name:
		'sDeleteUser',
		// parameter list:
		array( 'theUserID' => 'xsd:int' ),
		// return value(s):
		array('theUserID'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'delete a User'
	);

	$server->register(
		// method name:
		'sGetItemContent',
		// parameter list:
		array( 'itemID' => 'xsd:int' ),
		// return value(s):
		array('theItemContent'=>'ItemContent'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Get Table Of Content'
	);
	$server->register(
		// method name:
		'sUpdateItem',
		// parameter list:
		array( 'itemData' => 'ItemRecord' ),
		// return value(s):
		array('theItemID'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Update an existing Item'
	);
	$server->register(
		// method name:
		'sDeleteItem',
		// parameter list:
		array( 'itemID' => 'xsd:int' ),
		// return value(s):
		array('theItemID'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Delete an existing Item'
	);
	$server->register(
		// method name:
		'sAddVersion',
		// parameter list:
		array( 'itemID' => 'xsd:int', 'mimeType' => 'xsd:string', 'description' => 'xsd:string', 'encodedData' => 'xsd:string', 'modTime' => 'xsd:int' ),
		// return value(s):
		array('theVersionID'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Add a version to an existing Item'
	);

	$server->register(
		// method name:
		'sUpdateVersion',
		// parameter list:
		array( 'itemID' => 'xsd:int', 'versionID' => 'xsd:int', 'mimeType' => 'xsd:string', 'description' => 'xsd:string' ),
		// return value(s):
		array('theVersionID'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Update a version of an existing Item'
	);
	$server->register(
		// method name:
		'sDeleteVersion',
		// parameter list:
		array( 'itemID' => 'xsd:int', 'versionID' => 'xsd:int' ),
		// return value(s):
		array('theVersionID'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Delete a version of an existing Item'
	);

	$server->register(
		// method name:
		'sGetVersions',
		// parameter list:
		array( 'itemID' => 'xsd:int', 'versionID' => 'xsd:int' ),
		// return value(s):
		array('theItemVersions'=>'VersionTable'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'Get Versionlist of a File'
	);
	$server->register(
		// method name:
		'sCanReserve',
		// parameter list:
		array( 'itemID' => 'xsd:int' ),
		// return value(s):
		array('canReserve'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'check a file can be reserved'
	);
	$server->register(
		// method name:
		'sCanUnreserve',
		// parameter list:
		array( 'itemID' => 'xsd:int' ),
		// return value(s):
		array('canUnreserve'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'check a file can be unreserved'
	);
	$server->register(
		// method name:
		'sReserve',
		// parameter list:
		array( 'itemID' => 'xsd:int' ),
		// return value(s):
		array('reservedBy'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'reserve a file'
	);
	$server->register(
		// method name:
		'sUnreserve',
		// parameter list:
		array( 'itemID' => 'xsd:int' ),
		// return value(s):
		array('error'=>'xsd:int'),
		// namespace:
		$namespace,
		// soapaction: (use default)
		false,
		// style: rpc or document
		$callType,
		// use: encoded or literal
		'encoded',
		// description: documentation for the method
		'unreserve a file'
	);

	// create HTTP listener 
	$postdata = file_get_contents("php://input");
	$server->service($postdata);
?>  