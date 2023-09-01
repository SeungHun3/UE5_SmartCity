// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define DBGPRINTF_LEVEL 6

#ifdef PlaySound
#undef PlaySound
#endif

#include "../Actor_PhotonAudioIn.h"
#include "../Actor_PhotonAudioOut.h" // ��������

#include <vector>
#include "LoadBalancing-cpp/inc/Client.h"
#include "PhotonVoice-cpp/inc/Transport/LoadBalancingListener.h"
#include "PhotonVoice-cpp/inc/VoiceAudio.h"
#include "PhotonVoice-cpp/inc/RemoteVoiceOptions.h"
#include "PhotonVoice-cpp/inc/AudioOutDelayControl.h"
#include "PhotonVoice-cpp/inc/AudioUtil.h"
#include "SH_PhotonVoiceBasic.h"

class TEST_LIDARSAMPLE_API SH_PhotonVoiceListener : public ExitGames::Voice::LoadBalancingListener
{
public:
	SH_PhotonVoiceListener(ExitGames::Common::JString const& appID, ExitGames::Common::JString const& appVersion, IAudioInFactory* audioInFactory, IAudioOutFactory* audioOutFactory, SH_PhotonVoiceBasic* PhotonVoiceBasic);
	~SH_PhotonVoiceListener(void);
	void update(void);
	ExitGames::Common::JString getStateString(void);

	void connect(const ExitGames::Common::JString& name);
	void disconnect(void);
	void toggleEcho(void);
private:
	// receive and print out debug out here
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	// events, triggered by certain operations of all players in the same room
	virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	virtual void leaveRoomEventAction(int playerNr, bool isInactive);
	virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

	// callbacks for operations on PhotonLoadBalancing server
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster);
	virtual void disconnectReturn(void);
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinLobbyReturn(void);
	virtual void leaveLobbyReturn(void);
	virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& /*availableRegions*/, const ExitGames::Common::JVector<ExitGames::Common::JString>& /*availableRegionServers*/);

	void sendData(void);
	static void remoteVoiceRemoveCallback(void* opaque);
	void remoteVoiceRemoveCallback(AActor_PhotonAudioOut* audioOut);
	static void remoteVoiceInfoCallback(void* opaque, int channelId, int playerId, nByte voiceId, ExitGames::Voice::VoiceInfo const& voiceInfo, ExitGames::Voice::RemoteVoiceOptions& options);
	void remoteVoiceInfoCallback(int channelId, int playerId, nByte voiceId, ExitGames::Voice::VoiceInfo const& voiceInfo, ExitGames::Voice::RemoteVoiceOptions& options);

	class State
	{
	public:
		enum States
		{
			INITIALIZED = 0,
			CONNECTING,
			CONNECTED,
			JOINING,
			JOINED,
			SENT_DATA,
			RECEIVED_DATA,
			LEAVING,
			LEFT,
			DISCONNECTING,
			DISCONNECTED
		};
	};
	State::States mState;

	
	ExitGames::Common::Logger mLogger;

	int64 mSendCount;
	int64 mReceiveCount;

	bool mVoicesCreated;

	//������ ������ ��Ʈ���� ��Ÿ���ϴ�.(���̵� ����)
	std::vector<ExitGames::Voice::LocalVoice*> mLocalVoices;

	//����� Ǫ�� �������̽�
	//IAudioPusher�� �غ� �� ������ ����� �����͸� Ǫ���մϴ�.
	std::vector<ExitGames::Voice::IAudioPusher<short>*> mAudioSources;

	//����� ��Ŷ ��ũ
	std::vector<ExitGames::Voice::IAudioOut<short>*> mAudioPlayers;

	IAudioInFactory* mpAudioInFactory;
	IAudioOutFactory* mpAudioOutFactory;
	SH_PhotonVoiceBasic* mBasic;

public:
	ExitGames::LoadBalancing::Client mLoadBalancingClient;
	void JoinRoom();
	void SetRoomName(const FString& FullName) { v_RoomName = TCHAR_TO_UTF8(*FullName); }
	//id�� �÷��̾� �ѹ��� �����Ѵ�.
	ExitGames::Common::Hashtable mCharacterInfo;
	bool isconnected = false;
	bool IsChanging = false;

private:
	ExitGames::Common::JString v_RoomName;

	
};