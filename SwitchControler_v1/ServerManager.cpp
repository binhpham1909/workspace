/*
 * ServerManager.cpp
 *
 *  Created on: 6 thg 2, 2017
 *      Author: MyPC
 */

#include "ServerManager.h"

ServerManager::ServerManager(int port) : ESP8266WebServer(port){
	// TODO Auto-generated constructor stub

	DBGF("HTTP Server Started");
}

ServerManager::~ServerManager() {
	// TODO Auto-generated destructor stub
}

void ServerManager::handlerRoot() {
	int role = getUserRole();
	if(role < 1){
		redirect("/login.html");
	}else{
		redirect("/admin.html");
	}
}


void ServerManager::handlerFile(int code, const char* content_type, PGM_P data, size_t contentLength, bool bGzip, unsigned long expire) {
	getSession();
	sendHeaderGzip(contentLength, bGzip, expire);
	send_P(code, content_type, data, contentLength);
}

void ServerManager::handlerFileLogined(int code, const char* content_type,
		PGM_P data, size_t contentLength, bool bGzip, bool needLogined) {
	getSession();
	if(needLogined && (getUserRole() < 1)){
		redirect("/login.html");
		return;
	}
	sendHeaderGzip(contentLength, bGzip, 0);
	send_P(code, content_type, data, contentLength);
}

void ServerManager::loop() {
}

void ServerManager::handlerNotFound() {

}

void ServerManager::handlerCommand() {
	sendHeader("Connection", "close");
	send(200, MimeJson, Commander::getInstance()->process(arg("plain")));
}

int ServerManager::checkLogined() {
	int role = getUserRole();
	if(role < 1){
		redirect("/login.html");
	}
	return role;
}

void ServerManager::sendHeaderGzip(size_t contentLength, bool gzip, unsigned long expire) {
	if(gzip) sendHeader("Content-Encoding", "gzip");
	sendHeader("Connection", "close");
	if(expire){
		char buf[34];
		time_t t = now();
		// if time is accurate, then try to send a Date header Wed, 15 Nov 1995 06:25:24 GMT
		if ( year(t)>2015 ) {
			char dow[5];
			strncpy(dow,dayShortStr(weekday(t)),4);
			dow[3] = 0;
			sprintf(buf,"%s, %02u %s %04u %02u:%02u:%02u GMT",dow,day(t),monthShortStr(month(t)),year(t),hour(t),minute(t),second(t));
			sendHeader("Date",buf);
		}
		sprintf(buf,"max-age=%lu",expire);
		sendHeader("Cache-Control",buf);
		sprintf(buf,"%lu",(unsigned long)contentLength);
		sendHeader("ETag",buf);
	}
}

int ServerManager::getUserRole() {
	return Session::getInstance()->getUserRole(getSession());
}
void ServerManager::handlerLogin() {
	getSession();
	int role = getUserRole();
	if(role > 0){
		redirect("/");
		return;
	}
	if(!hasArg("userid")||!hasArg("password")){
		sendHeader("Connection", "close");
		send_P(200, MimeHtml, login_html);
	}else{
		String username = arg("userid");
		String password = arg("password");
		if(username == ModuleSettings::getInstance()->getUserDevice()){
			if(password == ModuleSettings::getInstance()->getPassDevice()){
				setSession(USER_ROLE);
				send(200, MimeHtml, "ok");
			}else{
				sendHeader("Connection", "close");
				send(200, MimeHtml, "wrongp");
			}
		}else{
			sendHeader("Connection", "close");
			send(200, MimeHtml, "wrongu");
		}
	}
}

void ServerManager::handlerLogout() {
	String ss = getSession();
	DBGF0("Logout => ");
	DBG2F("Delete Session: ", ss);
	Session::getInstance()->deleteSS(ss);
	redirect("/login.html");
}

void ServerManager::redirect(const char* path) {
	if(!path)	return;
	String url;
	url = "http://";
	url += getHost();
	url += path;

	String content;
	content.reserve(250);

	content = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta http-equiv=\"refresh\" content=\"0; URL='";
	content += url;
	content += "'\" />\n<title>Redirecting</title>\n</head>\n<body onload=\"window.location='";
	content += url;
	content += "';\">\n</body>\n</html>";

	sendHeader("Location",url);
	sendHeader("Cache-Control","no-cache");
	sendHeader("Pragma","no-cache");
	DBGF("Redirect...");
	send(302,"text/html",content);
}

String ServerManager::getHost() {
	String host;
	host.reserve(24);
	host = header("Host");
	if ( host.length()<1 ) {
	host=WiFi.localIP().toString();
	if ( ModuleSettings::getInstance()->getServerPort()!=80 ) {
	  host += ":";
	  host += ModuleSettings::getInstance()->getServerPort();
	}
	}
	DBG2F("Host: ",host);
	return host;
}

void ServerManager::setSession(int role) {
	String cookie;
	String ssName= Session::getInstance()->createSS(role);
	cookie = "espid=";
	cookie += ssName;
	cookie += "; Path=/; Max-Age=";
	cookie += SESSION_EXPIRE_TIME;
	DBG2F("Set-Cookie: ", cookie);
	sendHeader("Set-Cookie",cookie);
}

String ServerManager::getSession() {
	int numH = headers();
	for(int i = 0; i<numH; i++){
		DBG2F0("header ",i);
		DBG2F0(" : ",headerName(i));
		DBG2F(" == ",header(i));
	}
	String cookie = header("Cookie");
	DBGF0("Client Cookie: ");
	DBG(header("Cookie"));
	return cookie;
}

void ServerManager::handlerGetConfigs() {
	getSession();
	sendHeaderGzip(sizeof(bootstrap_min_css_gz), GZIP_DISABLE, 0);
	String config = FPSTR(configs_language_js);
	config.replace("$cf1$","vi");

	config+= FPSTR(configs_nework_js);
	config.replace("$cf1$",ModuleSettings::getInstance()->getWifiSSID());
	config.replace("$cf2$",ModuleSettings::getInstance()->getWifiPassword());
	config.replace("$cf3$",String(ModuleSettings::getInstance()->getDHCP()));
	config.replace("$cf4$",ModuleSettings::getInstance()->getStaticIP());
	config.replace("$cf5$",ModuleSettings::getInstance()->getMask());
	config.replace("$cf6$",ModuleSettings::getInstance()->getGateway());
	config.replace("$cf7$",String(ModuleSettings::getInstance()->getServerPort()));
	send(200, MimeTypeJS, config);

}

void ServerManager::handlerBootstrapTheme() {
}

void ServerManager::handlerBootstrapJS() {
}

void ServerManager::handlerMD5JS() {
}

void ServerManager::handlerJqueryJS() {
}

void ServerManager::handlerAjax() {
	getSession();
	if(getUserRole()>0){
		sendHeader("Connection", "close");
		send(200, MimeJson, Commander::getInstance()->process(arg("plain")));
	}else{
		sendHeader("Connection", "close");
		send_P(200, MimeJson, JsonFalse, sizeof(JsonFalse));
	}
}

void ServerManager::handlerNetworkSettings() {

}
