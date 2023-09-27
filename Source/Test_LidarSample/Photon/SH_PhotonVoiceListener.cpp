// Fill out your copyright notice in the Description page of Project Settings.


#include "SH_PhotonVoiceListener.h"

#include <algorithm>
#include <vector>
#include "limits.h"
#include "LoadBalancing-cpp/inc/Enums/DirectMode.h"
#include "PhotonVoice-cpp/inc/Transport/LoadBalancingListener.h"
#include "PhotonVoice-cpp/inc/Transport/LoadBalancingTransport.h"
#include "PhotonVoice-cpp/inc/RemoteVoiceOptions.h"
#include "PhotonVoice-cpp/inc/AudioGen.h"
#include "PhotonVoice-cpp/inc/OpusCodec.h"
#include "../Actor_PhotonAudioIn.h"
#include "Console.h"


using namespace ExitGames;
//using namespace ExitGames::Common; // outcommented to avoid symbol clashes between UE_LOG and Common::JString
using namespace ExitGames::Common::Helpers;
using namespace ExitGames::Common::MemoryManagement;
using namespace ExitGames::LoadBalancing;
using namespace ExitGames::Photon;
using namespace ExitGames::Voice;
using namespace ExitGames::Voice::AudioUtil;

static const bool DIRECT = false; // send audio peer to peer (debug echo mode is not supported)
static const bool DEBUG_ECHO_MODE_INIT = false; // initial state of local voice debug echo mode (streaming back to client via server)


static const int MAX_SENDCOUNT = 100;


static void frameDataCallback(void* opaque, const Buffer<short>& frame)
{
	IAudioOut<short>* audioOut = static_cast<IAudioOut<short>*>(opaque);
	audioOut->push(frame);
}


struct RemoveInfo
{
	RemoveInfo(SH_PhotonVoiceListener* photonLib, AActor_PhotonAudioOut* out)
		: photonLib(photonLib), out(out)
	{}
	SH_PhotonVoiceListener* photonLib;
	AActor_PhotonAudioOut* out;
};

//�ݹ�
void SH_PhotonVoiceListener::remoteVoiceRemoveCallback(void* opaque)
{
	static_cast<void>(opaque);
	RemoveInfo* ri = static_cast<RemoveInfo*>(opaque);
	ri->photonLib->remoteVoiceRemoveCallback(ri->out);
	delete ri;
}

//���� ��º� ���� �ݹ�
void SH_PhotonVoiceListener::remoteVoiceRemoveCallback(AActor_PhotonAudioOut* out)
{
	out->getPlayer()->stop();
	//mAudioPlayers.erase(std::remove(mAudioPlayers.begin(), mAudioPlayers.end(), out->getPlayer()), mAudioPlayers.end());
	out->finit();
	mAudioPlayers.Remove(out);
	mpAudioOutFactory->destroyAudioOut(out);
	
}

//�ݹ�
void SH_PhotonVoiceListener::remoteVoiceInfoCallback(void* opaque, int channelId, int playerId, nByte voiceId, VoiceInfo const& voiceInfo, RemoteVoiceOptions& options)
{
	static_cast<SH_PhotonVoiceListener*>(opaque)->remoteVoiceInfoCallback(channelId, playerId, voiceId, voiceInfo, options);
}

//���⼭ �÷��̾� ���� �ݹ�
void SH_PhotonVoiceListener::remoteVoiceInfoCallback(int channelId, int playerId, nByte voiceId, VoiceInfo const& voiceInfo, RemoteVoiceOptions& options)
{
	AudioOutDelayConfig delayConfig(200, 500, 1000, 5);

	Common::JString str = ((Common::ValueObject<Common::JString>)mCharacterInfo.getValue(playerId)).getDataCopy();
	FString UserID = UTF8_TO_TCHAR(str.UTF8Representation().cstr());

	AActor_PhotonAudioOut* out = mpAudioOutFactory->createAudioOut();
	out->init(
		*mpTransport
		, delayConfig
		, Common::JString(L"Audio Player p#") + playerId + L", v#" + voiceId
		, true,playerId , UserID);

	IAudioOut<short>* p = out->getPlayer();
	p->start(voiceInfo.getSamplingRate(), voiceInfo.getChannels(), voiceInfo.getFrameDurationSamples());

	//���� ���
	mAudioPlayers.Add(out);
	mBasic->Voice_UpdateFriend();


	//���� �ݹ� ����
	options.setRemoteVoiceRemoveAction(new RemoveInfo(this, out), remoteVoiceRemoveCallback);

	options.setOutput(out->getPlayer(), frameDataCallback);
}


