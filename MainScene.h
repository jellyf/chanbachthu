#pragma once
#include "BaseScene.h"
#include "Data.h"

class MainScene : public BaseScene, public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(MainScene);
	virtual void onInit();
	virtual void registerEventListenner();
	virtual void unregisterEventListenner();
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

	void onConnected();
	void onConnectionLost();
	void onConfigZoneReceived();
	void onLoginZoneError(short int code, std::string msg);
	void onErrorResponse(unsigned char code, std::string msg);
	void onTableDataResponse(LobbyListTable data);
	void onPlayLogDataResponse(std::vector<PlayLogData> logs);
	void onShopHistoryDataResponse(std::vector<ShopHistoryData> list);
	void onShopItemsDataResponse(std::vector<ShopItemData> list);
	void onExchangeItemResponse(std::string msg);
	void onNewMailResponse(unsigned char count);
	void onListMailDataResponse(std::vector<MailData> list);
	void onMailContentResponse(std::string content);
	void onNewsDataResponse(std::vector<NewsData> list);
protected:
	virtual void onBackScene();
	virtual void showPopupHistory();
	virtual void onChangeMoneyType(int type);
private:
	void initPopupCharge();
	void initPopupGuide();
	void initPopupMail();
	void initPopupNews();
	void initPopupShop();
	void initPopupHistory();
	void initWebView();
	void initPopupDisplayName();

	void showPopupMail();
	void showPopupNews();
	void showWebView(std::string url);
	void addBtnChoosePage(int x, int y, cocos2d::Node* node, std::function<void(int)> funcPage);

	cocos2d::Node* popupShop;
	cocos2d::Node* popupMail;
	cocos2d::Node* popupNews;
	cocos2d::Node* popupGuide;
	cocos2d::Node* popupCharge;
	cocos2d::Node* popupHistory;
	cocos2d::Node* popupDisplayName;
	cocos2d::Node* nodeWebview;

	cocos2d::Label* lbNewMail;

	int currentMoneyType = 0;
	int tmpZoneId = -1;
	bool isBackToLogin = false;
	bool isGoToLobby = false;
	std::string tmpDisplayName;
	std::vector<ShopItemData> listItems;
	std::map<std::string, cocos2d::Texture2D*> textures;
};

