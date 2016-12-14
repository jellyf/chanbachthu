#pragma once
#include "SFSConnector.h"
#include "SFSResponse.h"
#include "Constant.h"
#include "Data.h"
#include "Command.h"
#include "Utils.h"
#include "EventHandler.h"

#include "Bitswarm/BitSwarmEvent.h"
#include "Util/EventDispatcher.h"
#include "Requests/LoginRequest.h"
#include "Requests/LogoutRequest.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/PublicMessageRequest.h"
#include "Requests/JoinRoomRequest.h"
#include "Requests/LeaveRoomRequest.h"

template<> SFSConnector* SingLeton<SFSConnector>::mSingleton = 0;
SFSConnector* SFSConnector::getSingletonPtr(void)
{
	return mSingleton;
}
SFSConnector& SFSConnector::getSingleton(void)
{
	assert(mSingleton);  return (*mSingleton);
}

SFSConnector::SFSConnector()
{
	mSmartFox = boost::shared_ptr<Sfs2X::SmartFox>();
}


SFSConnector::~SFSConnector()
{
}

void SFSConnector::InitializeSmartFox()
{
	// Initialize Smart Fox
	mSmartFox = boost::shared_ptr<Sfs2X::SmartFox>(new Sfs2X::SmartFox(true));
	mSmartFox->ThreadSafeMode(true);

	// Add event listeners
	mSmartFox->AddEventListener(SFSEvent::CONNECTION, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxConnection, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::CONNECTION_LOST, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxConnectionLost, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::LOGIN, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxLoginZone, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::LOGIN_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxLoginZoneError, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::LOGOUT, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxLogoutZone, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::ROOM_JOIN, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxRoomJoin, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::ROOM_JOIN_ERROR, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxRoomJoinError, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::USER_EXIT_ROOM, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnUserExitRoom, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::USER_ENTER_ROOM, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnUserEnterRoom, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::EXTENSION_RESPONSE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnExtensionResponse, (unsigned long long)this)));
	mSmartFox->AddEventListener(SFSEvent::PUBLIC_MESSAGE, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnPublicMessage, (unsigned long long)this)));
	mSmartFox->AddEventListener(BitSwarmEvent::DISCONNECT, boost::shared_ptr<EventListenerDelegate>(new EventListenerDelegate(SFSConnector::OnSmartFoxDisconnection, (unsigned long long)this)));
}

void SFSConnector::OnSmartFoxConnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSuccess = (*ptrEventParams)["success"];
	boost::shared_ptr<bool> ptrErrorMessage = ((boost::static_pointer_cast<bool>))(ptrEventParamValueSuccess);

	if (*ptrErrorMessage == true)
	{
		CCLOG("OnSmartFoxConnection: successful");
		// Connection with SmartFox Server has been established
		// Set internal event to notify the connection establishment
		//SFSConnector::getSingleton().LoginZone("", "g", constant::ZONE_AUTHEN);
		if (EventHandler::getSingleton().onConnected != NULL) {
			EventHandler::getSingleton().onConnected();
		}
	} else {
		CCLOG("OnSmartFoxConnection: failed");
		if (EventHandler::getSingleton().onConnectionFailed != NULL) {
			EventHandler::getSingleton().onConnectionFailed();
		}
	}
}

// -------------------------------------------------------------------------
// OnSmartFoxConnectionLost
// -------------------------------------------------------------------------
void SFSConnector::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	CCLOG("OnSmartFoxConnectionLost");
	if (EventHandler::getSingleton().onConnectionLost != NULL) {
		EventHandler::getSingleton().onConnectionLost();
	}
}

// -------------------------------------------------------------------------
// OnSmartFoxDisconnection
// -------------------------------------------------------------------------
void SFSConnector::OnSmartFoxDisconnection(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	CCLOG("OnSmartFoxDisconnection");
	if (EventHandler::getSingleton().onDisconnected != NULL) {
		EventHandler::getSingleton().onDisconnected();
	}
}

