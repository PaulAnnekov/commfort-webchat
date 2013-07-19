/*!
 * CommFort WebChat JS API v0.1
 * http://webcf.ru/
 *
 * Copyright 2011, Paul Annekov
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/ or send a letter 
 * to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

/**
 * Creates CommFortWebchat copy.
 *
 * @constructor
 * @this {CommFortWebchat}
 * @param {string} server Server IP or Domain name.
 * @param [settings] Additional settings.
 * @param [settings.language=en] UI language.
 * @param [settings.proxyUrl=/gateway?server=] URL of proxy.
 * @param [settings.chatAreaHeight=400] Height of chat area.
 * @param [settings.channelUsers] Channel users list settings.
 * @param [settings.channelUsers.position=inside] Position of channel users list.
 * @param [settings.channelUsers.anchor=undefined] The anchor element of channel users list.
 * @param [settings.channelUsers.width=200] Width of channel users list.
 * @param [settings.channelUsers.height=400] Height of channel users list.
 */
function CommFortWebchat(server, settings) {
    var $ = null;
    var loadError = checkDependencies(server);
    if (loadError) {
        console.log("CommFort WebChat load error: '" + loadError + "'");
        return;
    }
	
	var root = this;

    // Load settings.
	this.settings = {
        applicationName: 'Web Interface v0.1',
        loadImages: true,
        maxMessageLength: 20000,
        language: 'en',
        proxyUrl: '/gateway?server=',
        chatAreaHeight: 400,
        channelUsers: {
            position: 'inside',
            width: 200,
            height: 400
        }
    };
    mergeSettings(settings);

    this.server = server;
    this.requestPrefix = '{"jsonrpc": "2.0",';
	this.currentID = 0;
	this.connectID = 0;
	this.authState = 0;
	this.myName = '';
	this.$channelsList = null;
    this.$channelsUsersList = null;
	//this.channels = {};
	//this.users = {};

	this.authStatesTexts = {
		10: 'Too much users online',
		11: 'Nick is out of rules',
		12: 'You are banned',
		13: 'Nick includes bad words',
		14: 'Nick is already registered',
		15: 'Too much users from IP',
		16: 'Activation request sent',
		17: 'Wrong password',
		18: 'Activation request denied or in progress'
	}

    this.translation = {
        ru: {
            'Channels': 'Каналы',
            'Name': 'Название',
            'Users': 'Пользователи',
            'Topic': 'Тема',
            'No topic': 'Нет темы',
            'Users online': 'Пользователей',
            'Hidden': 'Недоступен',
            'Server is offline': 'Сервер недоступен',
            'Loading image': 'Загрузка изображения',
            'Login incorrect': 'Недопустимый логин',
            'Password incorrect': 'Недопустимый пароль',
            'Sending request to server': 'Отправляю запрос серверу',
            'Empty login or password': 'Поле логина или пароля не заполнено',
            'Sex incorrect': 'Недопустимый пол',
            'Message is empty': 'Поле для ввода сообщения пустое',
            'Too long message. Maximum length is': 'Сообщение слишком длинное. Максимальная длинна:',
            'Authorization': 'Авторизация',
            'Waiting for server response to authorization request': 'Ожидание ответа на запрос авторизации',
            'Another user has connected using your name and password': 'Другой пользователь подключился используя Ваше имя и пароль',
            'Too much users online': 'Превышено максимальное число подключенных пользователей',
            'Nick is out of rules': 'Имя не соответствует требованиям (содержит служебные символы, либо превышает максимальную длину в 40 символов)',
            'You are banned': 'Для данного пользователя доступ к серверу запрещен (бан)',
            'Nick includes bad words': 'Имя содержит запрещенные слова (не проходит фильтр плохих слов)',
            'Nick is already registered': 'Схожее по написанию имя уже зарегистрировано',
            'Too much users from IP': 'Превышено максимальное число учетных записей с данного IP-адреса',
            'Activation request sended': 'Заявка на активацию успешно отправлена',
            'Wrong password': 'Неверный пароль',
            'Activation request denied or in progress': 'Заявка на активацию не обработана, либо отклонена'
        }
    };

    function mergeSettings(settings) {
        $.extend(true, root.settings, settings);
    }

    function checkDependencies(server) {
        if (typeof jQuery == "undefined") {
            return "jQuery framework script not found";
        } else if (jQuery != $) {
            $ = jQuery;
        }

        if (!$.hasOwnProperty('ui')) {
            return "jQuery UI script not found";
        }

        if (!$.ui.hasOwnProperty('dialog')) {
            return "jQuery UI Dialog component not found";
        }

        if (!$.ui.hasOwnProperty('sortable')) {
            return "jQuery UI Sortable component not found";
        }
		
		if (!$.ui.hasOwnProperty('draggable')) {
            return "jQuery UI Draggable component not found";
        }

        if (!$.ui.hasOwnProperty('droppable')) {
            return "jQuery UI Droppable component not found";
        }

        if (!$.fn.hasOwnProperty('qtip')) {
            return "jQuery qTip component not found";
        }

        if (!server) {
            return "Server is not valid";
        }

        return false;
    }

	this.parseError = function (err) {
		console.log(err);
		this.error = {};
		if (err.code != undefined) {
			//do something with error
			alert(err.code + ": " + err.message + " (" + err.data + ")");

			this.error.code = err.code;
		} else {
			alert(err); //ReferenceError: connectID is not defined
			this.error.code = 100;
			this.error.message = err.message;
		}
	};

	this.parseResponseError = function (error) {
		switch (error.code) {
			// Such conn_id value does not exist.
			case -32602:	
				this.dom.showGlobalNotification(this.t('Previous connection closed. Reconnecting...'), 'notice');
				this.methods.connectionInit();
			break;
		}
	};
	
	this.resetID = function () {
		this.currentID = 0;
	};

	this.addCurrentID = function () {
		return (this.currentID)++;
	};

    /**
     * Builds JSON-RPC request.
     *
     * @param {string} method JSON-RPC method to call.
     * @param {object} params Params for called method.
     * @param {boolean} [notify=false] Detects request type: default or notification.
     * @return {string} Request string.
     */
	this.buildParameters = function (method, params, notify) {
		var requestParams = this.requestPrefix;
		requestParams += '"method":"' + method + '", ';
		requestParams += '"params":';

		if (typeof params == 'string') {
            requestParams += this.getJSONValue(params);
        } else {
            requestParams += "{";

            //connectID
            if (this.connectID !== 0) {
                requestParams += '"conn_id":"' + this.connectID + '",';
            }

            for (var param in params) {
                if (params.hasOwnProperty(param)) {
                    requestParams += (params[param].type) ? this.getVariableDefinition(param, params[param].value, false, 'plain') : this.getVariableDefinition(param, params[param]);
                }
            }
            requestParams = requestParams.substr(0, requestParams.length - 1) + '}';
        }

		// If it isn't notification request then add 'id' parameter.
        if (typeof notify == 'undefined' || !notify) {
            requestParams += ', "id":' + this.addCurrentID();
        }
		requestParams += '}';

		return requestParams;
	};

    this.getJSONValue = function(value, type) {
        return (type != 'plain') ? '"' + encodeURI(value) + '"' : value;
    }

	this.getVariableDefinition = function (name, value, noComma, type) {
		var result = '"' + name + '":' + this.getJSONValue(value, type);

		if (!noComma) {
			result += ',';
		}

		return result;
	};
	
	this.sendRequest = function (jsonRequest, context, callback, callOnError, params) {
        $.post(this.settings.proxyUrl + this.server, jsonRequest, function(data) {
            if (typeof callback != 'undefined') {
                root[context][callback] (data, params);
            }
		})
		.error(function (e) {
            if (e.status != 502) {
                root.dom.showGlobalNotification(root.t("Server is offline"), 'error', false);
            }
            if (typeof callOnError != 'undefined') {
                root[context][callback] ({error: e.status});
            }
		});
	};

	this.t = function (text) {
		if (root.settings.language != 'en' && root.translation[root.settings.language].hasOwnProperty(text)) {
			text = root.translation[root.settings.language][text];
		}
		
		return text;
	};
	
	this.updateChannelsUsersCounter = function () {
		for (var id in this.channels) {
			if (this.channels.hasOwnProperty(id) && this.channels[id].active) {
				var number = 0;
				for (var user in this.users) {
					if (this.users.hasOwnProperty(user) && typeof this.getArrayIndex(this.users[user].channels, 'name', this.channels[id].name) != undefined) {
						number++;
					}
				}
				this.channels[id].usersOnline = number;
				this.dom.updateChannelUsersListCounter(this.channels[id].usersList, number);
				this.dom.updateChannelTabCounter(this.channels[id].tab, number);
			}
		}
	};
	
	this.removeActiveChannels = function(channelID) {
		//if (channels.constructor == String) {
		//	channels = [{name: channels}];
		//}
		//for (var id in channels) {
			//var channelID = this.getArrayIndex(this.channels, 'name', channels[id].name);
			this.dom.removeActiveChannel(this.channels[channelID]);
			delete this.channels[channelID].tab;
			delete this.channels[channelID].messagesArea;
			delete this.channels[channelID].usersList;
			this.channels[channelID].active = false;
			this.channels[channelID].current = false;
		//}
	};
	
	this.updateChannel = function (name, topic, greeting, users) {
		var channelID = this.getArrayIndex(this.channels, 'name', name);
		if (typeof channelID != undefined) {
			this.channels[channelID].topic = topic;
			this.channels[channelID].greeting = greeting;
			this.channels[channelID].active = true;
			this.channels[channelID].usersOnline = users.length;
		} else {
			this.channels.push({
				name: name,
				topic: topic,
				greeting: greeting,
				usersOnline: users.length,
				active: true
			});
			channelID = this.getArrayIndex(this.channels, 'name', name);
		}
		this.dom.addActiveChannel(channelID);
		for (var user in users) {
			if (users.hasOwnProperty(user)) {
                this.addUserToChannel(users[user], this.channels[channelID]);
            }
		};
	};
	
	this.addUserToChannel = function(user, channel) {
		//var userID = this.getArrayIndex(this.users, 'name', user);
		console.log(this.users);
        console.log(user);
        this.users[user].channels.push({name: channel.name, listElem: this.dom.addUserToChannel(channel.usersList, this.users[user])});
	};
	
	this.htmlGenerators = {
		parent: this,
		channelList: function (channel, active) {		
			var topic = (channel.topic) ? root.escapeHtml(channel.topic) : '';
			if (active) {
				active = ' class="cfw-active-channel"';
			} else {
				active = '';
			}
			
			return '<tr' + active + '><td>' + root.escapeHtml(channel.name) + '</td><td>' + channel.usersOnline + '</td><td>' + topic + '</td></tr>';
		},
		channelMessage: function (sender, message, mode, image) {
			var element = '<div class="cfw-message"><div class="cfw-sender">' + root.escapeHtml(sender) + '</div>: ';
			if (!image) {
				element += root.escapeHtml(message);
			} else {
				if (root.settings.loadImages) {
					element += '<div class="cfw-loading-image">' + root.t('Loading image') + '...</div>';
				} else {
					element += '<div class="cfw-load-image">' + root.t('Click to load image') + '</div>';
				}
			}
			element += '</div>';
			
			return element;
		},
		image: function (image) {
			return '<div class="cfw-image"><img src="data:image/jpeg;base64,' + image + '" /></div>';
		}
	};
	
	this.joinChannel = function (channelID) {
		if (this.users[this.myName].channels.length < 16) {
			this.methods.channelConnect(this.channels[channelID].name);
		} else {
			this.dom.showGlobalNotification(this.t('Too much active channels. User can\t access more then 16 channels'), 'warning');
		}
	};
	
	this.getCurrentChannelName = function ()  {
		var channelObj = this.getArrayElem(this.channels, 'current', true);

		return channelObj.name;
	};
	
	this.setCurrentChannel = function (channelID) {
		var currentChannelID = this.getArrayIndex(this.channels, 'current', true);
		if (typeof currentChannelID != 'undefined') {
			this.channels[currentChannelID].current = false;
		}
		this.channels[channelID].current = true;
		
		this.dom.setCurrentChannel(this.channels[channelID]);
	};
	
	/*this.setCurrentUser = function(name) {
		this.users[this.getArrayIndex(this.users, 'name', name)].iAm = true;
	};*/

	this.getArrayIndex = function (array, property, value) {
		var result;

		for (var id in array) {
			if (array.hasOwnProperty(id) && array[id][property] == value) {
				result = id;
				break;
			}
		}

		return result;
	};
	
	this.getArrayElem = function (array, property, value) {
		var result;
		
		var index = this.getArrayIndex(array, property, value);
		if (index >= 0) {
			result = array[index];
		}

		return result;
	};
	
	this.cookie = {
		setCookie: function (name, value, path, domain, expires, secure) {
			if (!name) {
				return false;
			}
			var str = name + '=' + encodeURIComponent(value);

			if (expires) {
				str += '; expires=' + expires.toGMTString();
			}
			if (path) {
				str += '; path=' + path;
			}
			if (domain) {
				str += '; domain=' + domain;
			}
			if (secure) {
				str += '; secure';
			}

			document.cookie = str;
			return true;
		},
		getCookie: function (name) {
			var pattern = "(?:; )?" + name + "=([^;]*);?";
			var regexp = new RegExp(pattern);

			if (regexp.test(document.cookie)) {
				return decodeURIComponent(RegExp["$1"]);
			}

			return false;
		},
		deleteCookie: function (name) {
            this.setCookie(name, '', null, null, new Date(0));

            return true;
		}
	};
	
	this.escapeHtml = function (str) {
        if (typeof str != 'undefined' && str != null && str.length > 0) {
            str =
                str
                    .replace(/&/g, "&amp;")
                    .replace(/</g, "&lt;")
                    .replace(/>/g, "&gt;")
                    .replace(/"/g, "&quot;")
                    .replace(/'/g, "&#039;");
        } else {
            str = '';
        }

        return str;
    };
	
	// Parse actions array.
	this.parseActions = function (data) {
		if (data && data.length > 0) {
			for (var action in data) {
				if (data.hasOwnProperty(action) && this.actions.hasOwnProperty(data[action].method)) {
                    this.actions[data[action].method] (data[action].result);
                }
            }
		}
	};

    this.validate = {
        userData: function(userData) {
            if (userData.nick && userData.pass) {
                if (userData.nick.length <= 0 || userData.nick.length > 40) {
                    return root.t('Login incorrect');
                }
                if (userData.pass.length <= 0) {
                    return root.t('Password incorrect');
                }
            } else {
                return root.t('Empty login or password');
            }

            return true;
        }
    };

	this.methods = {
		parent: this,
		connectionInit: function () {
            return root.buildParameters('conn_init', {
                app_name: root.settings.applicationName
            });
		},
        checkConnectionID: function (connectionID) {
            return root.buildParameters('check_conn_id', connectionID);
        },
		connectionClose: function () {
			try {
				var request = root.buildParameters('conn_close');
                root.cookie.deleteCookie('cfConnectionID');
                root.cookie.deleteCookie('cfUserConnected');
                root.sendRequest(request, 'callbacks', 'connectionClose');
			} catch (err) {
				root.parseError(err);
				request = undefined;
			}
			return request;
		},

		userConnect: function (userData) {
            return root.buildParameters('user_connect', userData);
		},

		/*userRegister: function (userInformation) {
			try {
				if (userInformation.nick && userInformation.pass && userInformation.hasOwnProperty('sex')) {
					if (userInformation.nick.length <= 0 || userInformation.nick.length > 40) {
						throw root.t('Login incorrect');
					}
					if (userInformation.pass.length != 32) {
						throw root.t('Password incorrect');
					}
					if (userInformation.sex !== 0 && userInformation.sex !== 1) {
						throw root.t('Sex incorrect');
					}
					root.myName = userInformation.nick;
					var request = root.buildParameters('user_reg', userInformation);

					root.nick = userInformation.nick; //login
					root.pass = userInformation.pass; //pass
				} else {
					throw root.t('Empty login or password');
				}
			} catch (err) {
				root.dom.showAuthError(err, 'warning', false);
			}
		},*/
		userDisconnect: function () {
			try {
				var request = root.buildParameters('user_disconnect');
			} catch (err) {
				root.parseError(err);
				request = undefined;
			}
			return request;
		},
		sendPublicMess: function (channel, message, mode) {
			try {
				if (!message) {
					throw root.t('Message is empty');
				}
				if (message.length > root.settings.maxMessageLength) {
					throw root.t('Too long message. Maximum length is') + ' ' + root.settings.maxMessageLength;
				}
				var data = {
					channel: channel,
					message: message,
					mode: mode
				};
				var request = root.buildParameters('send_public_mess', data);
				root.sendRequest(request, 'callbacks', 'sendPublicMess');
			} catch (error) {
				root.dom.showSendError(error, 'error');
			}
		},
		getAuthState: function () {
			try {
				var request = root.buildParameters('get_auth_state');
				root.sendRequest(request, 'callbacks', 'getAuthState');
			} catch (err) {
				root.parseError(err);
				request = undefined;
			}
			return request;
		},
		getChnlsList: function (topic) {
			return root.buildParameters('get_chnls_list');
		},
		getActions: function () {
            return root.buildParameters('get_actions', null, true);
		},
		getChnlUsers: function (channel) {
			try {
				var params = (channel ? {
					chnl_name: channel
				} : '');
				var request = root.buildParameters('get_chnl_users', params);
			} catch (err) {
				root.parseError(err);
				request = undefined;
			}
			return request;
		},
		getOnlineUsers: function () {
			return root.buildParameters('get_online_users');
		},
		getUserChannels: function () {
			try {
				var request = root.buildParameters('get_user_channels');
				root.sendRequest(request, 'callbacks', 'getUserChannels');
			} catch (err) {
				root.parseError(err);
				request = undefined;
			}
			return request;
		},
		getImage: function (image_id, messageObj) {
			try {
				var params = {image_id: image_id};
				var request = root.buildParameters('get_image', params);
				root.sendRequest(request, 'callbacks', 'getImage', null, messageObj);
			} catch (err) {
				root.parseError(err);
				request = undefined;
			}
			return request;
		},
		channelConnect: function (channel, visible, enterType) {
			try {
				var params = {};
				params.channel = channel;
				if (typeof visible != "undefined") {
					params.visible = visible;
				}
				if (typeof enterType != "undefined") {
					params.enter_type = enterType;
				}
				
				var request = root.buildParameters('channel_connect', params);
				root.sendRequest(request, 'callbacks', 'channelConnect');
			} catch (err) {
				root.parseError(err);
			}
		},
		channelDisconnect: function (channel) {
			try {			
				var request = root.buildParameters('channel_disconnect', {channel: channel});
				root.sendRequest(request, 'callbacks', 'channelDisconnect');
			} catch (err) {
				root.parseError(err);
			}
		},
		getOwnUserName: function() {
			try {			
				var request = root.buildParameters('get_own_user_name');
				root.sendRequest(request, 'callbacks', 'getOwnUserName');
			} catch (error) {
				root.parseError(error);
			}
		}
	};

	this.callbacks = {
		parent: this,
		connectionClose: function (data) {
			if (!data.hasOwnProperty('error')) {
				root.connectID = 0;
				//пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅ, пїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅ пїЅ.пїЅ.
				root.sendRequest('getAuthState');
			} else {
				root.parseError(data.error);
			}
		},

		userRegister: function (data) {
			if (!data.hasOwnProperty('error')) {
                root.cookie.setCookie('cfUserConnected', true);
                root.sendRequest('userConnect', {
					nick: root.nick,
					pass: root.pass
				});
			} else {
				root.parseError(data.error);
			}
		},

		userDisconnect: function (data) {
			if (!data.hasOwnProperty('error')) {
				root.sendRequest('getAuthState');
			} else {
				root.parseError(data.error);
			}
		},

		sendPublicMess: function (data) {
			if (!data.hasOwnProperty('error')) {
				
			} else {
				root.parseError(data.error);
			}
		},

		getAuthState: function (data) {
			if (!data.hasOwnProperty('error')) {
				var waiting = root.checkAuthState(data.result);
				if (waiting) {
					root.methods.getAuthState();
				}
			} else {
				root.parseError(data.error);
			}
		},

		getChnlsList: function (data) {
			if (!data.hasOwnProperty('error')) {
				root.channels = [];
				for (var id in data.result) {
					if (data.result.hasOwnProperty(id)) {
                        root.channels.push({
                            name: data.result[id].name,
                            topic: data.result[id].topic,
                            usersOnline: data.result[id].users_online,
                            access: data.result[id].access
                        });
                    }
				}
				root.methods.getOnlineUsers();
			} else {
				root.parseError(data.error);
			}
		},

		getActions: function (data) {
			if (!data.hasOwnProperty('error')) {
                // Check the data that was returned: actions array or single action.
                if (data.method == 'get_actions') {
                    root.parseActions(data.result);
                } else if (root.actions.hasOwnProperty(data.method)) {
                    root.actions[data.method] (data.result);
                }

                if (root.authState != 6) {
					setTimeout(function() { root.methods.getActions() }, 2000);
				}
			} else if (data.error.code == 105) {
                setTimeout(function() { root.methods.getActions() }, 2000);
			} else {
                root.parseError(data.error);
            }
		},
		
		getActionsAuthState: function (data) {
            if (!data.hasOwnProperty('error')) {
				var waiting = true;
				if (!root.hasOwnProperty('tmpActions')) {
					root.tmpActions = [];
				}
                // Check if single notification received.
                if (data.method != 'get_actions') {
                    data.result = {method: data.method, result: data.result};
                }

				for (var action in data.result) {
					if (data.result[action].method == 'user_auth_state') {
                        waiting = root.actions[data.result[action].method] (data.result[action].result);
					} else {
						root.tmpActions.push(data.result[action]);
					}
				}
				// If auth state is 8 or 0 (never get auth state), then check for auth state change actions.
				if (waiting) {
					root.methods.getActions('getActionsAuthState');
				}
			} else {
				root.parseResponseError(data.error);
			}
		},
		
		getChnlUsers: function (data) {
			if (!data.hasOwnProperty('error')) {
				alert(data);
			} else {
				root.parseError(data.error);
			}
		},

		getOnlineUsers: function (data) {
			if (!data.hasOwnProperty('error')) {
				root.users = {};
				for (var id in data.result) {
					data.result[id].channels = [];
					root.users[data.result[id].name] = data.result[id];
				}

				//root.setCurrentUser(root.myName);
				//delete root.myName;
				root.chatDataLoaded = true;
				root.parseActions(root.tmpActions);
				delete root.tmpActions;
				root.methods.getActions();
			} else {
				root.parseError(data.error);
			}
		},

		getUserChannels: function (data) {
			if (!data.hasOwnProperty('error')) {
				for (var id in data.result) {
					var channelID = root.getArrayIndex(root.channels, 'name', data.result[id]);
					root.channels[channelID].active = true;
				}
				root.getInitChatData();
			} else {
				root.parseError(data.error);
			}
		},

		getImage: function (data, messageObj) {
			if (!data.hasOwnProperty('error')) {
				root.dom.addMessageImage(root.htmlGenerators.image(data.result), messageObj);
			} else {
				root.parseError(data.error);
			}
		},
		channelConnect: function (data) {
			if (!data.hasOwnProperty('error')) {
				//root.dom.addMessageImage(root.htmlGenerators.image(data.result), messageObj);
			} else {
				root.parseError(data.error);
			}
		},
		channelDisconnect: function (data) {
			if (!data.hasOwnProperty('error')) {
				//root.dom.addMessageImage(root.htmlGenerators.image(data.result), messageObj);
			} else {
				root.parseError(data.error);
			}
		},
		getOwnUserName: function (data) {
			if (!data.hasOwnProperty('error')) {
				root.myName = data.result;
				root.chatDataLoaded = true;
				root.methods.getActions();
			} else {
				root.parseError(data.error);
			}
		}
	};

	this.actions = {
		parent: this,
		user_auth_state: function (state) {
            if (state != root.authState) {
				// Check user's auth state if connection ID is set and no auth actions received.
				//waiting = (state == 8 || state == 0);

				root.authState = state;
				
				if (root.authState != 0) {
					// If user doesn't send authorization request or logged out.
                    if (root.authState == 7 || root.authState == 6) {
                        root.cookie.deleteCookie('cfUserConnected');
                        root.dom.showAuthForm();
                        root.dom.showNotification('auth', root.t('Another user has connected using your name and password'), 'warning', false);
                    // If user is waiting for authorization response.
					} else if (root.authState == 8) {
						root.dom.authState(root.t('Waiting for server response to authorization request'));
					// If user is online.
                    } else if (root.authState == 9) {
						root.dom.authState(root.t('In chat'));
                        root.dom.authSuccess();
                    // If unsuccessful authorization returned.
					} else if (root.authState > 9) {
                        root.cookie.deleteCookie('cfUserConnected');
						root.dom.showAuthForm();
						root.dom.authUnsuccess();
						root.dom.showAuthError(root.t(root.authStatesTexts[root.authState]));
					}
				}
			}
		},
		public_mess: function(data) {
			var element = root.htmlGenerators.channelMessage(data.sender, data.message, data.mode, data.image);
			var channel = root.getArrayElem(root.channels, 'name', data.channel);
			if (channel) {	
				var messageObj = root.dom.insertMessage(channel.messagesArea, element);
				if (messageObj && data.image) {
					root.methods.getImage(data.image, messageObj);
				}
			}			 
		},
		virtual_user_channel_connect: function(data) {
			root.updateChannel(data.channel, data.topic, data.greeting, data.users);
		},
		user_channel_connect: function(data) {
            //if (root.users.hasOwnProperty(userID)) {
            //    this.addUserToChannel(users[userID], this.channels[channelID]);
           // }
		},
		user_channel_disconnect: function(data) {
			//var user = root.getArrayElem(root.users, 'name', data.user);
			// If disconnected user is connection user.
			//if (user.hasOwnProperty('iAm') && user.iAm) {
			//	root.removeActiveChannels(data.channel);
			//}
			//root.removeUserFromChannels(root.getArrayElem(user.channels, 'name', data.channel).listElem);
		},
        user_disconnect: function(data) {

        },
        user_connect: function(data) {
            data.channels = [];
            root.users[data.name] = data;
        }
	};
	
	this.dom = {
		parent: this,
		showNotification: function (area, message, type, temporary) {
			type = (typeof(type) != 'undefined') ? type : 'error';
			temporary = (typeof(temporary) != 'undefined') ? temporary : true;
			
			$('#cfw-chat .cfw-' + area + '-notifications').stop(true, true).removeClass('cfw-error cfw-notice cfw-warning').html(message).addClass('cfw-' + type).css('display', '').removeClass('cfw-hidden');
			
			if (temporary) {
				this.hideNotification(area, false);
			}
		},
		hideNotification: function (area, noDelay) {
			var delay = (typeof(noDelay) == 'undefined' || noDelay) ? 0 : 1000;
			$('#cfw-chat .cfw-' + area + '-notifications').delay(delay).fadeOut(2000, function() {$('#cfw-chat .cfw-' + area + '-notifications').addClass('cfw-hidden');});
        },
		showGlobalNotification: function (message, type, temporary) {
			this.showNotification('global', message, type, temporary);
		},
		hideGlobalNotification: function () {
			this.hideNotification('global');
		},
		showAuthError: function (message, type, temporary) {
			this.showNotification('auth', message, type, temporary);
		},
		showSendError: function (message, type, temporary) {
			this.showNotification('send', message, type, temporary);
		},
		showAuthForm: function () {
            $('#cfw-chat .cfw-login-form input').removeAttr('disabled');
			$('#cfw-chat .cfw-auth-state').addClass('cfw-hidden');
            $('#cfw-chat .cfw-chat-area').addClass('cfw-hidden');
			$('#cfw-chat .cfw-login-form').removeClass('cfw-hidden');
		},
		showChat: function () {
			$('#cfw-chat .cfw-chat-area').removeClass('cfw-hidden');
		},
		authProcess: function () {
			$('#cfw-chat .cfw-login-form input').attr('disabled', 'disabled');
            $('#cfw-chat .cfw-auth-notifications').addClass('cfw-hidden');
			$('#cfw-chat .cfw-auth-state').removeClass('cfw-hidden');
		},
		authSuccess: function () {
			$('#cfw-chat .cfw-login-form').addClass('cfw-hidden');
			$('#cfw-chat .cfw-auth-state').addClass('cfw-hidden');
			this.showChat();
		},
        initUI: function() {
            this.clearUI();

            var table = '<div class="cfw-channels-list"><table><thead><tr><td>' + root.t('Name') + '</td><td>' + root.t('Users') + '</td><td>' + root.t('Topic') + '</td></tr></thead><tbody></tbody></table></div>';
            this.$channelsList = $(table).dialog({
                autoOpen: false,
                title: root.t('Channels'),
                modal: true,
                width: 400,
                height: 300
            });

            $('.cfw-channels-tabs').sortable({axis: 'x'});
            $('.cfw-channels-tabs').disableSelection();

            $('.cfw-channels').css('height', root.settings.chatAreaHeight + 'px');

            // Set width - 1 because there is a border with 1 pixel width.
            var channelsUsersList = '<div class="cfw-channels-users" style="width: ' + (root.settings.channelUsers.width - 1) + 'px"></div>';
            if (root.settings.channelUsers.position == 'inside') {
                root.$channelsUsersList = $(channelsUsersList).insertBefore('.cfw-channels-and-users .cfw-clear');
                $('.cfw-channels-wrapper').css('margin-left', '-' + root.settings.channelUsers.width + 'px');
                $('.cfw-channels').css('padding-left', root.settings.channelUsers.width + 'px');
                root.$channelsUsersList.addClass('cfw-channels-users-inside');
                $('.cfw-channels-wrapper').addClass('cfw-channels-two-columns');
                root.$channelsUsersList.css('height', root.settings.chatAreaHeight + 'px');
            } else {
                var offset;
                if (root.settings.channelUsers.anchor) {
                    offset = $(root.settings.channelUsers.anchor).offset();
                } else {
                    offset = $('#cfw-chat').offset();
                }
                root.$channelsUsersList = $(channelsUsersList).appendTo('body');
                root.$channelsUsersList.css({
                    position: 'absolute',
                    top: offset.top,
                    left: offset.left
                });
                root.$channelsUsersList.draggable({
                    containment: 'window',
                    snap: '.cfw-chat-area, body',
                    snapTolerance: 15,
                    handle: '.cfw-channel-users-head'
                });
                root.$channelsUsersList.addClass('cfw-channels-users-outside');
            }
        },
        clearUI: function() {
            $('#cfw-chat .cfw-channels-tabs .cfw-channel-tab').remove();
            $('#cfw-chat .cfw-channels').empty();
            root.$channelsList && root.$channelsList.remove();
            root.$channelsUsersList && root.$channelsUsersList.remove();
        },
		authUnsuccess: function () {
			$('#cfw-chat .cfw-login-form input').removeAttr('disabled');
			$('#cfw-chat .cfw-auth-state').addClass('cfw-hidden');
		},
		authState: function (message) {
			$('#cfw-chat .cfw-auth-state').text(message + '...');
		},
		insertMessage: function(channelObj, message) {
			var messageObj = $(message).appendTo($('.cfw-messages-list', channelObj));
			
			return messageObj.get(0);
		},
		addActiveChannel: function (channelID) {
            var channel = root.channels[channelID];
			var current = (channel.current) ? ' cfw-current' : '';
			var topic = (channel.topic) ? root.escapeHtml(channel.topic) : root.t('No topic');
			var tab = '\
				<div class="cfw-channel-tab' + current + '">\
					<div class="cfw-channel-tab-name">' + root.escapeHtml(channel.name) + '</div>\
					<div class="cfw-channel-tab-counter">[' + channel.usersOnline + ']</div>\
					<div class="cfw-channel-tab-close"></div>\
				</div>';
			var messagesArea = '<div class="cfw-channel' + current + '"><div class="cfw-channel-topic">' + topic + '</div><div class="cfw-messages-list"></div></div>';

			var usersList = ' \
				<div class="cfw-channel-users-list' + current + '"> \
					<div class="cfw-channel-users-head">' + root.t('Users online') + ': <span class="cfw-users-counter">' + channel.usersOnline + '</span></div> \
					<ul></ul> \
				</div>';

			root.channels[channelID].tab = $(tab).appendTo('#cfw-chat .cfw-channels-tabs').get(0);
			root.channels[channelID].messagesArea = $(messagesArea).appendTo('#cfw-chat .cfw-channels').get(0);
			root.channels[channelID].usersList = $(usersList).appendTo(root.$channelsUsersList).get(0);

			// Select this channel's tab.
			root.setCurrentChannel(channelID);
		},
		updateChannelTabCounter: function (channelObj, counter) {
			$('.cfw-channel-tab-counter', channelObj).text('[' + counter + ']');
		},
		addMessageImage: function (image, messageObj) {
			$('.cfw-loading-image', messageObj).remove();			
			$(messageObj).append(image);
		},
		showChannelsList: function () {
			$('#cfw-chat .cfw-channels-list').removeClass('cfw-hidden');
		},
		addUserToChannel: function(usersList, user) {
			var sex = (user.sex) ? 'female' : 'male';
			var ip = (user.ip == 'N/A') ? root.t('Hidden') : user.ip;
			var state = (user.state != null) ? ' [' + root.escapeHtml(user.state) + ']' : '';
			var elem = ' \
				<li class="cfw-user cfw-' + sex + '">'
					+ root.escapeHtml(user.name) + '<span class="cfw-user-state">' + state + '</span> \
				</li>';

			var userElem = $(elem).appendTo($('ul', usersList));
            userElem.mouseover(function() {root.dom.showUserInfoTip(this, user.name)});

			return userElem.get(0);
		},
        showUserInfoTip: function(userElem, name) {
            var user = root.users[name];
            if (typeof user != 'undefined') {
                $(userElem).qtip({
                    content: user.ip + ' ' + user.sex,
                    position: {
                        my: 'right center',
                        at: 'left center',
                        target: $(userElem),
                        viewport: $(window)
                    }
                });
            }
        },
		removeActiveChannel: function(channel) {
			// If this channel is current then we must select another channel before delete.
			if (channel.current == true) {
				var toSelectTab = $(channel.tab).prev('.cfw-channel-tab');
				if (!toSelectTab.length) {
					toSelectTab = $(channel.tab).next('.cfw-channel-tab');
				}
				if (toSelectTab.length) {
					root.setCurrentChannel(root.getArrayIndex(root.channels, 'tab', toSelectTab.get(0)));
				}
			}
			$(channel.tab).remove();
			$(channel.messagesArea).remove();
			$(channel.usersList).remove();
		},
		updateChannelUsersListCounter: function (channelUsersList, counter) {
			$('.cfw-users-counter', channelUsersList).text(counter);
		},
		addChannel: function (channelsList) {
			var channelsListObj = $(channelsList).appendTo($('table tbody', root.$channelsList.dialog('widget')));
			
			return channelsListObj.get(0);
		},
		setCurrentChannel: function(channel) {
			$('.cfw-channel-tab.cfw-current').removeClass('cfw-current');
			$(channel.tab).addClass('cfw-current');
			$('.cfw-channel.cfw-current').removeClass('cfw-current');
			$(channel.messagesArea).addClass('cfw-current');
			$('.cfw-channel-users-list.cfw-current').removeClass('cfw-current');
			$(channel.usersList).addClass('cfw-current');
		}
	};

    this.init = {
        connectionInit: function() {
            // If connection ID is set and user wasn't connected to chat, then use current connection ID.
            if (root.cookie.getCookie('cfConnectionID') && !root.cookie.getCookie('cfUserConnected')) {
                root.sendRequest(root.methods.checkConnectionID(root.cookie.getCookie('cfConnectionID')), 'init', 'checkConnection');
            } else {
                root.sendRequest(root.methods.connectionInit(), 'init', 'connectionInitiated');
            }
        },
        checkConnection: function(data) {
            if (data.result) {
                root.connectID = root.cookie.getCookie('cfConnectionID');
                root.dom.showAuthForm();
            } else {
                root.sendRequest(root.methods.connectionInit(), 'init', 'connectionInitiated');
            }
        },
        connectionInitiated: function(data) {
            if (!data.hasOwnProperty('error')) {
                root.cookie.setCookie('cfConnectionID', data.result.conn_id);
                root.cookie.deleteCookie('cfUserConnected');
                root.connectID = data.result.conn_id;
                root.dom.hideGlobalNotification();
                root.dom.showAuthForm();
            } else {
                root.dom.showGlobalNotification(data.error.message, 'error', false);
            }
        },
        userConnect: function(userData) {
            var valid = root.validate.userData(userData);

            if (valid) {
                userData.pass = $.md5(userData.pass);
                root.sendRequest(root.methods.userConnect(userData), 'init', 'userConnected');
                root.dom.authState(root.t('Sending request to server'));
                root.dom.authProcess();
                root.myName = userData.nick;
            } else {
                root.dom.showAuthError(valid, 'error', true);
            }
        },
        userConnected: function(data) {
            if (!data.hasOwnProperty('error')) {
                root.cookie.setCookie('cfUserConnected', true);
                root.dom.initUI();

                // Begin to check new actions (including auth changes).
                root.dom.authState(root.t('Authorization'));
                root.sendRequest(root.methods.getActions(), 'init', 'userConnectProcess', true);
            } else {
                root.dom.showAuthError(data.error);
            }
        },
        userConnectProcess: function(data) {
            var waiting = true;

            if (!data.hasOwnProperty('error')) {
                if (!root.hasOwnProperty('tmpActions')) {
                    root.tmpActions = [];
                }
                // Check if single notification received.
                if (data.method != 'get_actions') {
                    data.result = {method: data.method, result: data.result};
                }

                for (var action in data.result) {
                    if (data.result[action].method == 'user_auth_state') {
                        waiting = root.actions[data.result[action].method] (data.result[action].result);
                    } else {
                        root.tmpActions.push(data.result[action]);
                    }
                }
            }

            // If auth state is 8 or 0 (never get auth state), then check for auth state change actions.
            if (root.authState == 8 || root.authState == 0 || data.error == 502) {
                root.sendRequest(root.methods.getActions(), 'init', 'userConnectProcess', true);
            } else if (root.authState == 9) {
                root.sendRequest(root.methods.getChnlsList(), 'init', 'getChannelsList');
            }
        },
        getChannelsList: function(data) {
            if (!data.hasOwnProperty('error')) {
                root.channels = [];
                for (var id in data.result) {
                    if (data.result.hasOwnProperty(id)) {
                        root.channels.push({
                            name: data.result[id].name,
                            topic: data.result[id].topic,
                            usersOnline: data.result[id].users_online,
                            access: data.result[id].access
                        });
                    }
                }

                root.sendRequest(root.methods.getOnlineUsers(), 'init', 'getOnlineUsers');
            }
        },
        getOnlineUsers: function(data) {
            if (!data.hasOwnProperty('error')) {
                root.users = {};
                for (var id in data.result) {
                    data.result[id].channels = [];
                    root.users[data.result[id].name] = data.result[id];
                }

                //root.setCurrentUser(root.myName);
                //delete root.myName;
                root.chatDataLoaded = true;
                root.parseActions(root.tmpActions);
                delete root.tmpActions;
                root.sendRequest(root.methods.getActions(), 'init', 'getActions', true);
            }
        },
        getActions: function (data) {
            if (!data.hasOwnProperty('error')) {
                // Check the data that was returned: actions array or single action.
                if (data.method == 'get_actions') {
                    root.parseActions(data.result);
                } else if (root.actions.hasOwnProperty(data.method)) {
                    root.actions[data.method] (data.result);
                }
            }

            (root.authState != 6 || data.error == 502) && setTimeout(function() { root.sendRequest(root.methods.getActions(), 'init', 'getActions', true) }, 2000);
        }
    };

	// Listeners.
	$('#cfw-chat .cfw-join-channel').click(function () {
		var $channels = $('table tbody', root.$channelsList.dialog('widget'));
		$channels.empty();
		for (var channelID in root.channels) {
			var channel = root.channels[channelID];
			var elem = '\
			<tr' + ((channel.active) ? ' class="cfw-active-channel"' : '') + '>\
				<td>' + root.escapeHtml(channel.name) + '</td>\
				<td>' + channel.usersOnline + '</td>\
				<td>' + root.escapeHtml(channel.topic) + '</td>\
			</tr>';
			var $channel = $(elem).appendTo($channels);
			if (!channel.active) {
				(function (currentChannelID){
					$channel.click(function(){
						root.$channelsList.dialog('close');
						root.joinChannel(currentChannelID);
					})
				})(channelID);
			}
		}
		root.$channelsList.dialog('option', 'width', 'auto');
		root.$channelsList.dialog('option', 'height', 'auto');
		root.$channelsList.dialog('open');
	});
	
	//$(this.$channelsList.dialog('widget')).delegate('table tbody tr:not(.cfw-active-channel)', 'click', function () {
		//root.joinChannel(this);
	//});
	
	$('.cfw-channels-users').delegate('.cfw-channel-users-list .cfw-user-name', 'hover', function () {
		/* $(this).simpletip({
			fixed: true,
			position: 'right',
			boundryCheck: true,
			content: 'asd',
			baseClass: 'cfw-tooltip'
		}).show(); */
		//alert('asd');
	});
	
	/* $('.cfw-channels-users .cfw-channel-users-list .cfw-user-name').simpletip({
	   fixed: true,
	   position: 'right'
	}); */
	
	$('#cfw-chat .cfw-send-button').click(function() {
		root.methods.sendPublicMess(root.getCurrentChannelName(), $('#cfw-chat .cfw-message-area').val(), 0);
		$('#cfw-chat .cfw-message-area').val('');
	});
	
	$('#cfw-chat .cfw-channel-tab-close').live('click', function(event) {
		root.methods.channelDisconnect(root.getArrayElem(root.channels, 'tab', $(this).parent().get(0)).name);
		root.removeActiveChannels(root.getArrayIndex(root.channels, 'tab', $(this).parent().get(0)));
		event.stopPropagation();
	});
	
	$('#cfw-chat .cfw-channel-tab:not(.cfw-current)').live('click', function() {
		root.setCurrentChannel(root.getArrayIndex(root.channels, 'tab', this));
	});

    $('li.cfw-user').live('mouseover', function() {
        //root.getArrayIndex(root., 'tab', this)
    });
	
	$('#cfw-chat .cfw-login-form form').submit(function() {
        root.init.userConnect({
            nick: $('#cfw-chat .cfw-login-form .cfw-login').attr('value'),
            pass: $('#cfw-chat .cfw-login-form .cfw-password').attr('value')
        });
		
		return false;
	});

    this.init.connectionInit();
}