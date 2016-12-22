#pragma once
#include "BaseScene.h"
#include "md5.h"
class LoginScene : public BaseScene, public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(LoginScene);
	virtual void onInit();
	virtual void registerEventListenner();
	virtual void unregisterEventListenner();
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

	void onConnected();
	void onLoginZone();
	void onConnectionFailed();
	void onConfigZoneReceived();
	void onUserDataMeResponse();
	void onLoginFacebook(std::string token);
	void onErrorResponse(unsigned char code, std::string msg);
	void onHttpResponse(int tag, std::string content);
	void onHttpResponseFailed();
protected:
	virtual void onKeyBack();
private:
	void loginNormal();
	void loginFacebook();
	void initRegisterNode();
	void requestGameConfig();
    
	int waitingLogin;
	int currentConfigLink = 0;
	std::string fbToken = "";
    bool isIPv4 = true;

	cocos2d::Node* loginNode;
	cocos2d::Node* registerNode;
	cocos2d::ui::EditBox* tfUsername;
	cocos2d::ui::EditBox* tfPassword;
	cocos2d::ui::EditBox* tfResUname;
	cocos2d::ui::EditBox* tfResPass;
	cocos2d::ui::EditBox* tfResPassAgain;
	//cocos2d::ui::EditBox* tfResEmail;
	cocos2d::Label* labelPhone;
};

