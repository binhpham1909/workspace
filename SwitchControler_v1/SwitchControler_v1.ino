#include "Arduino.h"
#include "ModuleSettings.h"
#include "MQTTConnection.h"
#include "WifiManager.h"
#include "ServerManager.h"

ServerManager *HTTPServer = NULL;
//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	INIT_SERIAL_DEBUG();
	delay(100);
	//ModuleSettings::getInstance()->resetSetting();
	ModuleSettings::getInstance()->loadSettings();
	//DBG(ModuleSettings::getInstance()->setDeviceName("Wifi Device"));
	//DBG(ModuleSettings::getInstance()->saveSettings());
	DBG2F("Device name: ", ModuleSettings::getInstance()->getDeviceName());
	DBG2F("Username: ", ModuleSettings::getInstance()->getUserDevice());
	DBG2F("Password: ", ModuleSettings::getInstance()->getPassDevice());
	DBG2F("Wifi SSID: ", ModuleSettings::getInstance()->getWifiSSID());
	DBG2F("Wifi password: ", ModuleSettings::getInstance()->getWifiPassword());
	DBG2F("HTTP Server port: ", ModuleSettings::getInstance()->getServerPort());
	DBG2F("MQTT Username: ", ModuleSettings::getInstance()->getMqttUser());
	DBG2F("MQTT API: ", ModuleSettings::getInstance()->getMqttAPI());
	DBG2F("GPIO 0 name: ", ModuleSettings::getInstance()->getGPIOName(0));
	DBG2F("GPIO 1 name: ", ModuleSettings::getInstance()->getGPIOName(1));

	HTTPServer = new ServerManager(ModuleSettings::getInstance()->getServerPort());
	HTTPServer->on("/favicon.png",[](){HTTPServer->handlerFile(200, MimePNG, favicon_png,  sizeof(favicon_png), GZIP_DISABLE,BROWSER_CACHE_DAYS);});
	HTTPServer->on("/js/jquery.min.js.gz", [](){HTTPServer->handlerFile(200, MimeCss, jquery_3_1_0_min_js_gz, sizeof(jquery_3_1_0_min_js_gz), GZIP_ENABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/js/md5.min.js.gz", [](){HTTPServer->handlerFile(200, MimeCss, md5_min_js_gz, sizeof(md5_min_js_gz), GZIP_ENABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/js/bootstrap.min.js.gz", [](){HTTPServer->handlerFile(200, MimeCss, bootstrap_min_js_gz, sizeof(bootstrap_min_js_gz), GZIP_ENABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/css/bootstrap.min.css.gz", [](){HTTPServer->handlerFile(200, MimeCss, bootstrap_min_css_gz, sizeof(bootstrap_min_css_gz), GZIP_ENABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/css/bootstrap-theme.min.css.gz", [](){HTTPServer->handlerFile(200, MimeCss, bootstrap_theme_min_css_gz, sizeof(bootstrap_theme_min_css_gz), GZIP_ENABLE, BROWSER_CACHE_DAYS);});

	HTTPServer->on("/login.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, login_html, sizeof(login_html), GZIP_ENABLE, LOGIN_UNREQUIRE);});

	HTTPServer->on("/js/language-vi.js",[](){HTTPServer->handlerFile(200, MimeTypeJS, language_vi, sizeof(language_vi), GZIP_DISABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/js/main.js",[](){HTTPServer->handlerFile(200, MimeTypeJS, main_js, sizeof(main_js), GZIP_DISABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/css/skeleton.css",[](){HTTPServer->handlerFile(200, MimeCss, skeleton_css, sizeof(skeleton_css), GZIP_DISABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/css/normalize.css",[](){HTTPServer->handlerFile(200, MimeCss, normalize_css, sizeof(normalize_css), GZIP_DISABLE, BROWSER_CACHE_DAYS);});
	HTTPServer->on("/css/main.css",[](){HTTPServer->handlerFile(200, MimeCss, main_css, sizeof(main_css), GZIP_DISABLE, BROWSER_CACHE_DAYS);});

	HTTPServer->on("/js/configs.js",[](){HTTPServer->handlerGetConfigs(); });


	HTTPServer->on("/network.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, network_html, sizeof(network_html), GZIP_DISABLE, LOGIN_REQUIRE);});

	HTTPServer->on("/admin.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, admin_html, sizeof(admin_html), GZIP_DISABLE, LOGIN_REQUIRE);});
	HTTPServer->on("/gpio.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, gpio_html, sizeof(gpio_html), GZIP_DISABLE, LOGIN_REQUIRE);});
	HTTPServer->on("/prog.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, prog_html, sizeof(prog_html), GZIP_DISABLE, LOGIN_REQUIRE);});
	HTTPServer->on("/general.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, general_html, sizeof(general_html), GZIP_DISABLE, LOGIN_REQUIRE);});
	HTTPServer->on("/time.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, time_html, sizeof(time_html), GZIP_DISABLE, LOGIN_REQUIRE);});
	HTTPServer->on("/email.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, email_html, sizeof(email_html), GZIP_DISABLE, LOGIN_REQUIRE);});
	HTTPServer->on("/device.html",[](){HTTPServer->handlerFileLogined(200, MimeHtml, device_html, sizeof(device_html), GZIP_DISABLE, LOGIN_REQUIRE);});



	HTTPServer->on("/",[](){HTTPServer->handlerRoot();});
	HTTPServer->on("/command",[](){HTTPServer->handlerCommand(); });
	HTTPServer->on("/login",[](){HTTPServer->handlerLogin(); });
	HTTPServer->on("/logout.html",[](){HTTPServer->handlerLogout(); });
	HTTPServer->on("/logout",[](){HTTPServer->handlerLogout(); });
	HTTPServer->on("/ajax",[](){HTTPServer->handlerAjax(); });

	HTTPServer->onNotFound ( [](){HTTPServer->handlerNotFound(); });
	HTTPServer->begin();
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	WifiManager::getInstance()->loop();
	HTTPServer->handleClient();
	MQTTConnection::getInstance()->loop();
	//Session::getInstance()->checkExpireTime();
}