void SH_PhotonVoiceListener::JoinRoom()
{
	RoomOptions opt;
	if (DIRECT)
		opt.setDirectMode(DirectMode::ALL_TO_ALL);
	opt.setPublishUserID(true);
	opt.setMaxPlayers(0);


	mLoadBalancingClient->opJoinOrCreateRoom(RoomName, opt);
}


SH_PhotonVoiceListener::SH_PhotonVoiceListener(IAudioInFactory* audioInFactory, IAudioOutFactory* audioOutFactory, SH_PhotonVoiceBasic* PhotonVoiceBasic)
#ifdef _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
: mVoicesCreated(false)
, mpAudioInFactory(audioInFactory)
, mpAudioOutFactory(audioOutFactory)
, mBasic(PhotonVoiceBasic)
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
{
	mLogger.setListener(*this);
	mLogger.setDebugOutputLevel(DEBUG_RELEASE(Common::DebugLevel::INFO, Common::DebugLevel::WARNINGS)); // this 

	Common::Base::setListener(this);
	Common::Base::setDebugOutputLevel(DEBUG_RELEASE(Common::DebugLevel::INFO, Common::DebugLevel::WARNINGS)); // all classes that inherit from Base
}


SH_PhotonVoiceListener::~SH_PhotonVoiceListener(void)
{
	for (unsigned int i = 0; i < mAudioSources.size(); i++)
	{
		delete mAudioSources[i];
	}
	for (unsigned int i = 0; i < mLocalVoices.size(); i++)
	{
		mLocalVoices[i]->removeSelf();
	}

	// sources and players are AActor's destroyed automatically
	delete mpVoiceClient;
	delete mpTransport;
}

