#pragma once
#include "LoginScene.h"
#include "Utils.h"
#include "SFSRequest.h"
#include "EventHandler.h"
#include "Constant.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

using namespace std;
using namespace cocos2d;

void LoginScene::onInit()
{
	BaseScene::onInit();
	Utils::getSingleton().SoundEnabled = UserDefault::getInstance()->getBoolForKey(constant::KEY_SOUND.c_str(), true);
	Utils::getSingleton().IgnoreInvitation = UserDefault::getInstance()->getBoolForKey(constant::KEY_SOUND.c_str(), false);
	UserDefault::getInstance()->setBoolForKey(constant::KEY_SOUND.c_str(), Utils::getSingleton().SoundEnabled);
	UserDefault::getInstance()->setBoolForKey(constant::KEY_INVITATION.c_str(), Utils::getSingleton().IgnoreInvitation);

	Sprite* bg = Sprite::create("bg.jpg");
	bg->setPosition(560, 350);
	addChild(bg);

	Sprite* spTitle = Sprite::create("login/title.png");
	spTitle->setPosition(560, 550);
	mLayer->addChild(spTitle);
	Utils::getSingleton().autoScaleNode(spTitle);

	loginNode = Node::create();
	loginNode->setPosition(560, 350);
	mLayer->addChild(loginNode);
	Utils::getSingleton().autoScaleNode(loginNode);

	tfUsername = ui::EditBox::create(Size(440, 55), "login/box.png");
	tfUsername->setPosition(Vec2(0, 80));
	tfUsername->setFontName("Arial");
	tfUsername->setFontSize(25);
	tfUsername->setFontColor(Color3B::WHITE);
	tfUsername->setPlaceHolder(Utils::getSingleton().getStringForKey("login_name").c_str());
	tfUsername->setPlaceholderFontColor(Color3B::WHITE);
	tfUsername->setPlaceholderFont("Arial", 25);
	tfUsername->setMaxLength(16);
	tfUsername->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	tfUsername->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	tfUsername->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	tfUsername->setDelegate(this);
	loginNode->addChild(tfUsername);

	tfPassword = ui::EditBox::create(Size(440, 55), "login/box.png");
	tfPassword->setPosition(Vec2(0, 10));
	tfPassword->setFontName("Arial");
	tfPassword->setFontSize(25);
	tfPassword->setFontColor(Color3B::WHITE);
	tfPassword->setPlaceHolder(Utils::getSingleton().getStringForKey("password").c_str());
	tfPassword->setPlaceholderFontColor(Color3B::WHITE);
	tfPassword->setPlaceholderFont("Arial", 25);
	tfPassword->setMaxLength(16);
	tfPassword->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	tfPassword->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	tfPassword->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	tfPassword->setDelegate(this);
	loginNode->addChild(tfPassword);

	ui::CheckBox* checkbox = ui::CheckBox::create();
	checkbox->loadTextureBackGround("popup/box.png");
	checkbox->loadTextureFrontCross("popup/check.png");
	checkbox->setPosition(Vec2(-195, -50));
	checkbox->setSelected(true);
	loginNode->addChild(checkbox);

	Label* lb1 = Label::createWithTTF("", "fonts/arialbd.ttf", 20);
	lb1->setString(Utils::getSingleton().getStringForKey("remember_password"));
	
	lb1->setPosition(-105, -50);
	loginNode->addChild(lb1);

	Label* lb2 = Label::createWithTTF("", "fonts/arialbd.ttf", 20);
	lb2->setString(Utils::getSingleton().getStringForKey("forgot_password"));
	lb2->setPosition(145, -50);
	loginNode->addChild(lb2);

	ui::Button* btnLogin = ui::Button::create("login/btn_login.png", "login/btn_login_clicked.png");
	btnLogin->setPosition(Vec2(-110, -115));
	addTouchEventListener(btnLogin, [=]() {
		if (Utils::getSingleton().gameConfig.phone.length() == 0) {
			requestGameConfig();
			return;
		}
		string username = tfUsername->getText();// Utils::getSingleton().trim(tfUsername->getText());
		if (username.length() == 0) {
			showPopupNotice(Utils::getSingleton().getStringForKey("hay_nhap_tai_khoan"), [=]() {});
			return;
		}
		string password = tfPassword->getText();// Utils::getSingleton().trim(tfPassword->getText());
		if (password.length() == 0) {
			showPopupNotice(Utils::getSingleton().getStringForKey("hay_nhap_mat_khau"), [=]() {});
			return;
		}
		showWaiting();
		SFSRequest::getSingleton().Connect();
	});
	loginNode->addChild(btnLogin);

	ui::Button* btnRegister = ui::Button::create("login/btn_register.png", "login/btn_register_clicked.png");
	btnRegister->setPosition(Vec2(110, -115));
	addTouchEventListener(btnRegister, [=]() {
		loginNode->setVisible(false);
		registerNode->setVisible(true);
	});
	loginNode->addChild(btnRegister);

	ui::Button* btnFB = ui::Button::create("login/btn_fb.png", "login/btn_fb_clicked.png");
	btnFB->setPosition(Vec2(0, -195));
	addTouchEventListener(btnFB, [=]() {
		showWaiting(300);
		Utils::getSingleton().loginFacebook();
	});
	loginNode->addChild(btnFB);

	initRegisterNode();

	ui::Button* btnPhone = ui::Button::create("login/btn_phone.png", "login/btn_phone_clicked.png");
	btnPhone->setPosition(Vec2(45, 40 * scaleScene.x));
	addTouchEventListener(btnPhone, [=]() {
		string phone = Utils::getSingleton().gameConfig.phone;
		if (phone.length() == 0) return;
		Utils::getSingleton().openTel(phone);
	});
	mLayer->addChild(btnPhone);
	Utils::getSingleton().autoScaleNode(btnPhone);

	labelPhone = Label::create("01639070707", "fonts/arialbd.ttf",25);
	labelPhone->setPosition(90, 3);
	labelPhone->setAnchorPoint(Vec2(0, 0));
	mLayer->addChild(labelPhone);

	Label* labelVersion = Label::create("ver 1.0.0", "fonts/arial.ttf", 18);
	labelVersion->setPosition(1115, 3);
	labelVersion->setAnchorPoint(Vec2(1, 0));
	mLayer->addChild(labelVersion);

	std::string lastUsername = UserDefault::getInstance()->getStringForKey(constant::KEY_USERNAME.c_str());
	std::string lastPassword = UserDefault::getInstance()->getStringForKey(constant::KEY_PASSWORD.c_str());
	tfUsername->setText(lastUsername.c_str());
	tfPassword->setText(lastPassword.c_str());

	if (Utils::getSingleton().gameConfig.phone.length() == 0) {
		requestGameConfig();
	}
}