// -------------------------------------------------------------------------
// OnSmartFoxLogin
// -------------------------------------------------------------------------
void SFSConnector::OnSmartFoxLoginZone(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	CCLOG("OnSmartFoxLoginZone");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueZone = (*ptrEventParams)["zone"];
	boost::shared_ptr<string> ptrNotifiedZone = ((boost::static_pointer_cast<string>))(ptrEventParamValueZone);

	Utils::getSingleton().currentZoneName = *ptrNotifiedZone;
	CCLOG("%s", ptrNotifiedZone->c_str());
	/*for (auto iter = ptrEventParams->begin(); iter != ptrEventParams->end(); iter++) {
		CCLOG("%s", iter->first.c_str());
	}*/

	if (EventHandler::getSingleton().onLoginZone != NULL) {
		EventHandler::getSingleton().onLoginZone();
	}
}

// -------------------------------------------------------------------------
// OnSmartFoxLoginError
// -------------------------------------------------------------------------
void SFSConnector::OnSmartFoxLoginZoneError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	// Get the error message parameter of the event
	// Get the error code parameter of the event
	boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
	boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);

	boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
	boost::shared_ptr<short int> ptrErrorCode = ((boost::static_pointer_cast<short int>))(ptrEventParamValueErrorCode);

	if (EventHandler::getSingleton().onLoginZoneError != NULL) {
		EventHandler::getSingleton().onLoginZoneError(*ptrErrorCode, *ptrErrorMessage);
	}
	string message = "OnSmartFoxLoginZoneError: " + *ptrErrorMessage;
	CCLOG("%d %s", *ptrErrorCode, message.c_str());
}

void SFSConnector::OnSmartFoxLogoutZone(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	CCLOG("OnSmartFoxLogoutZone");
	Utils::getSingleton().logoutZone();
	if (EventHandler::getSingleton().onLogoutZone != NULL) {
		EventHandler::getSingleton().onLogoutZone();
	}
}

void SFSConnector::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//room joined
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
	
	Utils::getSingleton().currentRoomId = ptrNotifiedRoom->Id();
	Utils::getSingleton().currentRoomName = *(ptrNotifiedRoom->Name());
	CCLOG("RoomJoin: %d %s", ptrNotifiedRoom->Id(), ptrNotifiedRoom->Name()->c_str());
	if (EventHandler::getSingleton().onJoinRoom != NULL) {
		EventHandler::getSingleton().onJoinRoom(ptrNotifiedRoom->Id(), *(ptrNotifiedRoom->Name()));
	}
}

void SFSConnector::OnSmartFoxRoomJoinError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueErrorMessage = (*ptrEventParams)["errorMessage"];
	boost::shared_ptr<string> ptrErrorMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueErrorMessage);

	CCLOG("RoomJoinError: %s", (*ptrErrorMessage).c_str());
	if (EventHandler::getSingleton().onJoinRoomError != NULL) {
		EventHandler::getSingleton().onJoinRoomError(*ptrErrorMessage);
	}
}

void SFSConnector::OnUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//get the user parameter of the event
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	//get the room parameter of the event
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);

	CCLOG("%s leaved room %s", ptrNotifiedUser->Name()->c_str(), ptrNotifiedRoom->Name()->c_str());
	if (EventHandler::getSingleton().onUserExitRoom != NULL) {
		EventHandler::getSingleton().onUserExitRoom(ptrNotifiedUser->Id());
	}
}

void SFSConnector::OnUserEnterRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//get the user parameter of the event
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	//get the room parameter of the event
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
	boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);

	CCLOG("%s enter room %s", ptrNotifiedUser->Name()->c_str(), ptrNotifiedRoom->Name()->c_str());
}

