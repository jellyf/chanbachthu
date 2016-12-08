#pragma once
#include "BaseScene.h"
#include "Data.h"

class LobbyScene : public BaseScene
{
public:
	CREATE_FUNC(LobbyScene);
	virtual void onInit();
	virtual void registerEventListenner();
	virtual void unregisterEventListenner();

	void onConnected();
	void onConnectionLost();
	void onLoginZone();
	void onConfigZoneReceived();
	void onLoginZoneError(short int code, std::string msg);
	void onErrorSFSResponse(unsigned char code, std::string msg);
	void onInviteDataResponse(InviteData data);
	void onTableDataResponse(LobbyListTable data);
	void onRoomTypeDataResponse(LobbyListRoomType data);
	void onTableReconnectDataResponse(TableReconnectData data);
protected:
	virtual void onBackScene();
	virtual void onChangeMoneyType(int type);

	bool isBackToMain = false;
	bool isChangeMoney = false;
	int currentRoomType;
	int currentMoneyType;
	std::string tmpZoneName;
	LobbyListRoomType listRoomData;

	cocos2d::ui::ScrollView* scrollListRoom;
	cocos2d::ui::ScrollView* scrollListTable;
	std::vector<cocos2d::ui::Button*> vecTables;
	std::vector<cocos2d::Vec2> playerPos;
};

