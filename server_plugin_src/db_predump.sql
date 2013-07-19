SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

DROP TABLE IF EXISTS `%scf_actions`;
CREATE TABLE IF NOT EXISTS `%scf_actions` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `c_id` varchar(40) NOT NULL,
  `json_object` text NOT NULL,
  `time` int(13) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

DROP TABLE IF EXISTS `%scf_channels`;
CREATE TABLE IF NOT EXISTS `%scf_channels` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(40) NOT NULL,
  `topic` varchar(260) DEFAULT NULL,
  `last_topic_editor` varchar(40) DEFAULT NULL,
  `topic_edit_time` int(13) DEFAULT NULL,
  `topic_edit_rights` int(1) DEFAULT NULL,
  `greeting` text,
  `image_type` int(1) DEFAULT NULL,
  `visibility` int(1) NOT NULL DEFAULT '1',
  `access` int(1) DEFAULT NULL,
  `users_online` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=9 ;

DROP TABLE IF EXISTS `%scf_users`;
CREATE TABLE IF NOT EXISTS `%scf_users` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `ip` varchar(20) NOT NULL,
  `computer_id` varchar(40) DEFAULT NULL,
  `sex` int(1) NOT NULL,
  `client_version` varchar(90) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

DROP TABLE IF EXISTS `%scf_users_online`;
CREATE TABLE IF NOT EXISTS `%scf_users_online` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(40) NOT NULL COMMENT 'max length - 40 symbols',
  `state` varchar(64) DEFAULT NULL,
  `public_ip` varchar(15) NOT NULL,
  `window_activity` int(1) DEFAULT NULL,
  `downtime` int(13) DEFAULT NULL,
  `process` text,
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

DROP TABLE IF EXISTS `%scf_user_channel`;
CREATE TABLE IF NOT EXISTS `%scf_user_channel` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `user` varchar(40) NOT NULL,
  `channel` varchar(32) NOT NULL COMMENT 'max length - 32 symbols',
  PRIMARY KEY (`id`),
  KEY `user` (`user`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;