void LoginScene::registerEventListenner()
{
	BaseScene::registerEventListenner();
	EventHandler::getSingleton().onConnected = std::bind(&LoginScene::onConnected, this);
	EventHandler::getSingleton().onLoginZone = std::bind(&LoginScene::onLoginZone, this);
	EventHandler::getSingleton().onConnectionFailed = std::bind(&LoginScene::onConnectionFailed, this);
	EventHandler::getSingleton().onConfigZoneReceived = std::bind(&LoginScene::onConfigZoneReceived, this);
	EventHandler::getSingleton().onErrorSFSResponse = std::bind(&LoginScene::onErrorResponse, this, std::placeholders::_1, std::placeholders::_2);
	EventHandler::getSingleton().onUserDataMeSFSResponse = std::bind(&LoginScene::onUserDataMeResponse, this);
	EventHandler::getSingleton().onLoginFacebook = std::bind(&LoginScene::onLoginFacebook, this, std::placeholders::_1);

	SFSRequest::getSingleton().onHttpResponseFailed = std::bind(&LoginScene::onHttpResponseFailed, this);
	SFSRequest::getSingleton().onHttpResponse = std::bind(&LoginScene::onHttpResponse, this, std::placeholders::_1, std::placeholders::_2);
}

void LoginScene::unregisterEventListenner()
{
	BaseScene::unregisterEventListenner();
	EventHandler::getSingleton().onConnected = NULL;
	EventHandler::getSingleton().onLoginZone = NULL;
	EventHandler::getSingleton().onConnectionFailed = NULL;
	EventHandler::getSingleton().onConfigZoneReceived = NULL;
	EventHandler::getSingleton().onErrorSFSResponse = NULL;
	EventHandler::getSingleton().onUserDataMeSFSResponse = NULL;
	EventHandler::getSingleton().onLoginFacebook = NULL;

	SFSRequest::getSingleton().onHttpResponse = NULL;
	SFSRequest::getSingleton().onHttpResponseFailed = NULL;
}

void LoginScene::editBoxReturn(ui::EditBox * editBox)
{
}

void LoginScene::onConnected()
{
	SFSRequest::getSingleton().LoginZone("", "g", Utils::getSingleton().gameConfig.zone);
}

