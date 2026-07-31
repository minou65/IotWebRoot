#ifndef _IOTWEBROOTTAB_h
#define _IOTWEBROOTTAB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "IotWebRoot.h"

// -- HTML page fragments for tabs
const char IOTWEBROOTTAB_HTML_STYLE_INNER_TAB[] PROGMEM = 
    "body > div{min-width:260px;max-width:600px;width:100%;box-sizing:border-box;}\n"
    ".tab{overflow:hidden;border-bottom:2px solid #16A1E7;background-color:#f1f1f1;margin-bottom:10px;display:flex;}\n"
    ".tab button{background-color:#f1f1f1 !important;flex:1 1 0;min-width:0;border:1px solid #ccc !important;outline:none;cursor:pointer;"
    "padding:14px 16px;transition:0.3s;font-size:16px;border-top-left-radius:5px;border-top-right-radius:5px;"
    "margin-right:2px;border-bottom:none !important;color:#333 !important;line-height:normal !important;width:auto !important;box-sizing:border-box;}\n"
    ".tab button:hover{background-color:#ddd !important;}\n"
    ".tab button.active{background-color:#16A1E7 !important;color:white !important;border:1px solid #16A1E7 !important;"
    "border-bottom:2px solid #fff !important;position:relative;z-index:1;}\n"
    ".tabcontent{display:none;padding:12px;border:1px solid #ccc;border-top:none;background-color:#fff;}\n"
    "fieldset{width:100%;box-sizing:border-box;}\n";

const char IOTWEBROOTTAB_HTML_TAB_CONTAINER[] PROGMEM = "<div class=\"tab\">\n";
const char IOTWEBROOTTAB_HTML_TAB_CONTAINER_END[] PROGMEM = "</div>\n";
const char IOTWEBROOTTAB_HTML_TAB_BUTTON[] PROGMEM = "<button class=\"tablinks\" onclick=\"openTab(event, '{id}')\">{v}</button>\n";
const char IOTWEBROOTTAB_HTML_TAB_CONTENT[] PROGMEM = "<div id=\"{id}\" class=\"tabcontent\">\n";
const char IOTWEBROOTTAB_HTML_TAB_CONTENT_END[] PROGMEM = "</div>\n";
const char IOTWEBROOTTAB_HTML_SCRIPT_INNER_TAB[] PROGMEM = 
    "function openTab(evt, tabName) {\n"
    "    var i, tabcontent, tablinks;\n"
    "    tabcontent = document.getElementsByClassName('tabcontent');\n"
    "    for (i = 0; i < tabcontent.length; i++) {\n"
    "        tabcontent[i].style.display = 'none';\n"
    "    }\n"
    "    tablinks = document.getElementsByClassName('tablinks');\n"
    "    for (i = 0; i < tablinks.length; i++) {\n"
    "        tablinks[i].className = tablinks[i].className.replace(' active', '');\n"
    "    }\n"
    "    document.getElementById(tabName).style.display = 'block';\n"
    "    if(evt) evt.currentTarget.className += ' active';\n"
    "}\n";
const char IOTWEBROOTTAB_HTML_SCRIPT_INNER_TAB_INIT[] PROGMEM = 
    "window.addEventListener('DOMContentLoaded',function(){var t=document.getElementsByClassName('tablinks')[0];if(t)t.click();});\n";

struct TabData {
	String id;
	String title;
	String content;
};

class TabHtmlRootFormatProvider : public HtmlRootFormatProvider {
public:
	TabHtmlRootFormatProvider() : _tabCount(0) {}

	void addTab(String tabName, String tabId = "") {
		if (_tabCount >= 10) return;
		
		if (tabId == "") {
			tabId = "tab" + String(_tabCount);
		}
		
		_tabs[_tabCount].id = tabId;
		_tabs[_tabCount].title = tabName;
		_tabs[_tabCount].content = "";
		_tabCount++;
	}

	void addString(String htmlString, String tabName) {
		for (uint8_t i = 0; i < _tabCount; i++) {
			if (_tabs[i].title == tabName || _tabs[i].id == tabName) {
				_tabs[i].content += htmlString;
				return;
			}
		}
	}

	String getHtmlTabs() {
		String html = "";
		
		// Tab Buttons
		html += getHtmlTabContainer();
		for (uint8_t i = 0; i < _tabCount; i++) {
			html += getHtmlTabButton(_tabs[i].title, _tabs[i].id);
		}
		html += getHtmlTabContainerEnd();
		
		// Tab Contents
		for (uint8_t i = 0; i < _tabCount; i++) {
			html += getHtmlTabContent(_tabs[i].id);
			html += _tabs[i].content;
			html += getHtmlTabContentEnd();
		}
		
		return html;
	}

	virtual String getHtmlHeadEnd() override {
		String styleInner_ = getStyleInner();
		String s = "<style>\n" + styleInner_ + "</style>\n";
		s += FPSTR(IOTWEBROOT_HTML_HEAD_END);  // </head><body>
		s += getBodyStart();                    // <div>
		String scriptInner_ = getScriptInner();
		s += "<script>\n" + scriptInner_ + "\n</script>\n";
		return s;
	}

	virtual String getHtmlScript() override {
		return "";
	}

	virtual String getHtmlTabContainer() {
		return FPSTR(IOTWEBROOTTAB_HTML_TAB_CONTAINER);
	}

	virtual String getHtmlTabContainerEnd() {
		return FPSTR(IOTWEBROOTTAB_HTML_TAB_CONTAINER_END);
	}

	virtual String getHtmlTabButton(String title, String id) {
		String s = FPSTR(IOTWEBROOTTAB_HTML_TAB_BUTTON);
		s.replace("{v}", title);
		s.replace("{id}", id);
		return s;
	}

	virtual String getHtmlTabContent(String id) {
		String s = FPSTR(IOTWEBROOTTAB_HTML_TAB_CONTENT);
		s.replace("{id}", id);
		return s;
	}

	virtual String getHtmlTabContentEnd() {
		return FPSTR(IOTWEBROOTTAB_HTML_TAB_CONTENT_END);
	}

protected:
	virtual String getStyleInner() override {
		String base_ = String(FPSTR(IOTWEBROOT_HTML_STYLE_INNER));
		String tab_ = String(FPSTR(IOTWEBROOTTAB_HTML_STYLE_INNER_TAB));

		return base_ + tab_;
	}

	virtual String getScriptInner() override {
		return String(FPSTR(IOTWEBROOT_HTML_SCRIPT_INNER_JS_REQUESTDATA)) + 
		       String(FPSTR(IOTWEBROOTTAB_HTML_SCRIPT_INNER_TAB)) + 
		       String(FPSTR(IOTWEBROOTTAB_HTML_SCRIPT_INNER_TAB_INIT));
	}

private:
	TabData _tabs[10];
	uint8_t _tabCount;
};

#endif