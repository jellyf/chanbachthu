#pragma once
#include "cocos2d.h"
#define __DECLARE_CMD_PROPERTY(name, value) const std::string name(value);
namespace cmd
{
	__DECLARE_CMD_PROPERTY(LOGIN, "710");
	__DECLARE_CMD_PROPERTY(REGISTER, "711");
	__DECLARE_CMD_PROPERTY(LOGIN_FACEBOOK, "712");
	__DECLARE_CMD_PROPERTY(USER_INFO, "96");
	__DECLARE_CMD_PROPERTY(JOIN_ROOM, "7");
	__DECLARE_CMD_PROPERTY(REQUEST_PING, "85");
	__DECLARE_CMD_PROPERTY(RESPONSE_PING, "86");
	__DECLARE_CMD_PROPERTY(REQUEST_CHARGE_CARD, "80");
	__DECLARE_CMD_PROPERTY(REQUEST_RANK, "44");
	__DECLARE_CMD_PROPERTY(RESPONSE_RANK, "45");
	__DECLARE_CMD_PROPERTY(RESPONSE_CONFIG_ZONE, "118");
	__DECLARE_CMD_PROPERTY(RESPONSE_UPDATE_USERINFO, "95");
	__DECLARE_CMD_PROPERTY(VIEW_USER_INFO, "92");
	__DECLARE_CMD_PROPERTY(PLAY_HISTORY, "714");
	__DECLARE_CMD_PROPERTY(SHOP_HISTORY, "122");
	__DECLARE_CMD_PROPERTY(SHOP_ITEMS, "120");
	__DECLARE_CMD_PROPERTY(EXCHANGE_ITEM, "121");
	__DECLARE_CMD_PROPERTY(NEW_MAIL, "718");
	__DECLARE_CMD_PROPERTY(LIST_MAIL, "719");
	__DECLARE_CMD_PROPERTY(MAIL_CONTENT, "720");
	__DECLARE_CMD_PROPERTY(UPDATE_DISPLAYNAME, "119");
	__DECLARE_CMD_PROPERTY(NEWS, "124");
	__DECLARE_CMD_PROPERTY(EVENTS, "213");
	__DECLARE_CMD_PROPERTY(CURRENT_TABLE_RECONNECT, "61");

	__DECLARE_CMD_PROPERTY(GAME_ROOM_DATA, "25");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_ROOM_DATA_GA, "78");
	__DECLARE_CMD_PROPERTY(GAME_ROOM_DATA_GA, "79");
	__DECLARE_CMD_PROPERTY(GAME_READY, "16");
	__DECLARE_CMD_PROPERTY(GAME_START, "4");
	__DECLARE_CMD_PROPERTY(GAME_CHOOSE_STILT, "70");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_STILT, "71");
	__DECLARE_CMD_PROPERTY(GAME_CHOOSE_HOST, "72");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_HOST, "73");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_BASH, "132");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_BASH, "8");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_BASH_BACK, "17");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_BASH_BACK, "18");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_HOLD, "11");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_HOLD, "12");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_PUNISH, "13");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_PICK, "9");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_PICK, "10");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_PENET, "14");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_PENET, "15");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_WIN, "19");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_WIN, "99");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_DROP_WIN, "74");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_DROP_WIN, "76");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_CALL_CREST, "23");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_CALL_CREST, "24");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_FORWARD, "5");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_FORWARD, "6");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_END_MATCH, "20");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_END_MATCH_TIE, "76");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_END_MATCH_MONEY, "117");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_TABLE_INFO, "31");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_TABLE_INFO, "32");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_LOBBY_USER, "27");
	__DECLARE_CMD_PROPERTY(GAME_REQUEST_INVITE_PLAYER, "39");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_PLAYING_DATA, "26");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_SPECTATOR_DATA, "82");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_MY_RECONNECT_DATA, "63");
	__DECLARE_CMD_PROPERTY(GAME_RESPONSE_USER_RECONNECT_DATA, "68");

	__DECLARE_CMD_PROPERTY(LOBBY_RESPONSE_DATA_ROOM, "30");
	__DECLARE_CMD_PROPERTY(LOBBY_RESPONSE_DATA_ROOM_TYPE, "k");
	__DECLARE_CMD_PROPERTY(LOBBY_RESPONSE_DATA_USER, "28");
	__DECLARE_CMD_PROPERTY(LOBBY_RESPONSE_INVITE_PLAYER, "40");
}