void LoginScene::onLoginZone()
{
	if (loginNode->isVisible()) {
		if (fbToken.length() > 0) {
			SFSRequest::getSingleton().RequestLoginFacebook(fbToken);
		} else {
			SFSRequest::getSingleton().RequestLogin(tfUsername->getText(), md5(tfPassword->getText()));
		}
	} else {
		//SFSRequest::getSingleton().RequestRegister(tfResUname->getText(), md5(tfResPass->getText()), tfResEmail->getText());
		SFSRequest::getSingleton().RequestRegister(tfResUname->getText(), md5(tfResPass->getText()), md5(tfResPassAgain->getText()));
	}
}

void LoginScene::onConnectionFailed()
{
	hideWaiting();
	showPopupNotice(Utils::getSingleton().getStringForKey("error_connection"), [=]() {});
}

void LoginScene::onConfigZoneReceived()
{
	Utils::getSingleton().goToMainScene();
}

void LoginScene::onUserDataMeResponse()
{
	BaseScene::onUserDataMeResponse();
	if (fbToken.length() > 0) {
		Utils::getSingleton().loginType = constant::LOGIN_FACEBOOK;
		Utils::getSingleton().saveUsernameAndPassword(fbToken, "");
	} else {
		Utils::getSingleton().loginType = constant::LOGIN_NORMAL;
		Utils::getSingleton().saveUsernameAndPassword(tfUsername->getText(), tfPassword->getText());
	}
}

void LoginScene::onLoginFacebook(std::string token)
{
	hideWaiting();
	if (token.length() == 0) {
		showPopupNotice(Utils::getSingleton().getStringForKey("error_unknown"), [=]() {});
		return;
	}
	fbToken = token;
	showWaiting();
	SFSRequest::getSingleton().Connect();
}

void LoginScene::onErrorResponse(unsigned char code, std::string msg)
{
	if (code == 49) return;
	hideWaiting();
	if (code == 48) {
		loginNode->setVisible(true);
		registerNode->setVisible(false);
		tfUsername->setText(tfResUname->getText());
		tfPassword->setText(tfResPass->getText());
		Utils::getSingleton().saveUsernameAndPassword(tfUsername->getText(), tfPassword->getText());
	}
	if (msg.length() == 0) return;
	showPopupNotice(msg, [=]() {});
	fbToken = "";
}

void LoginScene::onHttpResponse(int tag, std::string content)
{
	rapidjson::Document d;
	d.Parse<0>(content.c_str());
	if (tag == 1) {
		GameConfig config;
		config.paymentEnabled = d["payment"].GetBool();
		config.zone = d["name"].GetString();
		config.host = d["host"].GetString();
		config.port = d["port"].GetInt();
		config.websocket = d["port"].GetInt();
		config.version = d["version"].GetInt();
		config.versionIOS = d["versionIOS"].GetInt();
		config.ip_rs = d["ip_rs"].GetString();
		config.phone = d["phone"].GetString();
		config.smsVT = d["smsVT"].GetString();
		config.smsVNPVMS = d["smsVNPVMS"].GetString();
		config.smsKH = d["smsKH"].GetString();
		config.smsMK = d["smsMK"].GetString();

		Utils::getSingleton().gameConfig = config;
		labelPhone->setString(config.phone);
		//string location = Utils::getSingleton().getUserCountry();
		//Utils::getSingleton().gameConfig.paymentEnabled = config.paymentEnabled && location.compare("vn") == 0;
	}
	hideWaiting();
}

void LoginScene::onHttpResponseFailed()
{
	if (currentConfigLink == 0) {
		currentConfigLink = 1;
		SFSRequest::getSingleton().RequestHttpGet("http://125.212.207.71/config/configChan.txt", 1);
	} else {
		hideWaiting();
		showPopupNotice(Utils::getSingleton().getStringForKey("error_connection"), [=]() {});
	}
}

