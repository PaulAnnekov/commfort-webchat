<?php
/*
Plugin Name: CommFort WebChat
Plugin URI: http://webcf.ru/
Description: Provides a web-based chat CommFort
Version: 1.0
Author: SteelRat
Author URI: http://steelrat.info
*/

/*
Copyright 2010-2011  SteelRat  (email: global AT steelrat DOT info)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

$cfw_dir = plugins_url( '', __FILE__ );

// Registring css style.
wp_register_style( 'cf-webchat-general', plugins_url( '/general/styles/style.css', __FILE__ ) );
wp_register_style( 'jquery-ui', plugins_url( '/general/jquery-ui/theme/jquery-ui.css', __FILE__ ) );
wp_register_style( 'jquery-ui-start', plugins_url( '/general/jquery-ui/theme/jquery-ui-start.css', __FILE__ ) );

// Registring javascript.
wp_register_script( 'jquery.md5', $cfw_dir . '/general/scripts/jquery.md5.js', array( 'jquery' ) );
wp_register_script( 'cf-webchat-api', $cfw_dir . '/general/scripts/cf-webchat-api.js', array( 'jquery'  ) );
wp_register_script( 'cfw', $cfw_dir . '/script.js', array( 'jquery', 'jquery-ui-core', 'jquery-ui-sortable', 'jquery-ui-dialog', 'jquery.md5', 'cf-webchat-api' ) );

// Adding action to load text domain.
add_action( 'init', 'cfw_on_init' );

// Adding actions to add css and js.
add_action( 'wp_print_scripts', 'cfw_add_js' );
add_action( 'wp_print_styles', 'cfw_add_css' );

// Adding shortcode for displaying chat.
add_shortcode( 'cf_webchat', 'cfw_shortcode' );

// Adding action to add widget when all plugins loaded.
add_action('plugins_loaded', 'cfw_register_widget');

function cfw_shortcode( $atts ) {
	global $cfw_chat_page;
	
	$cfw_chat_page = true;
	
	$texts = array(
		'channel_theme' => __( 'Channel theme', 'cfw' ),
		'send' => __( 'Send', 'cfw' ),
		'settings' => array(
			'sound' => array(
				'label' => '',
				'hint'  => '',
			),
			'autohide' => array(
				'label' => '',
				'hint'  => '',
			)
		),
		'actions' => array(
			'smiles'   => __( 'Smiles', 'cfw' ),
			'settings' => __( 'Settings', 'cfw' ),
			'clear'    => __( 'Clear', 'cfw' ),
		),
		'login_form' => array(
			'login'        => __( 'Login', 'cfw' ),
			'password'     => __( 'Password', 'cfw' ),
			'male'         => __( 'Male', 'cfw' ),
			'female'       => __( 'Female', 'cfw' ),
			'login_button' => __( 'Log in', 'cfw' ),
			'register'     => __( 'Register', 'cfw' )
		),
		'channels-list' => array(
			'channel_name'  => __( 'Name', 'cfw' ),
			'users_count'   => __( 'Users', 'cfw' ),
			'channel_theme' => __( 'Theme', 'cfw' ),
		)
	);
	
	$general_dir_url = $GLOBALS['cfw_dir'] . '/general';
	
	ob_start();
	
	require( 'general/templates/chat.php' );
	
	return ob_get_clean();
}

// Loads plugin init data.
function cfw_on_init() {
	// Set translation domain and path to folder with files.
	load_plugin_textdomain( 'cfw', false, dirname( plugin_basename( __FILE__ ) ) . '/languages' );
}

function cfw_register_widget() {
	wp_register_sidebar_widget( 'cfw', __( 'CommFort WebChat', 'cf_webchat' ), 'cfw_widget' );
}

// Add js.
function cfw_add_js() {
	if ( ! empty( $_GET['address'] ) && ( filter_var( $_GET['address'], FILTER_VALIDATE_IP, FILTER_FLAG_IPV4 ) || preg_match('/^[a-z0-9][a-z0-9\-\.]+[a-z0-9]$/i', $_GET['address'] ) ) ) {
		wp_enqueue_script( 'cfw' );
		$settings = array(
			'plugin_dir' => $GLOBALS['cfw_dir'],
			'server' => $_GET['address'],
		);
		wp_localize_script( 'cfw', 'cfSettings', array_merge( $settings ) );
	}
}

// Add css.
function cfw_add_css() {
	wp_enqueue_style( 'cf-webchat-general' );
	wp_enqueue_style( 'jquery-ui' );
	wp_enqueue_style( 'jquery-ui-start' );
}

// Outputs widget content.
function cfw_widget() {	
	if ( $GLOBALS['cfw_chat_page'] ) {	
		require( 'general/templates/widget.php' );
	}
	
	return true;
}