CommFort WebChat is the plugin for [CommFort chat](http://www.commfort.com/en/index.shtml) (Windows only) desktop application. The main purpose of this plugin is to bring chat to the web.

You can read more about this plugin [here](http://www.commfort.com/ru/forum/viewtopic.php?f=32&t=6607) (in russian lang).

To formalize contact process between server plugin and clients there was written [API](https://docs.google.com/file/d/0B8riTqvQMQTIRUYtd2pHU2E1N28/edit?usp=sharing) (in russian lang) over [JSON-RPC 2.0](http://www.jsonrpc.org/specification) protocol.

# Parts
* Plugin DLL server library. Loaded by CommFort server.
* Web scripts. JS/PHP/HTML files loaded on client side. They connect to server.

# Facts
* Server plugin was tested and optimized for [Wine](http://www.winehq.org/). CommFort server works under Wine. It was my goal to make plugin work under Wine too. 
* There is small HTTP server implemented in server plugin. It just gets HTTP request, parses it and returns response.
* Client side can communicate with server using HTTP requests (AJAX) or clean protocol (pass only request data). All comunication is implemented over Winsock.
* I have created small JSON parser/generator (in C++) specially for project's purposes.
* CommFort server provided binary API for plugins. Plugin parses and generates binary data.

# Tools
* RAD Studio XE2.
* [KGrid](http://www.sourcecodeonline.com/details/kgrid.html). Grid control with a variety of options.
* [CommFort plugin API](http://www.commfort.com/download/commfort_plugins_5_doc_ru.zip) (in russian lang).

# About
CommFort chat is popular chat application in CIS countries (Russia, Ukraine, Belarus etc.).

It was my first serious project on C++ so don't be preconceived. I have started development on July 2010. Project was closed on September 2012 due to the fact that the popularity of CommFort chat began to fall.

The code is in transition state. I had many todos but my motivation fell and I have closed the project.

# Screenshots
## Server plugin
All server plugin images are stored [here](https://github.com/PaulAnnekov/commfort-webchat/tree/master/server_plugin_screen).

![Main form](https://raw.github.com/PaulAnnekov/commfort-webchat/master/server_plugin_screen/main_form_en.png)
![Connections form](https://raw.github.com/PaulAnnekov/commfort-webchat/master/server_plugin_screen/connections_form.png)

## Web client
![Chat](https://raw.github.com/PaulAnnekov/commfort-webchat/master/web_client_screen/chat.png)

# License
The source code of this plugin is licensed under the [GNU GENERAL PUBLIC LICENSE](https://github.com/PaulAnnekov/commfort-webchat/blob/master/COPYING.txt).