void LoginScene::initRegisterNode()
{
	registerNode = Node::create();
	registerNode->setPosition(560, 350);
	registerNode->setVisible(false);
	mLayer->addChild(registerNode);
	Utils::getSingleton().autoScaleNode(registerNode);

	tfResUname = ui::EditBox::create(Size(440, 55), "login/box.png");
	tfResUname->setPosition(Vec2(0, 80));
	tfResUname->setFontName("Arial");
	tfResUname->setFontSize(25);
	tfResUname->setFontColor(Color3B::WHITE);
	tfResUname->setPlaceHolder(Utils::getSingleton().getStringForKey("login_name").c_str());
	tfResUname->setPlaceholderFontColor(Color3B::WHITE);
	tfResUname->setPlaceholderFont("Arial", 25);
	tfResUname->setMaxLength(20);
	tfResUname->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	tfResUname->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	tfResUname->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	tfResUname->setDelegate(this);
	registerNode->addChild(tfResUname);

	tfResPass = ui::EditBox::create(Size(440, 55), "login/box.png");
	tfResPass->setPosition(Vec2(0, 10));
	tfResPass->setFontName("Arial");
	tfResPass->setFontSize(25);
	tfResPass->setFontColor(Color3B::WHITE);
	tfResPass->setPlaceHolder(Utils::getSingleton().getStringForKey("password").c_str());
	tfResPass->setPlaceholderFontColor(Color3B::WHITE);
	tfResPass->setPlaceholderFont("Arial", 25);
	tfResPass->setMaxLength(20);
	tfResPass->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	tfResPass->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	tfResPass->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	tfResPass->setDelegate(this);
	registerNode->addChild(tfResPass);

	tfResPassAgain = ui::EditBox::create(Size(440, 55), "login/box.png");
	tfResPassAgain->setPosition(Vec2(0, -60));
	tfResPassAgain->setFontName("Arial");
	tfResPassAgain->setFontSize(25);
	tfResPassAgain->setFontColor(Color3B::WHITE);
	tfResPassAgain->setPlaceHolder(Utils::getSingleton().getStringForKey("retype_password").c_str());
	tfResPassAgain->setPlaceholderFontColor(Color3B::WHITE);
	tfResPassAgain->setPlaceholderFont("Arial", 25);
	tfResPassAgain->setMaxLength(20);
	tfResPassAgain->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	tfResPassAgain->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	tfResPassAgain->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	tfResPassAgain->setDelegate(this);
	registerNode->addChild(tfResPassAgain);

	/*tfResEmail = ui::EditBox::create(Size(440, 55), "login/box.png");
	tfResEmail->setPosition(Vec2(0, -60));
	tfResEmail->setFontName("Arial");
	tfResEmail->setFontSize(25);
	tfResEmail->setFontColor(Color3B::WHITE);
	tfResEmail->setPlaceHolder("Email");
	tfResEmail->setPlaceholderFontColor(Color3B::WHITE);
	tfResEmail->setPlaceholderFont("Arial", 25);
	tfResEmail->setMaxLength(30);
	tfResEmail->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	tfResEmail->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	tfResEmail->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	tfResEmail->setDelegate(this);
	registerNode->addChild(tfResEmail);*/

	ui::Button* btnBack = ui::Button::create("login/btn_back.png", "login/btn_back_clicked.png");
	btnBack->setPosition(Vec2(-110, -130));
	addTouchEventListener(btnBack, [=]() {
		loginNode->setVisible(true);
		registerNode->setVisible(false);
	});
	registerNode->addChild(btnBack);

	ui::Button* btnRegister = ui::Button::create("login/btn_register.png", "login/btn_register_clicked.png");
	btnRegister->setPosition(Vec2(110, -130));
	addTouchEventListener(btnRegister, [=]() {
		if (Utils::getSingleton().gameConfig.phone.length() == 0) {
			requestGameConfig();
			return;
		}
		if (!Utils::getSingleton().isUsernameValid(tfResUname->getText())
			|| !Utils::getSingleton().isPasswordValid(tfResPass->getText())) {
			showPopupNotice(Utils::getSingleton().getStringForKey("error_password_must_6_char"), [=]() {});
			return;
		}
		string retypePass = tfResPassAgain->getText();
		if (retypePass.compare(tfResPass->getText()) != 0) {
			showPopupNotice(Utils::getSingleton().getStringForKey("error_retype_password"), [=]() {});
			return;
		}
		/*if (!Utils::getSingleton().isEmailValid(tfResEmail->getText())) {
			showPopupNotice(Utils::getSingleton().getStringForKey("error_email_format"), [=]() {});
			return;
		}*/
		showWaiting();
		SFSRequest::getSingleton().Connect();
	});
	registerNode->addChild(btnRegister);
}

void LoginScene::requestGameConfig()
{
	showWaiting();
	//SFSRequest::getSingleton().RequestHttpGet(this, "http://125.212.207.71/config/configChan.txt", 1);
	SFSRequest::getSingleton().RequestHttpGet("http://125.212.192.96:8899/configchan.txt", 1);
}
