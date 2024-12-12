<?php
	/*
		select the database type:
		MYSQL:	Connect to MySQL
		PG:		Connect to PostgresSQL
		ORA:	Connect to Oracle
	*/
	$database = "ORA";
	$database = "MYSQL";
	$database = "PG";
	
	
	/*
		connect strings für die datenbank:
	*/

	$postgresDB		= "dbname=xxxxx user=xxxxx password=xxxxx";
	$mysqlHost		= "xxxxx";
	$mysqlUser		= "xxxxx";
	$mysqlDB		= "xxxxx";
	$mysqlPassword	= "xxxxx";
	$oraUser		= "xxxxx";
	$oraPassword	= "xxxxx";
	$oraConnection	= "xxxxx";
	
	$serverName = $_SERVER["SERVER_NAME"];

	$config = "includes/tools/config." . $serverName . ".php";
	if( is_file( $config ) )
		include_once( $config );

	$config = "../includes/tools/config." . $serverName . ".php";
	if( is_file( $config ) )
		include_once( $config );
		
	/*
		application specific constants
	*/
	define( "APPLICATION_NAME", "Doc Manager Server" );
	define( "APPLICATION_COPYRIGHT", "&copy; 2012 - 2014 by <a href='http://www.cresd.de/' target='_blank'>CRESD GmbH</a>" );
	$backupTables = array( "user_tab", "user_login_prot", "group_member", "item_tree", "file_versions" );
	
	if( is_dir( "../DocManStorage" ) )
		define( "STORAGE_PATH", "../DocManStorage/" );
	else if( is_dir( "DocManStorage" ) )
		define( "STORAGE_PATH", "DocManStorage/" );
	else
		define( "STORAGE_PATH", "storage/" );
		
	define( "TYPE_FOLDER", 0 );
	define( "TYPE_DOCUMENT", 1 );
	define( "TYPE_CRYPTO_FILE", 28 );
?>
