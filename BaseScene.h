#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Data.h"

class BaseScene : public cocos2d::Scene
{
public:
	BaseScene();
	~BaseScene();
	virtual bool init();
	virtual void onInit();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);
	virtual void registerEventListenner();
	virtual void unregisterEventListenner();

	CC_SYNTHESIZE(bool, mIsTouch, IsTouch);
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* _event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* _event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* _event);

	void onUserDataMeResponse();
	void onRankDataResponse(std::vector<std::vector<RankData>> list);
	void onListEventDataResponse(std::vector<EventData> list);
	void onPlayLogDataResponse(std::vector<PlayLogData> logs);
protected:
	virtual void initHeaderWithInfos();
	virtual void onBackScene();
	virtual void onChangeMoneyType(int type);

	void addTouchEventListener(cocos2d::ui::Button* btn, std::function<void()> func, float scale = 1.0f);
	void hideSplash();
	void hideWaiting();
	void hidePopup(cocos2d::Node* popup);
	void initEventView(cocos2d::Vec2 pos, cocos2d::Size size);
	void initPopupNotice();
	void initPopupRank();
	void initPopupSettings();
	void initPopupUserInfo();
	void initPopupHistory();
	void setMoneyType(int type);
	void showPopupNotice(std::string msg, std::function<void()> func, bool showBtnClose = true);
	void showPopupRank(int type);
	void showPopupUserInfo(UserData userData, bool showHistoryIfIsMe = true);
	void showPopupHistory();
	void showSplash();
	void showToast(std::string msg, cocos2d::Vec2 pos, cocos2d::Color3B textColor = cocos2d::Color3B::WHITE, cocos2d::Color3B bgColor = cocos2d::Color3B(80, 80, 80), int bgOpacity = 200);
	void showWaiting();
	void showPopup(cocos2d::Node* popup);
	void setDisplayName(std::string name);
	void runEventView(std::vector<EventData> list, int currentPosX = 1500);
	void addBtnChoosePage(int x, int y, cocos2d::Node* node, std::function<void(int)> funcPage);

	bool hasHeader = false;
	bool isWaiting = false;
	float pingTime = 0;

	cocos2d::ui::Button* moneyBg;
	cocos2d::Sprite* chosenBg;
	cocos2d::Sprite* spWaiting;
	cocos2d::Layer* mLayer;
	cocos2d::Label* lbGold;
	cocos2d::Label* lbSilver;
	cocos2d::Label* lbName;
	cocos2d::Label* lbId;
	cocos2d::Label* lbLevel;

	cocos2d::Node* popupRank;
	cocos2d::Node* popupMainSettings;
	cocos2d::Node* popupUserInfo;
	cocos2d::Node* popupHistory;
	cocos2d::Node* eventView;

	std::vector<cocos2d::ui::Button*> buttons;
	std::vector<cocos2d::ui::Button*> blockedButtons;
	std::vector<cocos2d::Node*> popups;

	cocos2d::Vector<Node*> tmps;
	int tmpIndex;

private:
	cocos2d::ui::Scale9Sprite* splash;
	cocos2d::Node* popupNotice;

	std::vector<std::vector<RankData>> listRanks;
};

