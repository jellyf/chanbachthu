#pragma once
#include "cocos2d.h"
#define __DECLARE_STRING_PROPERTY(name, value) const std::string name(value);
#define __DECLARE_INT_PROPERTY(name, value) const int name(value);
namespace constant
{
	/*__DECLARE_STRING_PROPERTY(ZONE_AUTHEN, "Authen");
	__DECLARE_STRING_PROPERTY(ZONE_NHATRANH, "NhaTranh");
	__DECLARE_STRING_PROPERTY(ZONE_DINHLANG, "SanDinh");
	__DECLARE_STRING_PROPERTY(ZONE_PHUCHUA, "VuongPhu");
	__DECLARE_STRING_PROPERTY(ZONE_LOIDAI, "DauTruong");*/

	__DECLARE_STRING_PROPERTY(PROVIDER_VIETTEL, "viettel");
	__DECLARE_STRING_PROPERTY(PROVIDER_VINAPHONE, "vinaphone");
	__DECLARE_STRING_PROPERTY(PROVIDER_MOBIFONE, "mobifone");

	__DECLARE_STRING_PROPERTY(DISCONNECTION_REASON_BAN, "ban");
	__DECLARE_STRING_PROPERTY(DISCONNECTION_REASON_IDLE, "idle");
	__DECLARE_STRING_PROPERTY(DISCONNECTION_REASON_KICK, "kick");
	__DECLARE_STRING_PROPERTY(DISCONNECTION_REASON_MANUAL, "manual");
	__DECLARE_STRING_PROPERTY(DISCONNECTION_REASON_UNKNOWN, "unknown");

	__DECLARE_STRING_PROPERTY(KEY_USERNAME, "KEY_USERNAME");
	__DECLARE_STRING_PROPERTY(KEY_PASSWORD, "KEY_PASSWORD");
	__DECLARE_STRING_PROPERTY(KEY_SOUND, "KEY_SOUND");
	__DECLARE_STRING_PROPERTY(KEY_INVITATION, "KEY_INVITATION");
	__DECLARE_STRING_PROPERTY(KEY_AUTO_READY, "KEY_AUTO_READY");
	//__DECLARE_STRING_PROPERTY(KEY_MONEY_TYPE, "KEY_MONEY_TYPE");

	__DECLARE_INT_PROPERTY(ZORDER_POPUP, 500);
	__DECLARE_INT_PROPERTY(ZORDER_POPUP_NOTICE, 600);
	__DECLARE_INT_PROPERTY(MAIN_ZORDER_HEADER, 99);
	__DECLARE_INT_PROPERTY(GAME_MAX_SLOT, 4);
	__DECLARE_INT_PROPERTY(GAME_ZORDER_BUTTON, 150);
	__DECLARE_INT_PROPERTY(GAME_ZORDER_USER, 100);
	__DECLARE_INT_PROPERTY(GAME_ZORDER_HAND_CARD, 60);
	__DECLARE_INT_PROPERTY(GAME_ZORDER_TABLE_CARD, 10);
	__DECLARE_INT_PROPERTY(GAME_ZORDER_SPLASH, 200);
	__DECLARE_INT_PROPERTY(TAG_CARD_HAND, 100);
	__DECLARE_INT_PROPERTY(TAG_CARD_TABLE, 200);
	__DECLARE_INT_PROPERTY(CARD_GROUP_THIENKHAI, 4000);
	__DECLARE_INT_PROPERTY(CARD_GROUP_CHAN, 3000);
	__DECLARE_INT_PROPERTY(CARD_GROUP_3DAU, 2000);
	__DECLARE_INT_PROPERTY(CARD_GROUP_CA, 1000);
	__DECLARE_INT_PROPERTY(CARD_GROUP_QUE, 0);

	__DECLARE_INT_PROPERTY(GAME_ACTION_BASH, 0);
	__DECLARE_INT_PROPERTY(GAME_ACTION_PICK, 1);
	__DECLARE_INT_PROPERTY(GAME_ACTION_FORWARD, 2);
	__DECLARE_INT_PROPERTY(GAME_ACTION_HOLD, 3);
	__DECLARE_INT_PROPERTY(GAME_ACTION_WIN, 4);
	__DECLARE_INT_PROPERTY(GAME_ACTION_DROP_WIN, 5);
	__DECLARE_INT_PROPERTY(GAME_ACTION_PENET, 6);

	__DECLARE_INT_PROPERTY(LOGIN_NORMAL, 0);
	__DECLARE_INT_PROPERTY(LOGIN_FACEBOOK, 1);

	__DECLARE_INT_PROPERTY(EVENT_START_POSX, 1500);

	/*__DECLARE_INT_PROPERTY(SCENE_LOGIN, 0);
	__DECLARE_INT_PROPERTY(SCENE_MAIN, 1);
	__DECLARE_INT_PROPERTY(SCENE_LOBBY, 2);
	__DECLARE_INT_PROPERTY(SCENE_GAME, 3);*/
}