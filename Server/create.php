<?php include_once( "includes/components/login.php" ); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.0 Strict//EN">
<?php
	if( !isset( $error ) )
	{
		$itemType = $_GET['itemType'];
		$userList = getAllUserIDs();
		if( !is_array( $userList ) )
			$error = $userList;
			
		$parentid = $_GET['parentid'];
		if( $parentid > 0 )
			$itemData = getItemData( $parentid );
		else
		{
			$itemData = array();
			$itemData['mode'] = 0600;
			$itemData['ownergroup'] = 0;
		}
			
		
	}
?>
<html>
	<head>
		<?php
			$title = APPLICATION_NAME . " - Erstellen";
			include_once( "includes/components/defhead.php" );
		?>
	</head>
	<body>
		<?php include( "includes/components/headerlines.php" ); ?>

		<?php if( !isset( $error ) ) { ?>

			<form action="create2.php" method="post" enctype="multipart/form-data">
				<input type="hidden" name="parentid" value="<?php echo $parentid; ?>">
				<input type="hidden" name="itemType" value="<?php echo $itemType; ?>">
				<table>
					<tr><td class="fieldLabel">Name</td><td><input type="text" name="folderName" required="required"></td></tr>
					<tr><td class="fieldLabel">Beschreibung</td><td><input type="text" name="description"></td></tr>
					<tr><td class="fieldLabel">Eigentümer</td><td><select name="owneruser">
						<?php
							forEach( $userList as $theUser )
							{
								if( !$theUser['is_group'] )
								{
									if( $theUser['id'] == $actUser['id'] )
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
						<tr><td class="fieldLabel">Gruppe</td><td><select name="ownergroup">
						<?php
							forEach( $userList as $theUser )
							{
								if( $theUser['is_group'] )
								{
									if( $theUser['id'] == $itemData['ownergroup'] )
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
							<input type='submit' value='Erstellen'>
							<input type='button' onClick='window.history.back();' value='Abbruch'>
						</td>
					</tr>
				</table>
			</form>
		<?php } else 
			include "includes/components/error.php";
		?>

		<?php include( "includes/components/footerlines.php" ); ?>
	</body>
</html>
