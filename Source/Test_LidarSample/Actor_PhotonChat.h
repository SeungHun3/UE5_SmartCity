// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Photon/SH_PhotonChatListener.h"
#include "../Photon/Chat-cpp/inc/Client.h"
#include "Actor_PhotonChat.generated.h"

UENUM(BlueprintType)
enum class enum_PhotonChat : uint8
{
	Public, // �Ϲ� ä�ε�
	Channel,
	Private, // �ӼӸ�.
	TEST,
	Max,
	World,
};


UCLASS()
class TEST_LIDARSAMPLE_API AActor_PhotonChat : public AActor , public SH_PhotonChatBasic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_PhotonChat();

	// ä�� ä�� ����Ʈ 
	TArray<FString> SubscribeList;

	// ���� ä�� ä��
	FString targetSubscribe;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Test
	UFUNCTION(BlueprintCallable)
	TArray<FString> getSubscribeList() {
		return SubscribeList;
	}
	// Message 
	UFUNCTION(BlueprintCallable)
		void SendMessageEvent(const FString& Message);
	UFUNCTION(BlueprintCallable)
		void SendPrivateMessage(const FString& username, const FString& message);
	// set Message Subscribe
	UFUNCTION(BlueprintCallable)
		void setTargetSubscribe(enum_PhotonChat target); // targetSubscribe
	
	// Subscribe ä�� �߰�
	UFUNCTION(BlueprintCallable)
		void AddSubscribeEvent(TArray<FString> sub);

	// �÷����� �α��� ������ ȣ�� �Ͽ� id ����ȭ
	UFUNCTION(BlueprintCallable)
		void Connect_Chat(const FString& playfabid);

	////////////////////////////////////////////////////////////////////////////
	// Basic Class override / SH_PhotonChatListener
	
	// ���� ê ���� ���� 
	void Chat_ConnectComplete(void) override;
	void Chat_getMessageEvent(const FString& ChannelName, const FString& sender, const FString& Message) override;

	// ���� ä�� �߰� 
	void Chat_AddSubscribe(const FString& Channel) override;
	// ���� ä�� ����
	void Chat_RemoveSubscribe(const FString& Channel) override;

	////////////////////////////////////////////////////////////////////////////
	// Blueprint Binding Func
	UFUNCTION(BlueprintImplementableEvent)
		void Blueprint_ConnectComplete(); // ���̽� ��������
	UFUNCTION(BlueprintImplementableEvent)
		void Blueprint_getMessageEvent(const FString& ChannelName, const FString& Sender, const FString& Message);
public:
	class SH_PhotonChatListener* m_pListener;

	ExitGames::Chat::Client* m_pChatClient;

	FString m_RoomName;
	void Chat_ResetJoinChannel(const FString& FullName);
};
