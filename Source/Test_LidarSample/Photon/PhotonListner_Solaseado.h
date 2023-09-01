#pragma once

#ifdef __clang__
#pragma clang diagnostic ignored "-Woverloaded-virtual"
#endif
#if defined(_EG_WINDOWS_PLATFORM)
#include "Windows/AllowWindowsPlatformTypes.h"
#endif
#pragma warning (disable: 4263)
#pragma warning (disable: 4264)
#include "LoadBalancing-cpp/inc/Client.h"
#pragma warning (default: 4263)
#pragma warning (default: 4264)
#if defined(_EG_WINDOWS_PLATFORM)
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#include "SH_PhotonBasic.h"
#include "CoreMinimal.h"

enum class EMoveEvent : uint8;
using namespace ExitGames;
using namespace Common;

class PhotonListner_Solaseado : public ExitGames::LoadBalancing::Listener
{
public:
	PhotonListner_Solaseado(SH_PhotonBasic* pView);
	virtual ~PhotonListner_Solaseado(void);
	virtual void service(void);

	void SetClient(ExitGames::LoadBalancing::Client* client);

	// receive and print out debug out here
	virtual void debugReturn(int debugLevel, const Common::JString& string) override;

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode) override;
	virtual void clientErrorReturn(int errorCode) override;
	virtual void warningReturn(int warningCode) override;
	virtual void serverErrorReturn(int errorCode) override;

	// Room 
	// events, triggered by certain operations of all players in the same room
	virtual void joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const LoadBalancing::Player& player);
	virtual void leaveRoomEventAction(int playerNr, bool isInactive) override;

	// Ŀ���� �̺�Ʈ 
	virtual void customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContentObj) override;

	// callbacks for operations on the server
	virtual void connectReturn(int errorCode, const Common::JString& errorString, const Common::JString& region, const Common::JString& cluster) override;
	virtual void disconnectReturn(void) override;

	// ����� �̺�Ʈ 
	virtual void joinOrCreateRoomReturn(int localPlayerNr, const Common::Hashtable& roomProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString) override;
	virtual void createRoomReturn(int localPlayerNr, const Common::Hashtable& roomProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString) override;

	// �� ���� or ������
	virtual void leaveRoomReturn(int errorCode, const Common::JString& errorString) override;

	// ���� ���� ���� ���� 
	virtual void onAvailableRegions(const JVector<JString>& availableRegions, const JVector<JString>& availableRegionServers) override;

	// ĳ���� ������Ƽ ������Ʈ 
	virtual void onPlayerPropertiesChange(int playerNr, const Common::Hashtable& changes) override;

	// �� ���� ������Ƽ ������Ʈ 
	virtual void onRoomPropertiesChange(const Common::Hashtable& changes) override;

	virtual void onRoomListUpdate(void) override;
protected:

	// ���� ������Ʈ �ð� üũ
	unsigned long lastUpdateTime;

	SH_PhotonBasic* m_pView;
	ExitGames::LoadBalancing::Client* m_pClient;
	// ���� �� 
	ExitGames::Common::JString mMap;
	ExitGames::Common::JString mChannel = ExitGames::Common::JString("-1");

	// �÷��̾� �ѹ�
	int LocalPlayerNr;

	// ���� �׷� ����
	int m_UseGroups;

public:

	void Connect(const JString& userName, const JString& serverAddress);

	// // Custom Event 
	// ��ġ ���� 
	void SetMoveAndRotation(int vX, int vY, int vZ, int yaw);		// 7

	void SetPlayerAnim(int Anim);									// 15

	// �ؽ�Ʈ ����
	void PlayerTargetMessage(const int* target, int size, nByte text);	// 2
	void TextMessage(const JString& message, const JString& type);		// 1

	// �ִϸ��̼�D
	void setPlayerAnimationData(uint8 anim); // addCustomProperties

	// �� �������� �̺�Ʈ üũ
	void setRoomEventProperties(uint8 Ev);
	// �������� �̺�Ʈ �Ͻ�����
	void setEventPause(bool ev);

	void SetFriendRequest(const int& Target, const FString& PlayFabID);

protected:

	//���� ���� �ִ� �ο���
	int MaxPlayerRoom = 15;

	//������ �Ѹ� �����͸� ������ �� ��� ���̺�
	//���⿡ ����� ������ �����Ͱ� �ѷ����� ���� ������ updatePlayerProperties()���� ó���� �� �ְ� �Ѵ�.
	ExitGames::Common::Hashtable mCharacterInfo;

public:
	//�÷��̾� Ŀ���� ������ ���� ó��
	void SetChracterInfo(FString _key, FString _value);
	void SendCharacterInfo();
	void RemoveCharacterInfo();

	void InitJoinOrCreateRoom();
	void SendPlayerAnimState(uint8 _State);

//////////////////////������, ����
protected:
	FString sRoomName = "MainRoom";
	FString sRoomCount;
	bool b_IsFirstConnect = true;
	int RoomMaxSize = 20;
	int PlayerMaxCount = 20;

public:
	void ChangeRoomNumber(int Number) { sRoomCount = FString::FromInt(Number); }
	int getRoomCount() { return FCString::Atoi(*sRoomCount); }

///////////////////////���̿��� �Ǵ�
protected:
	bool b_IsDummy = false;
public:
	void setDummy(bool IsDummy);

///////////////////////�̵�
public:
	void Move(FVector Loc);
	void MoveStop();
	void MoveStopFinish(FVector Loc);

/////NPC
	// NPC ������Ʈ
	void UpdateNPC(TArray<FString> aID, TArray<FVector> aLoc);
	void SetNPCMove(FString sName,FVector vLoc);
////NPC ��
};