void SFSConnector::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEventParams)["cmd"];
	boost::shared_ptr<string> ptrNotifiedCmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEventParams)["params"];
	boost::shared_ptr<ISFSObject> ptrNotifiedISFSObject = ((boost::static_pointer_cast<ISFSObject>)(ptrEventParamValueParams));

	CCLOG("OnExtensionResponse::cmd::%s", (*ptrNotifiedCmd).c_str());
	/*vector<string> vt = *(ptrNotifiedISFSObject->GetKeys());
	for (string s : vt) {
		CCLOG("%s", s.c_str());
	}*/
	if (ptrNotifiedCmd->compare(std::string("err")) == 0) {
		ErrorData error;
		SFSResponse::getErrorData(ptrNotifiedISFSObject, error);
		CCLOG("%d %s", error.Code, error.Message.c_str());
		if (EventHandler::getSingleton().onErrorSFSResponse != NULL) {
			EventHandler::getSingleton().onErrorSFSResponse(error.Code, error.Message);
		}
	} else if (ptrNotifiedCmd->compare(cmd::RESPONSE_PING) == 0) {
		boost::shared_ptr<long long> ptrTime = ptrNotifiedISFSObject->GetLong("s");
		//CCLOG("%d", *ptrTime);
		boost::shared_ptr<ISFSObject> parameters(new SFSObject());
		parameters->PutLong("s", *ptrTime);
		SFSConnector::getSingleton().SendExtensionRequest(cmd::REQUEST_PING, parameters);
	} else if (ptrNotifiedCmd->compare(cmd::RESPONSE_CONFIG_ZONE) == 0) {
		SFSResponse::getConfigZone(ptrNotifiedISFSObject, Utils::getSingleton().zones);
		if (EventHandler::getSingleton().onConfigZoneReceived != NULL) {
			EventHandler::getSingleton().onConfigZoneReceived();
		}
	} else if (ptrNotifiedCmd->compare(cmd::REGISTER) == 0) {

	} else if (ptrNotifiedCmd->compare(cmd::USER_INFO) == 0) {
		//if(Utils::getSingleton().userDataMe.UserID == 0)
		SFSResponse::getUserData(ptrNotifiedISFSObject, Utils::getSingleton().userDataMe);
		if (Utils::getSingleton().moneyType == -1) {
			Utils::getSingleton().moneyType = Utils::getSingleton().userDataMe.MoneyType;
		}
		if (EventHandler::getSingleton().onUserDataMeSFSResponse != NULL) {
			EventHandler::getSingleton().onUserDataMeSFSResponse();
		}
	} else if (ptrNotifiedCmd->compare(cmd::RESPONSE_UPDATE_USERINFO) == 0) {
		SFSResponse::getUserData(ptrNotifiedISFSObject, Utils::getSingleton().userDataMe);
		if (EventHandler::getSingleton().onUserDataMeSFSResponse != NULL) {
			EventHandler::getSingleton().onUserDataMeSFSResponse();
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_ROOM_DATA) == 0) {
		RoomData roomData;
		SFSResponse::getRoomData(ptrNotifiedISFSObject, roomData);
		if (EventHandler::getSingleton().onRoomDataSFSResponse != NULL) {
			EventHandler::getSingleton().onRoomDataSFSResponse(roomData);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_START) == 0) {
		StartGameData startData;
		SFSResponse::getStartGameData(ptrNotifiedISFSObject, startData);
		if (EventHandler::getSingleton().onStartGameDataSFSResponse != NULL) {
			EventHandler::getSingleton().onStartGameDataSFSResponse(startData);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_STILT) == 0) {
		boost::shared_ptr<ByteArray> byteArray = ptrNotifiedISFSObject->GetByteArray("d");
		unsigned char stilt;
		byteArray->ReadByte(stilt);
		if (EventHandler::getSingleton().onChooseStiltSFSResponse != NULL) {
			EventHandler::getSingleton().onChooseStiltSFSResponse(stilt);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_HOST) == 0) {
		boost::shared_ptr<ByteArray> byteArray = ptrNotifiedISFSObject->GetByteArray("d");
		unsigned char stilt1, stilt2, host;
		byteArray->ReadByte(stilt1);
		//byteArray->ReadByte(stilt2);
		stilt2 = 1;
		byteArray->ReadByte(host);
		if (EventHandler::getSingleton().onChooseHostSFSResponse != NULL) {
			EventHandler::getSingleton().onChooseHostSFSResponse(stilt1, stilt2, host);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_BASH) == 0) {
		BashData data;
		SFSResponse::getBashData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onUserBashSFSResponse != NULL) {
			EventHandler::getSingleton().onUserBashSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_BASH_BACK) == 0) {
		BashBackData data;
		SFSResponse::getBashBackData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onUserBashBackSFSResponse != NULL) {
			EventHandler::getSingleton().onUserBashBackSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_HOLD) == 0) {
		HoldData data;
		SFSResponse::getHoldData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onUserHoldSFSResponse != NULL) {
			EventHandler::getSingleton().onUserHoldSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_PICK) == 0) {
		PickData data;
		SFSResponse::getPickData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onUserPickSFSResponse != NULL) {
			EventHandler::getSingleton().onUserPickSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_PENET) == 0) {
		PenetData data;
		SFSResponse::getPenetData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onUserPenetSFSResponse != NULL) {
			EventHandler::getSingleton().onUserPenetSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_FORWARD) == 0) {
		ForwardData data;
		SFSResponse::getForwardData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onUserForwardSFSResponse != NULL) {
			EventHandler::getSingleton().onUserForwardSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_WIN) == 0) {
		boost::shared_ptr<ByteArray> byteArray = ptrNotifiedISFSObject->GetByteArray("d");
		long uid;
		unsigned char sound;
		byteArray->ReadInt(uid);
		byteArray->ReadByte(sound);
		if (EventHandler::getSingleton().onUserWinSFSResponse != NULL) {
			EventHandler::getSingleton().onUserWinSFSResponse(uid, sound);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_CALL_CREST) == 0) {
		CrestResponseData data;
		SFSResponse::getCrestResponseData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onCrestResponseSFSResponse != NULL) {
			EventHandler::getSingleton().onCrestResponseSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_END_MATCH) == 0) {
		EndMatchData data;
		SFSResponse::getEndMatchData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onEndMatchSFSResponse != NULL) {
			EventHandler::getSingleton().onEndMatchSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_END_MATCH_TIE) == 0) {
		vector<unsigned char> stiltCards;
		SFSResponse::getEndMatchTieData(ptrNotifiedISFSObject, stiltCards);
		if (EventHandler::getSingleton().onEndMatchTieSFSResponse != NULL) {
			EventHandler::getSingleton().onEndMatchTieSFSResponse(stiltCards);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_END_MATCH_MONEY) == 0) {
		EndMatchMoneyData data;
		SFSResponse::getEndMatchMoneyData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onEndMatchMoneySFSResponse != NULL) {
			EventHandler::getSingleton().onEndMatchMoneySFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::LOBBY_RESPONSE_DATA_ROOM) == 0) {
		LobbyListTable data;
		SFSResponse::getLobbyTableData(ptrNotifiedISFSObject, data);
		Utils::getSingleton().lobbyListTable = data;
		if (EventHandler::getSingleton().onLobbyTableSFSResponse != NULL) {
			EventHandler::getSingleton().onLobbyTableSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::LOBBY_RESPONSE_DATA_ROOM_TYPE) == 0) {
		LobbyListRoomType data;
		SFSResponse::getLobbyRoomTypeData(ptrNotifiedISFSObject, data);
		Utils::getSingleton().lobbyListRoomType = data;
		Utils::getSingleton().moneyType = (int)data.IsRealMoney;
		if (EventHandler::getSingleton().onLobbyRoomTypeSFSResponse != NULL) {
			EventHandler::getSingleton().onLobbyRoomTypeSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_PUNISH) == 0) {
		long uid;
		string msg;
		SFSResponse::getGamePunishData(ptrNotifiedISFSObject, uid, msg);
		if (EventHandler::getSingleton().onGamePunishSFSResponse != NULL) {
			EventHandler::getSingleton().onGamePunishSFSResponse(uid, msg);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_READY) == 0) {
		long uid;
		SFSResponse::getGameReadyData(ptrNotifiedISFSObject, uid);
		if (EventHandler::getSingleton().onGameReadySFSResponse != NULL) {
			EventHandler::getSingleton().onGameReadySFSResponse(uid);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_TABLE_INFO) == 0) {
		GameTableData data;
		SFSResponse::getGameTableData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onGameTableSFSResponse != NULL) {
			EventHandler::getSingleton().onGameTableSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::LOBBY_RESPONSE_DATA_USER) == 0) {
		std::vector<UserData> listUser;
		SFSResponse::getLobbyUserData(ptrNotifiedISFSObject, listUser);
		if (EventHandler::getSingleton().onLobbyUserDataSFSResponse != NULL) {
			EventHandler::getSingleton().onLobbyUserDataSFSResponse(listUser);
		}
	} else if (ptrNotifiedCmd->compare(cmd::LOBBY_RESPONSE_INVITE_PLAYER) == 0) {
		InviteData data;
		SFSResponse::getLobbyInviteData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onLobbyInviteDataSFSResponse != NULL) {
			EventHandler::getSingleton().onLobbyInviteDataSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_ROOM_DATA_GA) == 0) {
		bool isGa;
		double gaMoney;
		SFSResponse::getGameRoomDataGa(ptrNotifiedISFSObject, isGa, gaMoney);
		if (EventHandler::getSingleton().onGameRoomDataGaSFSResponse != NULL) {
			EventHandler::getSingleton().onGameRoomDataGaSFSResponse(isGa, gaMoney);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_PLAYING_DATA) == 0) {
		PlayingTableData data;
		SFSResponse::getGamePlayingTableData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onGamePlayingDataSFSResponse != NULL) {
			EventHandler::getSingleton().onGamePlayingDataSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_SPECTATOR_DATA) == 0) {
		std::vector<PlayerData> list;
		SFSResponse::getGameSpectatorData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onGameSpectatorDataSFSResponse != NULL) {
			EventHandler::getSingleton().onGameSpectatorDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::RESPONSE_RANK) == 0) {
		std::vector<std::vector<RankData>> list;
		SFSResponse::getRankData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onRankDataSFSResponse != NULL) {
			EventHandler::getSingleton().onRankDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::VIEW_USER_INFO) == 0) {
		UserData data;
		SFSResponse::getUserData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onUserDataSFSResponse != NULL) {
			EventHandler::getSingleton().onUserDataSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::PLAY_HISTORY) == 0) {
		std::vector<PlayLogData> list;
		SFSResponse::getPlayLogData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onPlayLogDataSFSResponse != NULL) {
			EventHandler::getSingleton().onPlayLogDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_MY_RECONNECT_DATA) == 0) {
		GameReconnectData data;
		SFSResponse::getGameMyReconnectData(ptrNotifiedISFSObject, data);
		if (EventHandler::getSingleton().onGameMyReconnectDataSFSResponse != NULL) {
			EventHandler::getSingleton().onGameMyReconnectDataSFSResponse(data);
		}
	} else if (ptrNotifiedCmd->compare(cmd::GAME_RESPONSE_USER_RECONNECT_DATA) == 0) {
		std::vector<UserReconnectData> list;
		SFSResponse::getGameUserReconnectData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onGameUserReconnectDataSFSResponse != NULL) {
			EventHandler::getSingleton().onGameUserReconnectDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::SHOP_HISTORY) == 0) {
		std::vector<ShopHistoryData> list;
		SFSResponse::getShopHistoryData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onShopHistoryDataSFSResponse != NULL) {
			EventHandler::getSingleton().onShopHistoryDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::SHOP_ITEMS) == 0) {
		std::vector<ShopItemData> list;
		SFSResponse::getShopItemsData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onShopItemsDataSFSResponse != NULL) {
			EventHandler::getSingleton().onShopItemsDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::EXCHANGE_ITEM) == 0) {
		std::string msg;
		boost::shared_ptr<ByteArray> byteArray = ptrNotifiedISFSObject->GetByteArray("d");
		byteArray->ReadUTF(msg);
		if (EventHandler::getSingleton().onExchangeItemSFSResponse != NULL) {
			EventHandler::getSingleton().onExchangeItemSFSResponse(msg);
		}
	} else if (ptrNotifiedCmd->compare(cmd::NEW_MAIL) == 0) {
		unsigned char count;
		boost::shared_ptr<ByteArray> byteArray = ptrNotifiedISFSObject->GetByteArray("d");
		byteArray->ReadByte(count);
		if (EventHandler::getSingleton().onNewMailSFSResponse != NULL) {
			EventHandler::getSingleton().onNewMailSFSResponse(count);
		}
	} else if (ptrNotifiedCmd->compare(cmd::LIST_MAIL) == 0) {
		std::vector<MailData> list;
		SFSResponse::getListMailData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onListMailDataSFSResponse != NULL) {
			EventHandler::getSingleton().onListMailDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::MAIL_CONTENT) == 0) {
		long tmp;
		std::string content;
		boost::shared_ptr<ByteArray> byteArray = ptrNotifiedISFSObject->GetByteArray("d");
		if (byteArray->Length() > 0) {
			byteArray->ReadInt(tmp);
			byteArray->ReadUTF(content);
		}
		if (EventHandler::getSingleton().onMailContentSFSResponse != NULL) {
			EventHandler::getSingleton().onMailContentSFSResponse(content);
		}
	} else if (ptrNotifiedCmd->compare(cmd::NEWS) == 0) {
		std::vector<NewsData> list;
		SFSResponse::getNewsData(ptrNotifiedISFSObject, list);
		if (EventHandler::getSingleton().onNewsDataSFSResponse != NULL) {
			EventHandler::getSingleton().onNewsDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::EVENTS) == 0) {
		std::vector<EventData> list;
		SFSResponse::getListEventData(ptrNotifiedISFSObject, list);
		Utils::getSingleton().events = list;
		Utils::getSingleton().currentEventPosX = constant::EVENT_START_POSX;
		if (EventHandler::getSingleton().onListEventDataSFSResponse != NULL) {
			EventHandler::getSingleton().onListEventDataSFSResponse(list);
		}
	} else if (ptrNotifiedCmd->compare(cmd::CURRENT_TABLE_RECONNECT) == 0) {
		TableReconnectData data;
		SFSResponse::getTableReconnectData(ptrNotifiedISFSObject, data);
		Utils::getSingleton().tableReconnectData = data;
		if (EventHandler::getSingleton().onTableReconnectDataSFSResponse != NULL) {
			EventHandler::getSingleton().onTableReconnectDataSFSResponse(data);
		}
	}
}

