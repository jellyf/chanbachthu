#pragma once
#include "Utils.h"
#include "LoginScene.h"
#include "MainScene.h"
#include "LobbyScene.h"
#include "GameScene.h"
#include "Constant.h"
#include "SFSRequest.h"

using namespace std;
using namespace cocos2d;

template<> Utils* SingLeton<Utils>::mSingleton = 0;
Utils* Utils::getSingletonPtr(void)
{
	return mSingleton;
}
Utils& Utils::getSingleton(void)
{
	assert(mSingleton);  return (*mSingleton);
}

Utils::Utils()
{
	isRunningEvent = false;
	currentEventPosX = constant::EVENT_START_POSX;
	userDataMe.UserID = 0;
	viLang = cocos2d::FileUtils::getInstance()->getValueMapFromFile("lang/vi.xml");
	SFSRequest::getSingleton().onLoadTextureResponse = std::bind(&Utils::onLoadTextureResponse, this, std::placeholders::_1, std::placeholders::_2);
}

Utils::~Utils()
{
}

string Utils::formatMoneyWithComma(double money) {
	stringstream ss;
	//ss.imbue(locale("de-VI"));
	ss << fixed << money;
	string ret = ss.str();
	int index = ret.find_last_of('.');
	ret = ret.substr(0, index);

	unsigned int i = 3;
	while (i < ret.length())
	{
		ret.insert(ret.length() - i, ".");
		i = i + 3 + 1;
	}
	return ret;
}

std::string Utils::getStringForKey(std::string key)
{
	return viLang[key].asString();
}

std::string Utils::replaceString(std::string str, std::string strSearch, std::string strReplace)
{
	std::string s = str;
	for (size_t pos = 0; ; pos += strReplace.length()) {
		pos = s.find(strSearch, pos);
		if (pos == string::npos)
			break;

		s.erase(pos, strSearch.length());
		s.insert(pos, strReplace);
	}
	return s;
}

std::string Utils::trim(std::string str)
{
	if (str.length() == 0) {
		return str;
	}
	int i = 0;
	int j = str.length() - 1;
	while (i < str.length() && str.at(i) == ' ') i++;
	while (j >= 0 && str.at(j) == ' ') j--;
	return str.substr(i, j + 1);
}

bool Utils::isEmailValid(std::string email)
{
	int length = email.length();
	for (int i = 0; i < length; i++) {
		char c = email.at(i);
		if (!(c == '.' || c == '_' || c == '@' || c == '-' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			return false;
		}
	}
	return true;
}

bool Utils::isUsernameValid(std::string uname)
{
	int length = uname.length();
	if (length < 6 || length > 16) {
		return false;
	}
	for (int i = 0; i < length; i++) {
		char c = uname.at(i);
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			return false;
		}
	}
	return true;
}

bool Utils::isPasswordValid(std::string pw)
{
	int length = pw.length();
	if (length < 6 || length > 16) {
		return false;
	}
	for (int i = 0; i < length; i++) {
		char c = pw.at(i);
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			return false;
		}
	}
	return true;
}

bool Utils::isDisplayNameValid(std::string displayname)
{
	int length = displayname.length();
	if (length < 6 || length > 16) {
		return false;
	}
	for (int i = 0; i < length; i++) {
		char c = displayname.at(i);
		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
			return false;
		}
	}
	return true;
}

void Utils::split(const std::string & s, char delim, std::vector<std::string>& elems)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

void Utils::autoScaleNode(Node * pSender)
{
	auto vizibleSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto designResolutionSize = cocos2d::Size(1280, 800);
	float x0 = designResolutionSize.width / designResolutionSize.height;
	float x1 = vizibleSize.width / vizibleSize.height;
	if (x1 > x0)
	{
		float scaleX = x0 / x1;
		pSender->setScaleX(scaleX*pSender->getScaleX());
	} else
	{
		float scaleY = x1 / x0;
		pSender->setScaleY(scaleY*pSender->getScaleY());
	}
}

void Utils::replaceScene(cocos2d::Scene* newScene) {
	if (currentScene != nullptr) {
		currentScene->unscheduleUpdate();
	}
	currentScene = newScene;
	cocos2d::Director::getInstance()->replaceScene(newScene);
}

void Utils::goToLoginScene()
{
	logoutGame();
	replaceScene(LoginScene::create());
}

void Utils::goToMainScene()
{
	replaceScene(MainScene::create());
}

void Utils::goToLobbyScene()
{
	replaceScene(LobbyScene::create());
}

void Utils::goToGameScene()
{
	replaceScene(GameScene::create());
}

void Utils::saveUsernameAndPassword(std::string username, std::string password)
{
	this->username = username;
	this->password = password.length() == 0 ? "" : md5(password);
	if (password.length() > 0) {
		UserDefault::getInstance()->setStringForKey(constant::KEY_USERNAME.c_str(), username);
		UserDefault::getInstance()->setStringForKey(constant::KEY_PASSWORD.c_str(), password);
	}
}

void Utils::logoutGame()
{
	userDataMe.UserID = 0;
	userDataMe.Name = "";
	logoutZone();
}

void Utils::logoutZone()
{
	currentRoomId = 0;
	currentLobbyId = 0;
	currentZoneName = "";
	currentRoomName = "";
	currentLobbyName = "";
	lobbyListTable.Size = 0;
	lobbyListRoomType.ListRoomType.clear();
}

void Utils::LoadTextureFromURL(std::string url, std::function<void(cocos2d::Texture2D*)> func)
{
	if (textures[url] != nullptr) {
		func(textures[url]);
	} else {
		callbacks[url].push_back(func);
		if (callbacks[url].size() == 1) {
			SFSRequest::getSingleton().LoadTextureFromURL(url);
		}
	}
}

void Utils::onLoadTextureResponse(std::string url, cocos2d::Texture2D * texture)
{
	textures[url] = texture;
	for (auto func : callbacks[url]) {
		func(texture);
	}
	callbacks[url].clear();
}

void Utils::openSMS(std::string number, std::string text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (!JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "openSMS", "(Ljava/lang/String;Ljava/lang/String;)V")) {
		return;
	}
	jstring jaddress = methodInfo.env->NewStringUTF(number.c_str());
	jstring jsmsBody = methodInfo.env->NewStringUTF(text.c_str());
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jaddress, jsmsBody);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void Utils::openTel(std::string number)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (!JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "openCall", "(Ljava/lang/String;)V")) {
		return;
	}
	jstring jphone = methodInfo.env->NewStringUTF(number.c_str());
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jphone);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}

void Utils::loginFacebook()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	if (!JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "loginFacebook", "()V")) {
		return;
	}
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
}
