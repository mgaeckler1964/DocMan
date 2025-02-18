<?php 
	include_once( "includes/components/login.php" );

	if( !isset( $error ) )
	{
		$itemID = $_GET['ID'];
		$itemData = getItemData( $itemID );
		if( is_array( $itemData ) )
		{
			$itemType = $itemData['item_type'];
			if( $itemType == 1 )
			{
				$versionTable = getItemVersions( $itemID );
				if( !is_array( $versionTable ) )
					$error = $versionTable;
			}
		}
		else
			$error = $itemData;
	}
	if( !isset( $error ) )
	{
		if( !canRead( $itemData ) )
			$error = new errorClass( "Permission denied" );
	}
	if( !isset( $error ) )
	{
		$userList = getAllUserIDs();
		if( !is_array( $userList ) )
			$error = $userList;
	}
	if( $itemData['reservedby'] )
	{
		$reserver = getUser( $dbConnect, $itemData['reservedby'] );
		if( !is_array( $reserver ) || !count( $reserver ) )
			$error = $reserver;
	}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Strict//EN">
<html>
	<head>
		<?php
			$title = APPLICATION_NAME . " - Generele Information";
			include_once( "includes/components/defhead.php" );
		?>
	</head>
	<body>
		<?php include( "includes/components/headerlines.php" ); ?>
		<?php if( !isset( $error ) ) { ?>
			<form action="general2.php" method="post" enctype="multipart/form-data">
				<input type="hidden" name="ID" value="<?php echo $itemID; ?>">
				<input type="hidden" name="parentID" value="<?php echo $itemData["parentID"]; ?>">
				<table>
					<tr><td class="fieldLabel">ID</td><td><?php echo $itemID; ?></td></tr>
					<tr><td class="fieldLabel">Name</td><td><input type="text" name="itemName" required="required" value="<?php echo $itemData["name"]; ?>"></td></tr>
					<tr><td class="fieldLabel">Beschreibung</td><td><input type="text" name="description" value="<?php echo $itemData["description"]; ?>"></td></tr>
					<?php
						if( $itemData['reservedby'] )
							echo( "<tr><td class='fieldLabel'>Reserviert für</td><td>{$reserver["fullname"]}</td></tr>\n" );
					?>
					<tr><td class="fieldLabel">Eigentümer</td><td><select name="ownerUser">
						<?php
							forEach( $userList as $theUser )
							{
								if( !$theUser['is_group'] )
								{
									if( $theUser['id'] == $itemData['ownerUser'] )
										echo( "<option value='{$theUser['id']}' SELECTED>{$theUser['email']}</option>" );
									else
										echo( "<option value='{$theUser['id']}'>{$theUser['email']}</option>" );
								}
							}
						?>
						</select></td>
						<td>
							<?php
								if( $itemData['mode'] & 0400 )
									$checked = "checked";
								else
									$checked = "";
								echo( "<input type='checkbox' name='userReadPerm' value='0400' $checked> Lesen " );
								if( $itemData['mode'] & 0200 )
									$checked = "checked";
								else
									$checked = "";
								echo( "<input type='checkbox' name='userWritePerm' value='0200' $checked> Schreiben " );
							?>
						</td>
						</tr>
						<tr><td class="fieldLabel">Gruppe</td><td><select name="ownerGroup">
						<?php
							forEach( $userList as $theUser )
							{
								if( $theUser['is_group'] )
								{
									if( $theUser['id'] == $itemData['ownerGroup'] )
										echo( "<option value='{$theUser['id']}' SELECTED>{$theUser['email']}</option>" );
									else
										echo( "<option value='{$theUser['id']}'>{$theUser['email']}</option>" );
								}
							}
						?>
						</select></td>
						<td>
							<?php
								if( $itemData['mode'] & 0040 )
									$checked = "checked";
								else
									$checked = "";
								echo( "<input type='checkbox' name='groupReadPerm' value='0040' $checked> Lesen " );
								if( $itemData['mode'] & 0020 )
									$checked = "checked";
								else
									$checked = "";
								echo( "<input type='checkbox' name='groupWritePerm' value='0020' $checked> Schreiben " );
							?>
						</td>
					</tr>
					<tr>
						<td class="fieldLabel">Öffentlich</td>
						<td></td>
						<td>
							<?php
								if( $itemData['mode'] & 0004 )
									$checked = "checked";
								else
									$checked = "";
								echo( "<input type='checkbox' name='otherReadPerm' value='0004' $checked> Lesen " );
								if( $itemData['mode'] & 0002 )
									$checked = "checked";
								else
									$checked = "";
								echo( "<input type='checkbox' name='otherWritePerm' value='0002' $checked> Schreiben " );
							?>
						</td>
					</tr>
							
					
					<?php if( $itemType == 1 ) { ?>
						<tr><td class="fieldLabel">Datei</td><td><input type="file" name="versionFile"></td></tr>
					<?php } ?>
					<tr><td class="fieldLabel">&nbsp;</td><td>&nbsp;</td></tr>
					<tr>
						<td class="fieldLabel">&nbsp;</td>
						<td>
							<?php if( canWrite( $itemData ) ) { ?>
								<input type='submit' value='Ändern'>
							<?php } ?>
							<input type='button' onClick='window.history.back();' value='Abbruch'>
						</td>
					</tr>
				</table>
			</form>
		<?php } ?>
		<?php
			if( !isset( $error ) && $itemType == 1 )
			{
				echo( "<hr><table>" );
				echo( "<tr><th>Version</th><th>Erstellt</th><th>Geändert</th><th>Mimetype</th></tr>" );
				$i = count( $versionTable );
				forEach( $versionTable as $record )
				{
					if( $record['createdate'] )
						$record['createdate'] = formatTimeStamp($record['createdate']);

					if( $record['modifieddate'] )
						$record['modifieddate'] = formatTimeStamp($record['modifieddate']);
	
					echo( "<tr><td><a href='viewDoc.php?ID={$record['itemID']}&versionID={$record['id']}'>" . $i . "</td><td>" . $record['createdate'] . "</td><td>" . $record['modifieddate'] . "</td><td>" . $record['mimetype'] . "</td></tr>" );
					$i--;
				}
				echo( "</table>" );
			}
			
			if( isset( $error ) )
				include "includes/components/error.php";
		?>

		<?php include( "includes/components/footerlines.php" ); ?>
	</body>
</html>