void SFSConnector::OnPublicMessage(unsigned long long ptrContext, boost::shared_ptr<Sfs2X::Core::BaseEvent> ptrEvent)
{
	//get the user parameter of the event
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	//get the message parameter of the event
	boost::shared_ptr<void> ptrEventParamValueMsg = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMsg = ((boost::static_pointer_cast<string>))(ptrEventParamValueMsg);

	if(EventHandler::getSingleton().onPublicMessage != NULL) {
		EventHandler::getSingleton().onPublicMessage(ptrNotifiedUser->Id(), *ptrNotifiedMsg);
	}
}

void SFSConnector::Connect(std::string host, int port)
{
	SFSConnector::getSingleton().InitializeSmartFox();
	/*if (mSmartFox->IsConnected()) {
		if (EventHandler::getSingleton().onLoginZone != NULL) {
			EventHandler::getSingleton().onLoginZone();
		}
		return;
	}*/
	CCLOG("Connect to SmartFox: %s %d", host.c_str(), port);
	mSmartFox->Connect(host.c_str(), port);
}

void SFSConnector::Disconnect()
{
	mSmartFox->Disconnect();
}

void SFSConnector::ProcessEvents()
{
	if (mSmartFox) {
		mSmartFox->ProcessEvents();
	}
}

void SFSConnector::LoginZone(string username, string password, string zone) 
{
	boost::shared_ptr<IRequest> request(new LoginRequest(username, password, zone));
	mSmartFox->Send(request);
}

void SFSConnector::LogoutZone()
{
	boost::shared_ptr<IRequest> request(new LogoutRequest());
	mSmartFox->Send(request);
}

void SFSConnector::RequestLeaveRoom()
{
	boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
	mSmartFox->Send(request);
}

void SFSConnector::SendPublicMessage(std::string msg, boost::shared_ptr<ISFSObject> params, boost::shared_ptr<Room> room)
{
	boost::shared_ptr<IRequest> request(new PublicMessageRequest(msg, params, room));
	mSmartFox->Send(request);
}

void SFSConnector::SendExtensionRequest(std::string cmd, boost::shared_ptr<ISFSObject> params, boost::shared_ptr<Room> room)
{
	lastRequestCmd = cmd;
	CCLOG("SendExtensionRequest: %s", cmd.c_str());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(cmd, params, room));
	mSmartFox->Send(request);
}