void SH_PhotonVoiceListener::connect(const Common::JString& name, const Common::JString& serverAddress)
{
	mLoadBalancingClient->connect(ConnectOptions(AuthenticationValues().setUserID(name), name, serverAddress));

	// �־��� LoadBalancing Ŭ���̾�Ʈ�� ������� VoiceClient�� ���� ������ �����մϴ�.
	// VoiceClient ���� �� ����
	mpTransport = new LoadBalancingTransport(*mLoadBalancingClient, *this, DIRECT);

	//���� Ŭ���̾�Ʈ�� IVoiceTransport�� ���� ��Ʈ��ũ�� �ٸ� Ŭ���̾�Ʈ�� ��ȣ �ۿ��մϴ�.
	mpVoiceClient = new VoiceClient(mpTransport);
	mpVoiceClient->setOnRemoteVoiceInfoAction(this, remoteVoiceInfoCallback);//���⼭ ��Ʈ��ũ �����ϴ� �÷��̾� ������ �ݹ��Լ� ����

	mpTransport->setDebugOutputLevel(DEBUG_RELEASE(Common::DebugLevel::INFO, Common::DebugLevel::INFO)); // all instances of VoiceClient that use mpTransport //mpTransport�� ����ϴ� VoiceClient�� ��� �ν��Ͻ�
	mLoadBalancingClient->setDebugOutputLevel(DEBUG_RELEASE(Common::DebugLevel::INFO, Common::DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details // �ش� LoadBalancingClient �ν��Ͻ� �� �ش� ���� ���� ����
}

void SH_PhotonVoiceListener::update(void)
{
	mLoadBalancingClient->service();
	mpTransport->service();
	mpVoiceClient->service();

	//�÷��̾�� ������Ʈ ���ش�.
	for (auto it : mAudioPlayers)
	{
		it->service();
	}
}

void SH_PhotonVoiceListener::SetClient(ExitGames::LoadBalancing::Client* client)
{
	this->mLoadBalancingClient = client;
}

void SH_PhotonVoiceListener::disconnect(void)
{
	mLoadBalancingClient->disconnect();
}

//���� ��ȯ(�� ����� �Է� �׽�Ʈ��)
void SH_PhotonVoiceListener::toggleEcho(void)
{
	for (unsigned int i = 0; i < mLocalVoices.size(); i++)
	{
		mLocalVoices[i]->setDebugEchoMode(!mLocalVoices[i]->getDebugEchoMode());
		Console::get().writeLine(mLocalVoices[i]->getName() + L" echo: " + (mLocalVoices[i]->getDebugEchoMode() ? L"ON" : L"OFF"));
	}
}


void SH_PhotonVoiceListener::debugReturn(int debugLevel, const Common::JString& string)
{
	FString str = UTF8_TO_TCHAR(string.UTF8Representation().cstr());
	switch (debugLevel)
	{
	case Common::DebugLevel::ERRORS:
		mBasic->ErrorCheckMessage("Error//" + str, debugLevel);
		break;
	case Common::DebugLevel::WARNINGS:
		mBasic->ErrorCheckMessage("WARNINGS//" + str, debugLevel);
		break;
	default:
		mBasic->ErrorCheckMessage(str, debugLevel);
		break;
	}
}


//�÷��̾ �濡 �����Ҷ� ���δ�
void SH_PhotonVoiceListener::joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const Player& player)
{
	LoadBalancingListener::joinRoomEventAction(playerNr, playernrs, player);

	EGLOG(Common::DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
	//Console::get().writeLine(L"");
	Console::get().writeLine(Common::JString(L"player ") + playerNr + L" " + player.getName() + L" has joined the game");

	//���ʷ� �������� �����Ѵ�.
	if (!mVoicesCreated)
	{
		IAudioPusher<short>* audioSource2 = new AudioIn(mpAudioInFactory);

		
		VoiceInfo i2 = VoiceInfo::createAudioOpus(16000, audioSource2->getChannels(), 20000, 30000);
		//CreateLocalVoiceAudioFromSource : �߽� ����� ��Ʈ���� �����ϰ� ����� �ҽ� �����͸� �Һ��ϱ� ���� ������ �߰��Ѵ�.ó�� ���������ΰ� ��ȯ�Ǵ� ��Ʈ�� �ڵ鷯�� ����� ���ñ�� �߰��Ѵ�.
		LocalVoiceAudio<short>* v2 = mpVoiceClient->createLocalVoiceAudioFromSource(i2, audioSource2, 0);
		v2->setDebugEchoMode(DEBUG_ECHO_MODE_INIT);
		v2->getVoiceDetector()->setOn(false);
		mAudioSources.push_back(audioSource2);
		mLocalVoices.push_back(v2);
		
		//Cast<AudioIn>(audioSource2)->MuteInputSound(true);
		v2->setTransmitEnabled(true);//�̰ɷ� ����� Ȱ��ȭ�� ������ �� ����
		mVoicesCreated = true;
		{
			FString str = UTF8_TO_TCHAR(v2->getName().UTF8Representation().cstr());
			UE_LOG(LogTemp, Log, TEXT("//joinRoomEventAction Init // v2 Player Name :: %ls , "), *str);
		}


		MutableRoom& myRoom = mLoadBalancingClient->getCurrentlyJoinedRoom();
		const Common::JVector<Player*>& players = myRoom.getPlayers();
		int size = players.getSize();
		for (int i = 0; i < size; ++i)
		{
			const Player* p = players[i];
			mCharacterInfo.put(p->getNumber(), p->getName());

		}
	}
	else
	{
		MutableRoom& myRoom = mLoadBalancingClient->getCurrentlyJoinedRoom();
		const Common::JVector<Player*>& players = myRoom.getPlayers();
		int size = players.getSize();
		for (int i = 0; i < size; ++i)
		{
			const Player* p = players[i];

			if ( p->getNumber() == playerNr)
			{
				mCharacterInfo.put(p->getNumber(), p->getName());
			}
		}
	}

}

//�÷��̾ �濡�� ������
void SH_PhotonVoiceListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	LoadBalancingListener::leaveRoomEventAction(playerNr, isInactive);
	//Common::JString str= ((Common::ValueObject<Common::JString>)(mCharacterInfo.getValue(playerNr)))->getDataCopy();
	EGLOG(Common::DebugLevel::INFO, L"");


	Common::JString str = ((Common::ValueObject<Common::JString>)mCharacterInfo.getValue(playerNr)).getDataCopy();
	FString str2 = UTF8_TO_TCHAR(str.UTF8Representation().cstr());
	
	
	UE_LOG(LogTemp, Log, TEXT("//leaveRoomEventAction End//UserID :: %s , Number :: %d"), *str2, playerNr);
	mCharacterInfo.remove(playerNr);
}

void SH_PhotonVoiceListener::customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContentObj)
{
	LoadBalancingListener::customEventAction(playerNr, eventCode, eventContentObj);

	Common::Hashtable eventContent = Common::ValueObject<Common::Hashtable>(eventContentObj).getDataCopy();

	//Object const* obj = eventContent.getValue("1");


	switch (eventCode)
	{
	case 0:
		if (eventContent.getValue((nByte)0))
			//mReceiveCount = ((Common::ValueObject<int64>*)(eventContent.getValue((nByte)0)))->getDataCopy();
		break;
	default:
		break;
	}
}


