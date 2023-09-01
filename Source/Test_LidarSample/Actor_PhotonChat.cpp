// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_PhotonChat.h"

#define CHAT_AppID "bd96a445-11ab-4bc6-bb45-71b681b17273"
#define WorldChannel "World"
using namespace ExitGames;

// Sets default values
AActor_PhotonChat::AActor_PhotonChat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pListener = new SH_PhotonChatListener(this);
	m_pChatClient = new ExitGames::Chat::Client(*m_pListener, Common::JString(CHAT_AppID), Common::JString("ns.exitgames.com"));

	m_pListener->setChatClient(m_pChatClient);
}

void AActor_PhotonChat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (m_pListener)
		delete m_pListener;
	if (m_pChatClient)
		delete m_pChatClient;
}
// Called when the game starts or when spawned
void AActor_PhotonChat::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AActor_PhotonChat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_pChatClient)
		m_pChatClient->service();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// connect
// check playfab login id 
void AActor_PhotonChat::Connect_Chat(const FString& playfabid)
{
	// �׽�Ʈ ����
	FString UserID = playfabid;
	if (m_pChatClient)
	{
		ExitGames::Chat::AuthenticationValues AuthenticationValue;
		AuthenticationValue.setUserID(TCHAR_TO_UTF8(*UserID));
		
		m_pChatClient->connect(AuthenticationValue);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Message
// ä�� �߰��ϱ�
void AActor_PhotonChat::AddSubscribeEvent(TArray<FString> sub)
{
	Common::JVector<Common::JString> Subscribechannels;
	for (auto it : sub)
	{
		FString Subscribe = it;
		Subscribechannels.addElement(TCHAR_TO_UTF8(*Subscribe));
	}
	m_pChatClient->opSubscribe(Subscribechannels);
	
}
// �Ϲ� �޼��� 
void AActor_PhotonChat::SendMessageEvent(const FString& Message)
{
	// Channel Name;
	// ���� ä�� ����Ʈ ���� �� Ÿ�� ä��(targetSubscribe)�� �޼��� ������
	
	m_pChatClient->opPublishMessage(TCHAR_TO_UTF8(*targetSubscribe)  , TCHAR_TO_UTF8(*Message));
}
// �ӼӸ� 
void AActor_PhotonChat::SendPrivateMessage(const FString& username, const FString& message)
{
	m_pChatClient->opSendPrivateMessage(TCHAR_TO_UTF8(*username), TCHAR_TO_UTF8(*message));
}
// ä�� Ÿ�� ����  // �� -> ���� or �Ӹ� 
void AActor_PhotonChat::setTargetSubscribe(enum_PhotonChat target)
{
	if (target == enum_PhotonChat::World)
	{
		targetSubscribe = WorldChannel;
	}
	else {
		targetSubscribe = m_RoomName + "_" + FString::FromInt((int)target);
	}
	// ���濡 ���� UI ����
	// ChangeTargetSubscribe(target);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Basic Class override / Binding Event 
void AActor_PhotonChat::Chat_ConnectComplete(void)
{
	
	// Blueprint
	Blueprint_ConnectComplete(); // ���̽�ä�� ��������
	
	
}
// ������ �߰� 
void AActor_PhotonChat::Chat_AddSubscribe(const FString& Channel)
{
	SubscribeList.AddUnique(Channel);
}
// ������ ���� 
void AActor_PhotonChat::Chat_RemoveSubscribe(const FString& Channel)
{
	SubscribeList.Remove(Channel);
}

// ä�� ����
void AActor_PhotonChat::Chat_ResetJoinChannel(const FString& FullName)
{// Ŭ���̾�Ʈ ���� �ִٸ� ���� ����� ���� ����

	//��������Ʈ ������ �ִٸ� ���� �� �ʱ�ȭ
	if (SubscribeList.IsValidIndex(0))
	{
		Common::JVector<Common::JString> Subscribechannels;
		for (auto it : SubscribeList)
		{
			Subscribechannels.addElement(TCHAR_TO_UTF8(*it));
		}
		m_pChatClient->opUnsubscribe(Subscribechannels);
	}
	SubscribeList.Reset();


	m_RoomName = FullName;

	for (int i = 0; i < (int)enum_PhotonChat::Max; i++)
	{
		FString ChannelName = m_RoomName + "_" + FString::FromInt(i); // �Ϲ�, �ӼӸ� ...
		SubscribeList.Add(ChannelName);
	}
	FString WorldName = WorldChannel;
	SubscribeList.Add(WorldName);
	AddSubscribeEvent(SubscribeList);

	// ����Ʈ ä�� ����
	setTargetSubscribe(enum_PhotonChat::Public);

}
// �޼��� ��� 
void AActor_PhotonChat::Chat_getMessageEvent(const FString& ChannelName, const FString& sender, const FString& Message)
{
	// �޼��� ���� ���
	Blueprint_getMessageEvent(ChannelName, sender, Message);
}