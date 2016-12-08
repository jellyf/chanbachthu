#pragma once
#include "Data.h"
#include "SmartFox.h"

class SFSResponse {
public:
	static void getErrorData(boost::shared_ptr<ISFSObject> isfsObject, ErrorData &errorData) 
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadUTF(errorData.Message);
		byteArray->ReadByte(errorData.Code);
	}

	static void getConfigZone(boost::shared_ptr<ISFSObject> isfsObject, std::vector<std::vector<ZoneData>> &zones)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		while (byteArray->Position() < byteArray->Length()) {
			short size;
			std::vector<ZoneData> vecZone;
			byteArray->ReadShort(size);
			int pos = byteArray->Position() + size;
			while (byteArray->Position() < pos) {
				ZoneData data;
				byteArray->ReadByte(data.Id);
				byteArray->ReadUTF(data.ZoneName);
				byteArray->ReadUTF(data.ZoneIpIos);
				byteArray->ReadUTF(data.ZoneIp);
				byteArray->ReadInt(data.ZonePort);
				byteArray->ReadByte(data.Status);
				byteArray->ReadByte(data.Money);
				vecZone.push_back(data);
				//CCLOG("%d %s %s %s %d %d %d", data.Id, data.ZoneName.c_str(), data.ZoneIpIos.c_str(), data.ZoneIp.c_str(), data.ZonePort, data.Status, data.Money);
			}
			zones.push_back(vecZone);
		}
	}

	static void getUserData(boost::shared_ptr<ISFSObject> isfsObject, UserData &userData) 
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadUTF(userData.Name);
		byteArray->ReadUTF(userData.DisplayName);
		byteArray->ReadDouble(userData.MoneyFree);
		byteArray->ReadDouble(userData.MoneyReal);
		byteArray->ReadInt(userData.UserID);
		byteArray->ReadInt(userData.Exp);
		byteArray->ReadShort(userData.Level);
		byteArray->ReadInt(userData.Total);
		byteArray->ReadInt(userData.Win);
		byteArray->ReadByte(userData.MoneyType);
		//CCLOG("%s %s %d %.0f %.0f %d", userData.Name.c_str(), userData.DisplayName.c_str(), userData.UserID, userData.MoneyFree, userData.MoneyReal, userData.Device);
	}

	static void getRoomData(boost::shared_ptr<ISFSObject> isfsObject, RoomData &roomData) 
	{
		boost::shared_ptr<ByteArray> byteArray1 = isfsObject->GetByteArray("d");
		byteArray1->ReadByte(roomData.TimeStart);
		byteArray1->ReadByte(roomData.TimeDeal);
		byteArray1->ReadByte(roomData.TimeChooseHost);
		//CCLOG("%d %d %d", roomData.TimeStart, roomData.TimeDeal, roomData.TimeChooseHost);
		while (byteArray1->Position() < byteArray1->Length() - 10) {
			short size;
			vector<unsigned char> tmpByte;
			byteArray1->ReadShort(size);
			byteArray1->ReadBytes(size, tmpByte);
			boost::shared_ptr<ByteArray> byteArray = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmpByte))));
		
			PlayerData player;
			player.Info = UserData();
			player.Info.Name = "";
			byteArray->ReadBool(player.Ready);
			byteArray->ReadUTF(player.Info.Name);
			byteArray->ReadUTF(player.Info.DisplayName);
			byteArray->ReadByte(player.Index);
			byteArray->ReadInt(player.Info.SfsUserId);
			byteArray->ReadInt(player.Info.UserID);
			byteArray->ReadDouble(player.PMoney);
			byteArray->ReadShort(player.UType);
			byteArray->ReadUTF(player.Ip);
			byteArray->ReadDouble(player.TMoney);
			roomData.Players.push_back(player);
			//CCLOG("%s %s %d %d %d %f %f %d %s %f", player.Ready ? "true" : "false", player.Info.Name.c_str(), player.Index, player.Info.SfsUserId, player.Info.UserID,
			//	player.Info.MoneyFree, player.Info.MoneyReal, player.UType, player.Ip.c_str(), player.TMoney);
		}
	}

	static void getStartGameData(boost::shared_ptr<ISFSObject> isfsObject, StartGameData &data)
	{
		unsigned char size1, size2, size3, size4, size5;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadByte(data.CardStilt);
		byteArray->ReadInt(data.LastWinner);
		byteArray->ReadInt(data.CurrentTurn);
		if (byteArray->Position() < byteArray->Length()) {
			byteArray->ReadBool(data.CanWin);
			byteArray->ReadByte(size1);
			byteArray->ReadBytes(size1, data.CardHand.ThienKhai);
			byteArray->ReadByte(size2);
			byteArray->ReadBytes(size2, data.CardHand.Chan);
			byteArray->ReadByte(size3);
			byteArray->ReadBytes(size3, data.CardHand.BaDau);
			byteArray->ReadByte(size4);
			byteArray->ReadBytes(size4, data.CardHand.Ca);
			byteArray->ReadByte(size5);
			byteArray->ReadBytes(size5, data.CardHand.Que);
		}
		//CCLOG("%d %d %d %s", data.CardStilt, data.LastWinner, data.CurrentTurn, data.CanWin ? "true" : "false");
	}

	static void getBashData(boost::shared_ptr<ISFSObject> isfsObject, BashData &data)
	{
		unsigned char size1, size2, size3, size4, size5;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(data.UId);
		byteArray->ReadByte(data.CardId);
		byteArray->ReadInt(data.TurnId);
		byteArray->ReadBool(data.CanPenet);
		byteArray->ReadBool(data.CanPenetWin);
		byteArray->ReadByte(data.SoundId);
		if (byteArray->Position() < byteArray->Length() - 5) {
			byteArray->ReadByte(size1);
			byteArray->ReadBytes(size1, data.CardHand.ThienKhai);
			byteArray->ReadByte(size2);
			byteArray->ReadBytes(size2, data.CardHand.Chan);
			byteArray->ReadByte(size3);
			byteArray->ReadBytes(size3, data.CardHand.BaDau);
			byteArray->ReadByte(size4);
			byteArray->ReadBytes(size4, data.CardHand.Ca);
			byteArray->ReadByte(size5);
			byteArray->ReadBytes(size5, data.CardHand.Que);
		}
		//CCLOG("%d %d %d %s %s %d", data.UId, data.CardId, data.TurnId, data.CanPenet ? "true" : "false", data.CanPenetWin ? "true" : "false", data.SoundId);
	}

	static void getBashBackData(boost::shared_ptr<ISFSObject> isfsObject, BashBackData &data)
	{
		unsigned char size1, size2, size3, size4, size5;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		//byteArray->ReadByte(data.Status);
		byteArray->ReadInt(data.UId);
		byteArray->ReadByte(data.CardId);
		byteArray->ReadInt(data.BackId);
		byteArray->ReadBool(data.IsPicked);
		byteArray->ReadInt(data.TurnId);
		byteArray->ReadBool(data.CanPenet);
		byteArray->ReadBool(data.CanPenetWin);
		byteArray->ReadByte(data.SoundId);
		if (byteArray->Position() < byteArray->Length() - 5) {
			byteArray->ReadByte(size1);
			byteArray->ReadBytes(size1, data.CardHand.ThienKhai);
			byteArray->ReadByte(size2);
			byteArray->ReadBytes(size2, data.CardHand.Chan);
			byteArray->ReadByte(size3);
			byteArray->ReadBytes(size3, data.CardHand.BaDau);
			byteArray->ReadByte(size4);
			byteArray->ReadBytes(size4, data.CardHand.Ca);
			byteArray->ReadByte(size5);
			byteArray->ReadBytes(size5, data.CardHand.Que);
		}
		//CCLOG("%d %d %d %d %s %s %d", data.UId, data.CardId, data.BackId, data.TurnId, data.CanPenet ? "true" : "false", data.CanPenetWin ? "true" : "false", data.SoundId);
	}

	static void getHoldData(boost::shared_ptr<ISFSObject> isfsObject, HoldData &data)
	{
		unsigned char size1, size2, size3, size4, size5;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(data.UId);
		byteArray->ReadByte(data.CardId);
		byteArray->ReadByte(data.CardIdHold);
		byteArray->ReadInt(data.TurnId);
		byteArray->ReadBool(data.IsPicked);
		byteArray->ReadByte(data.SoundId);
		if (byteArray->Position() < byteArray->Length() - 5) {
			byteArray->ReadByte(size1);
			byteArray->ReadBytes(size1, data.CardHand.ThienKhai);
			byteArray->ReadByte(size2);
			byteArray->ReadBytes(size2, data.CardHand.Chan);
			byteArray->ReadByte(size3);
			byteArray->ReadBytes(size3, data.CardHand.BaDau);
			byteArray->ReadByte(size4);
			byteArray->ReadBytes(size4, data.CardHand.Ca);
			byteArray->ReadByte(size5);
			byteArray->ReadBytes(size5, data.CardHand.Que);
		}
		//CCLOG("%d %d %d %d %s %d", data.UId, data.CardId, data.CardIdHold, data.TurnId, data.IsPicked ? "true" : "false", data.SoundId);
	}

	static void getPickData(boost::shared_ptr<ISFSObject> isfsObject, PickData &data)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(data.UId);
		byteArray->ReadByte(data.CardId);
		byteArray->ReadInt(data.TurnId);
		byteArray->ReadBool(data.CanPenet);
		byteArray->ReadBool(data.CanWin);
		byteArray->ReadByte(data.StiltCount);
		byteArray->ReadByte(data.SoundId);
		//CCLOG("%d %d %d %d %s %s %d", data.UId, data.CardId, data.TurnId, data.StiltCount, data.CanPenet ? "true" : "false", data.CanWin ? "true" : "false", data.SoundId);
	}

	static void getForwardData(boost::shared_ptr<ISFSObject> isfsObject, ForwardData &data)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(data.UId);
		byteArray->ReadInt(data.TurnId);
		byteArray->ReadByte(data.SoundId);
		//CCLOG("%d %d %d", data.UId, data.TurnId, data.SoundId);
	}

	static void getPenetData(boost::shared_ptr<ISFSObject> isfsObject, PenetData &data)
	{
		unsigned char size1, size2, size3, size4, size5;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(data.UId);
		byteArray->ReadByte(data.CardId);
		byteArray->ReadBool(data.IsPicked);
		byteArray->ReadByte(data.SoundId);
		if (byteArray->Position() < byteArray->Length() - 5) {
			byteArray->ReadByte(size1);
			byteArray->ReadBytes(size1, data.CardHand.ThienKhai);
			byteArray->ReadByte(size2);
			byteArray->ReadBytes(size2, data.CardHand.Chan);
			byteArray->ReadByte(size3);
			byteArray->ReadBytes(size3, data.CardHand.BaDau);
			byteArray->ReadByte(size4);
			byteArray->ReadBytes(size4, data.CardHand.Ca);
			byteArray->ReadByte(size5);
			byteArray->ReadBytes(size5, data.CardHand.Que);
		}
		//CCLOG("%d %d %s %d", data.UId, data.CardId, data.IsPicked ? "true" : "false", data.SoundId);
	}

	static void getCrestResponseData(boost::shared_ptr<ISFSObject> isfsObject, CrestResponseData &data)
	{
		unsigned char size1, size2, size3;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadByte(data.Status);
		byteArray->ReadInt(data.UId); 
		byteArray->ReadShort(data.Ga);
		byteArray->ReadShort(data.Diem);
		byteArray->ReadInt(data.DenLangId);
		byteArray->ReadByte(size1);
		byteArray->ReadBytes(size1, data.CuocHo);
		/*string strcuoc;
		for (int i = 0; i < size1; i++) {
			strcuoc += to_string(data.CuocHo[i]) + " ";
		}
		//CCLOG("CUOC_HO: %s", strcuoc.c_str());*/
		byteArray->ReadByte(size2);
		byteArray->ReadBytes(size2, data.CuocSai);
		/*string strcuocs;
		for (int i = 0; i < size2; i++) {
			strcuocs += to_string(data.CuocSai[i]) + " ";
		}
		//CCLOG("CUOC_SAI: %s", strcuocs.c_str());*/
		byteArray->ReadUTF(data.Msg);
		byteArray->ReadUTF(data.MsgAnGa);
		if (byteArray->Position() < byteArray->Length()) {
			byteArray->ReadByte(size3);
			for (int i = 0; i < size3; i++) {
				long id;
				byteArray->ReadInt(id);
				data.ListUDe.push_back(id);
			}
		}
		//CCLOG("%d %d %d %d %d %s %s", data.Status, data.UId, data.Ga, data.Diem, data.DenLangId, data.Msg.c_str(), data.MsgAnGa.c_str());
	}

	static void getEndMatchTieData(boost::shared_ptr<ISFSObject> isfsObject, vector<unsigned char> &stiltCards)
	{
		unsigned char size1;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadByte(size1);
		byteArray->ReadBytes(size1, stiltCards);
	}

	static void getEndMatchData(boost::shared_ptr<ISFSObject> isfsObject, EndMatchData &data)
	{
		unsigned char size1, size2, size3, size4, size5, size6, size7, size8, size9, size10, size11;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(data.WinId);
		byteArray->ReadByte(size1);
		byteArray->ReadBytes(size1, data.ListChanU);
		byteArray->ReadByte(size2);
		byteArray->ReadBytes(size2, data.ListCaU);
		byteArray->ReadByte(size3);
		byteArray->ReadBytes(size3, data.ListCardU);
		byteArray->ReadByte(size4);
		byteArray->ReadBytes(size4, data.ListStiltCard);
		byteArray->ReadByte(data.SoundId);
		byteArray->ReadByte(size5);
		byteArray->ReadBytes(size5, data.ListHandCardU);
		byteArray->ReadByte(size6);
		for (int i = 0; i < size6; i++) {
			PlayerCardHandData player;
			byteArray->ReadInt(player.UId);
			byteArray->ReadByte(size7);
			byteArray->ReadBytes(size7, player.CardHand.ThienKhai);
			byteArray->ReadByte(size8);
			byteArray->ReadBytes(size8, player.CardHand.Chan);
			byteArray->ReadByte(size9);
			byteArray->ReadBytes(size9, player.CardHand.BaDau);
			byteArray->ReadByte(size10);
			byteArray->ReadBytes(size10, player.CardHand.Ca);
			byteArray->ReadByte(size11);
			byteArray->ReadBytes(size11, player.CardHand.Que);
			data.ListPlayerCard.push_back(player);
		}
	}

	static void getEndMatchMoneyData(boost::shared_ptr<ISFSObject> isfsObject, EndMatchMoneyData &data)
	{
		unsigned char size;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadByte(size);
		for (int i = 0; i < size; i++) {
			long uid;
			double amount;
			byteArray->ReadInt(uid);
			byteArray->ReadDouble(amount);
			data.ListUserId.push_back(uid);
			data.ListUserAmount.push_back(amount);
			//CCLOG("%d %.0f", uid, amount);
		}
	}

	static void getLobbyRoomTypeData(boost::shared_ptr<ISFSObject> isfsObject, LobbyListRoomType &data)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadBool(data.IsRealMoney);
		//CCLOG("RealMoney: %s", data.IsRealMoney ? "true" : "false");
		while (byteArray->Position() < byteArray->Length()) {
			LobbyRoomType room;
			byteArray->ReadByte(room.Id);
			byteArray->ReadUTF(room.Group);
			byteArray->ReadUTF(room.Name);
			byteArray->ReadUTF(room.Description);
			data.ListRoomType.push_back(room);
			//CCLOG("%d %s %s %s", room.Id, room.Group.c_str(), room.Name.c_str(), room.Description.c_str());
		}
	}

	static void getLobbyTableData(boost::shared_ptr<ISFSObject> isfsObject, LobbyListTable &listTable)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadByte(listTable.Size);
		byteArray->ReadInt(listTable.Money);
		while(byteArray->Position() < byteArray->Length()) {
			LobbyTableData data;
			vector<unsigned char> tmp1, tmp2;
			short size1, size2, size3;
			byteArray->ReadShort(size1);
			byteArray->ReadBytes(size1, tmp1);

			boost::shared_ptr<ByteArray> byteArray1 = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmp1))));
			byteArray1->ReadInt(data.Money);
			byteArray1->ReadBool(data.IsAdvance);
			byteArray1->ReadBool(data.IsU411);
			byteArray1->ReadByte(data.HasPot);
			byteArray1->ReadByte(data.Time);
			byteArray1->ReadByte(data.RoomId);

			while (byteArray1->Position() < byteArray1->Length()) {
				byteArray1->ReadShort(size2);
				byteArray1->ReadBytes(size2, tmp1);

				boost::shared_ptr<ByteArray> byteArray2 = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmp1))));
				LobbyPlayerData pdata;
				byteArray2->ReadInt(pdata.UserId);
				byteArray2->ReadDouble(pdata.Money);
				byteArray2->ReadUTF(pdata.Name);

				data.ListPlayer.push_back(pdata);
			}
			listTable.ListTable.push_back(data);
		}
	}

	static void getGamePunishData(boost::shared_ptr<ISFSObject> isfsObject, long &UiD, std::string &msg)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadUTF(msg);
		byteArray->ReadInt(UiD);
		//CCLOG("%d %s", UiD, msg.c_str());
	}

	static void getGameReadyData(boost::shared_ptr<ISFSObject> isfsObject, long &UiD)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(UiD);
		//CCLOG("%d", UiD);
	}

	static void getGameTableData(boost::shared_ptr<ISFSObject> isfsObject, GameTableData &data)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(data.Money);
		byteArray->ReadByte(data.Time);
		byteArray->ReadBool(data.IsAdvance);
		byteArray->ReadBool(data.IsU411);
		byteArray->ReadBool(data.IsQuick);
		////CCLOG("%d %d %s %s %s", data.Money, data.Time, data.IsAdvance ? "true" : "false", data.IsU411 ? "true" : "false", data.IsQuick ? "true" : "false");
	}

	static void getLobbyUserData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<UserData> &listUser)
	{
		boost::shared_ptr<ByteArray> byteArray1 = isfsObject->GetByteArray("d");
		while (byteArray1->Position() < byteArray1->Length()) {
			short size;
			vector<unsigned char> tmpByte;
			byteArray1->ReadShort(size);
			byteArray1->ReadBytes(size, tmpByte);
			boost::shared_ptr<ByteArray> byteArray = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmpByte))));
			
			UserData user;
			byteArray->ReadUTF(user.Name);
			byteArray->ReadDouble(user.MoneyFree);
			byteArray->ReadDouble(user.MoneyReal);
			byteArray->ReadInt(user.UserID);
			byteArray->ReadInt(user.Exp);
			byteArray->ReadShort(user.Level);
			byteArray->ReadInt(user.Total);
			byteArray->ReadInt(user.Win);
			byteArray->ReadInt(user.SfsUserId);

			listUser.push_back(user);
			//CCLOG("%s %.0f %.0f %d %d %d %d %d", user.Name.c_str(), user.MoneyFree, user.MoneyReal, user.UserID, user.Exp, user.Level, user.Total, user.Win);
		}
	}

	static void getLobbyInviteData(boost::shared_ptr<ISFSObject> isfsObject, InviteData &data)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadUTF(data.InviterName);
		byteArray->ReadInt(data.InviterId);
		byteArray->ReadDouble(data.Money);
		byteArray->ReadByte(data.RoomTime);
		byteArray->ReadUTF(data.Msg);
		byteArray->ReadUTF(data.RoomName);
		byteArray->ReadUTF(data.Password);
		////CCLOG("%s %d %.0f %d %s %s %s", data.InviterName.c_str(), data.InviterId, data.Money, data.RoomTime, data.Msg.c_str(), data.RoomName.c_str(), data.Password.c_str());
	}

	static void getGameRoomDataGa(boost::shared_ptr<ISFSObject> isfsObject, bool &isGa, double &gaMoney)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadBool(isGa);
		if(isGa) byteArray->ReadDouble(gaMoney);
		else gaMoney = 0;
		//CCLOG("%s %.0f", isGa ? "true" : "false", gaMoney);
	}

	static void getGamePlayingTableData(boost::shared_ptr<ISFSObject> isfsObject, PlayingTableData &data)
	{
		boost::shared_ptr<ByteArray> byteArray1 = isfsObject->GetByteArray("d");
		byteArray1->ReadBool(data.IsReconnect);
		byteArray1->ReadInt(data.TurnId);
		byteArray1->ReadByte(data.StiltCount);
		while (byteArray1->Position() < byteArray1->Length()) {
			short size;
			std::string str1, str2;
			std::vector<std::vector<char>> vnumb;
			std::vector<unsigned char> tmpByte;
			byteArray1->ReadShort(size);
			byteArray1->ReadBytes(size, tmpByte);
			boost::shared_ptr<ByteArray> byteArray = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmpByte))));

			PlayerData player;
			player.Info = UserData();
			byteArray->ReadUTF(player.Info.Name);
			byteArray->ReadUTF(player.Info.DisplayName);
			byteArray->ReadByte(player.Index);
			byteArray->ReadInt(player.Info.SfsUserId);
			byteArray->ReadInt(player.Info.UserID);
			byteArray->ReadDouble(player.PMoney);
			byteArray->ReadShort(player.UType);
			byteArray->ReadUTF(player.Ip);
			byteArray->ReadDouble(player.TMoney);
			byteArray->ReadUTF(player.Info.GroupAvatar);
			byteArray->ReadByte(player.Info.Device);
			byteArray->ReadUTF(str1);
			byteArray->ReadUTF(str2);
			//CCLOG("%s ___ %s", str1.c_str(), str2.c_str());
			if (str1.length() > 2) {
				SFSResponse::getPlayingCardFromJson(str1, player.PairCards);
			}
			if (str2.length() > 2) {
				SFSResponse::getPlayingCardFromJson(str2, vnumb);
			}
			if (vnumb.size() > 0) {
				player.SingleCards = vnumb[0];
			}

			data.Players.push_back(player);
			//CCLOG("%s %d %d %d %d %s %.0f %s %d ___ %s ___ %s", player.Info.Name.c_str(), player.Index, player.Info.SfsUserId, player.Info.UserID,
			//	player.UType, player.Ip.c_str(), player.PMoney, player.Info.GroupAvatar.c_str(), player.Info.Device, str1.c_str(), str2.c_str());
		}
	}

	static void getPlayingCardFromJson(std::string json, std::vector<std::vector<char>> &vec) {
		char numb = 0;
		vector<char> vnumb;
		for (int i = 0; i < json.length(); i++) {
			if (json[i] >= '0' && json[i] <= '9') {
				if (numb == 0) {
					numb = json[i] - 48;
					if (i > 0 && json[i - 1] == '-') {
						numb = -numb;
					}
				} else {
					if (numb > 0) {
						numb = (numb * 10) + json[i] - 48;
					} else {
						numb = (numb * 10) - json[i] + 48;
					}
				}
			} else if (numb != 0) {
				vnumb.push_back(numb);
				if (json[i] == ']') {
					vector<char> newVec = vector<char>(vnumb);
					vec.push_back(newVec);
					vnumb.clear();
				}
				numb = 0;
			}
		}
	}

	static void getGameSpectatorData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<PlayerData> &list)
	{
		boost::shared_ptr<ByteArray> byteArray1 = isfsObject->GetByteArray("d");
		while (byteArray1->Position() < byteArray1->Length()) {
			short size;
			std::vector<unsigned char> tmpByte;
			byteArray1->ReadShort(size);
			byteArray1->ReadBytes(size, tmpByte);
			boost::shared_ptr<ByteArray> byteArray = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmpByte))));

			PlayerData player;
			player.Info = UserData();
			byteArray->ReadBool(player.Ready);
			byteArray->ReadUTF(player.Info.Name);
			byteArray->ReadUTF(player.Info.DisplayName);
			byteArray->ReadByte(player.Index);
			byteArray->ReadInt(player.Info.SfsUserId);
			byteArray->ReadInt(player.Info.UserID);
			byteArray->ReadDouble(player.PMoney);
			byteArray->ReadShort(player.UType);
			byteArray->ReadUTF(player.Ip);
			byteArray->ReadShort(player.Info.Level);
			byteArray->ReadDouble(player.TMoney);
			byteArray->ReadUTF(player.Info.GroupAvatar);
			byteArray->ReadByte(player.Info.Device);

			list.push_back(player);
			//CCLOG("%s %d %d %d %.0f %d %s %s %d", player.Info.Name.c_str(), player.Index, player.Info.SfsUserId, player.Info.UserID,
			//	player.PMoney, player.UType, player.Ip.c_str(), player.Info.GroupAvatar.c_str(), player.Info.Device);
		}
	}

	static void getRankData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<std::vector<RankData>> &list)
	{
		short size1, size2;
		std::vector<RankData> list1, list2;
		std::vector<unsigned char> tmp1, tmp2;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadShort(size1);
		byteArray->ReadBytes(size1, tmp1);
		byteArray->ReadShort(size2);
		byteArray->ReadBytes(size2, tmp2);

		boost::shared_ptr<ByteArray> byteArray1 = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmp1))));
		while (byteArray1->Position() < byteArray1->Length()) {
			unsigned char size3;
			std::vector<unsigned char> tmp3;
			byteArray1->ReadByte(size3);
			byteArray1->ReadBytes(size3, tmp3);
			boost::shared_ptr<ByteArray> byteArray3 = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmp3))));

			RankData data;
			byteArray3->ReadUTF(data.Name);
			byteArray3->ReadDouble(data.Money);
			byteArray3->ReadInt(data.Exp);
			byteArray3->ReadInt(data.Uid);
			list1.push_back(data);
		}

		boost::shared_ptr<ByteArray> byteArray2 = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmp2))));
		while (byteArray2->Position() < byteArray2->Length()) {
			unsigned char size4;
			std::vector<unsigned char> tmp4;
			byteArray2->ReadByte(size4);
			byteArray2->ReadBytes(size4, tmp4);
			boost::shared_ptr<ByteArray> byteArray4 = boost::shared_ptr<ByteArray>(new ByteArray(boost::shared_ptr<vector<unsigned char>>(new vector<unsigned char>(tmp4))));

			RankData data;
			byteArray4->ReadUTF(data.Name);
			byteArray4->ReadDouble(data.Money);
			byteArray4->ReadInt(data.Exp);
			byteArray4->ReadInt(data.Uid);
			list2.push_back(data);
		}

		list.push_back(list1);
		list.push_back(list2);
	}

	static void getPlayLogData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<PlayLogData> &list)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		long tmp1;
		unsigned char tmp2;
		byteArray->ReadInt(tmp1);
		byteArray->ReadByte(tmp2);
		while (byteArray->Position() < byteArray->Length()) {
			PlayLogData data;
			byteArray->ReadUTF(data.Date);
			byteArray->ReadUTF(data.Info);
			byteArray->ReadInt(data.Money);
			byteArray->ReadDouble(data.Balance);
			byteArray->ReadInt(data.GameId);
			list.push_back(data);
			//CCLOG("%s %s %d %.0f %d", data.Date.c_str(), data.Info.c_str(), data.Money, data.Balance, data.GameId);
		}
	}

	static void getGameMyReconnectData(boost::shared_ptr<ISFSObject> isfsObject, GameReconnectData &data)
	{
		unsigned char size1, size2, size3, size4, size5;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadByte(data.PlayerState);
		byteArray->ReadByte(size1);
		byteArray->ReadBytes(size1, data.CardHand.ThienKhai);
		byteArray->ReadByte(size2);
		byteArray->ReadBytes(size2, data.CardHand.Chan);
		byteArray->ReadByte(size3);
		byteArray->ReadBytes(size3, data.CardHand.BaDau);
		byteArray->ReadByte(size4);
		byteArray->ReadBytes(size4, data.CardHand.Ca);
		byteArray->ReadByte(size5);
		byteArray->ReadBytes(size5, data.CardHand.Que);
		byteArray->ReadShort(data.IndexCard);
	}

	static void getGameUserReconnectData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<UserReconnectData> &list)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		while (byteArray->Position() < byteArray->Length()) {
			UserReconnectData data;
			byteArray->ReadInt(data.SfsUserId);
			byteArray->ReadUTF(data.Name);
			byteArray->ReadUTF(data.DisplayName);
			byteArray->ReadInt(data.UserId);
			list.push_back(data);
		}
	}

	static void getShopHistoryData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<ShopHistoryData> &list)
	{
		long numb;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(numb);
		while (byteArray->Position() < byteArray->Length()) {
			ShopHistoryData data;
			byteArray->ReadByte(data.Id);
			byteArray->ReadByte(data.ItemId);
			byteArray->ReadUTF(data.Name);
			byteArray->ReadByte(data.Price);
			byteArray->ReadByte(data.Status);
			byteArray->ReadUTF(data.CreateDate);
			byteArray->ReadUTF(data.UpdateDate);
			byteArray->ReadUTF(data.Content);
		}
	}

	static void getShopItemsData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<ShopItemData> &list)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		while (byteArray->Position() < byteArray->Length()) {
			ShopItemData data;
			byteArray->ReadByte(data.Id);
			byteArray->ReadUTF(data.Name);
			byteArray->ReadInt(data.Price);
			byteArray->ReadInt(data.PriceChange);
			byteArray->ReadShort(data.ItemType);
			byteArray->ReadUTF(data.Image);
			byteArray->ReadShort(data.IsActived);
			list.push_back(data);
			////CCLOG("%d %s %d %d %d %s %d", data.Id, data.Name.c_str(), data.Price, data.PriceChange, data.ItemType, data.Image.c_str(), data.IsActived);
		}
	}

	static void getListMailData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<MailData> &list)
	{
		long tmp;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(tmp);
		while (byteArray->Position() < byteArray->Length()) {
			MailData data;
			byteArray->ReadInt(data.Id);
			byteArray->ReadUTF(data.Sender);
			byteArray->ReadUTF(data.Title);
			byteArray->ReadBool(data.IsRead);
			byteArray->ReadUTF(data.Date);
			list.push_back(data);
			////CCLOG("%d %s %s %s %s", data.Id, data.Date.c_str(), data.Title.c_str(), data.Sender.c_str(), data.IsRead ? "true" : "false");
		}
	}

	static void getNewsData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<NewsData> &list)
	{
		long tmp;
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadInt(tmp);
		while (byteArray->Position() < byteArray->Length()) {
			NewsData data;
			byteArray->ReadByte(data.Id);
			byteArray->ReadUTF(data.Title);
			byteArray->ReadUTF(data.Content);
			byteArray->ReadUTF(data.Image);
			byteArray->ReadByte(data.Type);
			byteArray->ReadUTF(data.Date);
			list.push_back(data);
		}
	}

	static void getListEventData(boost::shared_ptr<ISFSObject> isfsObject, std::vector<EventData> &list)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		while (byteArray->Position() < byteArray->Length()) {
			EventData data;
			byteArray->ReadByte(data.Id);
			byteArray->ReadUTF(data.Content);
			byteArray->ReadUTF(data.Date);
			list.push_back(data);
		}
	}

	static void getTableReconnectData(boost::shared_ptr<ISFSObject> isfsObject, TableReconnectData &data)
	{
		boost::shared_ptr<ByteArray> byteArray = isfsObject->GetByteArray("d");
		byteArray->ReadUTF(data.Room);
		byteArray->ReadUTF(data.Message);
	}
};