void SH_PhotonVoiceListener::connectReturn(int errorCode, const Common::JString& errorString, const Common::JString& region, const Common::JString& cluster)
{
	EGLOG(Common::DebugLevel::INFO, L"connected to cluster " + cluster + L" of region " + region);
	if (errorCode)
	{
		EGLOG(Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		return;
	}
	Console::get().writeLine(L"connected to cluster " + cluster);

	mBasic->Voice_ConnectComplete();
}

void SH_PhotonVoiceListener::disconnectReturn(void)
{
	LoadBalancingListener::disconnectReturn();

	EGLOG(Common::DebugLevel::INFO, L"");
	//Console::get().writeLine(L"disconnected");
}

void SH_PhotonVoiceListener::createRoomReturn(int localPlayerNr, const Common::Hashtable& gameProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString)
{
	LoadBalancingListener::createRoomReturn(localPlayerNr, gameProperties, playerProperties, errorCode, errorString);

	EGLOG(Common::DebugLevel::INFO, L"");
	if (errorCode)
	{
		EGLOG(Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		//Console::get().writeLine(L"opCreateRoom() failed: " + errorString);

		MutableRoom& myRoom = mLoadBalancingClient->getCurrentlyJoinedRoom();
		ExitGames::Common::Hashtable props = myRoom.getCustomProperties();

		return;
	}

	EGLOG(Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	isconnected = true;
}

//�ַ� ���� joinorceate
void SH_PhotonVoiceListener::joinOrCreateRoomReturn(int localPlayerNr, const Common::Hashtable& gameProperties, const Common::Hashtable& playerProperties, int errorCode, const Common::JString& errorString)
{
	MutableRoom& myRoom = mLoadBalancingClient->getCurrentlyJoinedRoom();
	Common::Hashtable props = myRoom.getCustomProperties();

	if (errorCode)
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("//joinOrCreateRoomReturn// voicde room :: %ls"), myRoom.getName().cstr());

	
	const ExitGames::Common::JVector<Player*>& players = myRoom.getPlayers();
	int size = players.getSize();
	for (int i = 0; i < size; ++i)
	{
		const Player* p = players[i];
		UE_LOG(LogTemp, Log, TEXT("//joinOrCreateRoomReturn// Number : %d , user id :: %ls"), p->getNumber(), p->getName().cstr());
	}

	IsChanging = false;
}

void SH_PhotonVoiceListener::joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
{
}

void SH_PhotonVoiceListener::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
{
}

void SH_PhotonVoiceListener::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
{
}

void SH_PhotonVoiceListener::leaveRoomReturn(int errorCode, const Common::JString& errorString)
{
	//LoadBalancingListener::leaveRoomReturn(errorCode, errorString);
	if (mVoicesCreated)
	{
		mVoicesCreated = false;
		for (unsigned int i = 0; i < mAudioSources.size(); i++)
		{
			delete mAudioSources[i];
		}
		mAudioSources.clear();
		for (unsigned int i = 0; i < mLocalVoices.size(); i++)
		{
			mLocalVoices[i]->removeSelf();
		}
		mLocalVoices.clear();
	}

	if (IsChanging)
	{
		JoinRoom();
		return;
	}
	else if (errorCode)
	{
		return;
	}
}


void SH_PhotonVoiceListener::connectionErrorReturn(int errorCode)
{
	FString str = FString::Printf(TEXT("//received connection error %d "), errorCode);
	mBasic->ErrorCheckMessage(str, errorCode);
}

void SH_PhotonVoiceListener::clientErrorReturn(int errorCode)
{
	FString str = FString::Printf(TEXT("//received error %d "), errorCode) + " from client";
	mBasic->ErrorCheckMessage(str, errorCode);
}

void SH_PhotonVoiceListener::warningReturn(int warningCode)
{
	FString str = FString::Printf(TEXT("//received warning %d "), warningCode) + " from client";
	mBasic->ErrorCheckMessage(str, warningCode);
}

void SH_PhotonVoiceListener::serverErrorReturn(int errorCode)
{
	FString str = FString::Printf(TEXT("//received error %d "), errorCode) + " from server";
	mBasic->ErrorCheckMessage(str, errorCode);
}


void SH_PhotonVoiceListener::onAvailableRegions(const Common::JVector<Common::JString>& /*availableRegions*/, const Common::JVector<Common::JString>& /*availableRegionServers*/)
{
	mLoadBalancingClient->selectRegion(L"kr");
}

