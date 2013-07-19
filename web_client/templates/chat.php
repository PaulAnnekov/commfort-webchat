<div id="cfw-chat" class="cfw-chat">
	<div class="cfw-global-notifications cfw-hidden"></div>
	<?php //if ( ! $logged ): ?>
	<div class="cfw-login-form cfw-hidden">
		<form action="" type="post">
			<table class="cfw-login-form">
				<tbody>
					<tr>
						<td><?php echo $texts['login_form']['login']; ?></td>
						<td><input type="text" class="cfw-login" /></td>
					</tr>
					<tr>
						<td><?php echo $texts['login_form']['password']; ?></td>
						<td><input type="password" class="cfw-password" /></td>
					</tr>
					<tr>
						<td class="cfw-sex-icons" colspan="2">
							<img class="selected man-icon" src="<?php echo $general_dir_url; ?>/images/man-icon.png" title="<?php echo $texts['login_form']['male']; ?>" alt="<?php echo $texts['login_form']['male']; ?>" />
							<img class="woman-icon" src="<?php echo $general_dir_url; ?>/images/woman-icon.png" title="<?php echo $texts['login_form']['female']; ?>" alt="<?php echo $texts['login_form']['female']; ?>" />
						</td>
					</tr>
					<tr>
						<td><input type="submit" class="cfw-login-submit" value="<?php echo $texts['login_form']['login_button']; ?>"></td>
						<td><a href=""><?php echo $texts['login_form']['register']; ?></a></td>
					</tr>
					<tr>
						<td colspan="2" class="cfw-auth-state cfw-hidden"></td>
					</tr>
					<tr>
						<td colspan="2" class="cfw-auth-notifications cfw-hidden"></td>
					</tr>
				</tbody>
			</table>
		</form>
	</div>
	<?php //endif; ?>
	<div class="cfw-chat-area cfw-hidden<?php //echo ( ! $logged ) ? ' cfw-hidden' : ''; ?>">
		<div class="cfw-channels-tabs-wrapper">
			<div class="cfw-channels-tabs"></div>
			<div class="cfw-join-channel">+</div><div class="cfw-clear"></div>
		</div>
		<div class="cfw-channels-and-users">
			<div class="cfw-channels-wrapper">
				<div class="cfw-channels"></div>
			</div>
			<div class="cfw-clear"></div>
		</div>
		<div class="cfw-settings">
			<div class="cfw-sound-option" title="<?php echo $texts['settings']['sound']['hint']; ?>">
				<label><input type="checkbox" name="cfw-notification-sound" class="cfw-notification-sound"<?php echo ( $is_notif_sound ) ? ' checked="checked"' : ''; ?> />
				<?php echo $texts['settings']['sound']['label']; ?></label>
			</div>
			<div class="cfw-autohide-option" title="<?php echo $texts['settings']['autohide']['hint']; ?>">
				<label><input type="checkbox" name="cfw-autohide" class="cfw-autohide"<?php echo ( $is_autohide_sm ) ? ' checked="checked"' : ''; ?> />
				<?php echo $texts['settings']['autohide']['label']; ?></label>
			</div>
		</div>
		<div class="cfw-smilies"></div>
		<textarea class="cfw-message-area"></textarea>
		<input class="cfw-send-button" type="button" value="<?php echo $texts['send']; ?>" />
		<div class="cfw-actions-buttons">
			<img src="<?php echo $general_dir_url; ?>/images/smilies.png" alt="<?php echo $texts['actions']['smiles']; ?>" title="<?php echo $texts['actions']['smiles']; ?>" class="cfw-smilies-button" />
			<img src="<?php echo $general_dir_url; ?>/images/settings.png" alt="<?php echo $texts['actions']['settings']; ?>" title="<?php echo $texts['actions']['settings']; ?>" class="cfw-settings-button" />
			<img src="<?php echo $general_dir_url; ?>/images/clear_chat.png" alt="<?php echo $texts['actions']['clear']; ?>" title="<?php echo $texts['actions']['clear']; ?>" class="cfw-clear-button" />	
		</div>
		<div class="cfw-send-notifications"></div>
		<div class="cfw-clear"></div>
	</div>
</div>