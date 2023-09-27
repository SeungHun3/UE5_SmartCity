// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Player.h"
#include "Photon/PhotonListner_Solaseado.h"
#include "ActorComponent_Playfab.h"
#include "GameFramework/Actor.h"
#include "Actor_SolaseadoPhoton.generated.h"


UENUM(BlueprintType)
enum class Enum_TextType3 : uint8
{
	Normal = 0,
	World,		// 10sec
	infinite,	// infinite
	Custom,
};

//�ӽ÷� ������� �Ŷ� ���� ���濹��
//�ƹ�Ÿ Ÿ��
UENUM(BlueprintType)
enum class enum_CostumeType : uint8
{
	Top = 0,
	Bottom,
	Hair,
	Face,
	Skin,
	Default,
};

//�ƹ�Ÿ ���� ���� ����ü
USTRUCT(BlueprintType)
struct FCostume
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		enum_CostumeType Type = enum_CostumeType::Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PartNumber = 0;
};


UCLASS()
class TEST_LIDARSAMPLE_API AActor_SolaseadoPhoton : public AActor, public SH_PhotonBasic
{
	GENERATED_BODY()

public:
	AActor_SolaseadoPhoton();
	virtual ~AActor_SolaseadoPhoton();

	// ���� ���� ���� ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SH_Photon")
		FString serverAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SH_Photon")
		FString AppID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SH_Photon")
		FString appVersion = FString("1.2.0");

	//�÷��̾� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<class APawn_Player*> PlayerList;
	// ���� �÷��̾� ĳ����
	UPROPERTY(EditAnywhere)
		TSubclassOf<class APawn_Player> targetCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ������Ʈ �ֱ� 100ms
	unsigned long lastUpdateTime;

	// ���� Ŭ���̾�Ʈ
	ExitGames::LoadBalancing::Client* m_pClient;
	PhotonListner_Solaseado* m_pListener;


	// ���� ���� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString CurrentRoomName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		uint8 CurrentPeople;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		uint8 MaxPeople;

	// �÷��̾� ����
	class APawn_Player* LocalPlayer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �������� �� �̺�Ʈ. 0 ���� , 1~ Ư�� �̺�Ʈ
	UFUNCTION(BlueprintCallable)
		void ChangeRoomEventProperty(uint8 Ev);

	// ĳ���� �ִϸ��̼� ����
	UFUNCTION(BlueprintCallable)
		void setPlayerAnimationData(enum_PlayerAnimationState anim);
	// ���� ���� ���� 
	UFUNCTION(BlueprintCallable)
		void ConnectLogin(const FString& username);

	// Blueprint Ros Actor Spawn 
	UFUNCTION(BlueprintImplementableEvent)
		void ConnectRosActor();

	//////////////////////////////////////////////// 
	// Blueprint Widget Binding Func
	UFUNCTION(BlueprintImplementableEvent)
		void Blueprint_RemovePlayer(int PlayerNr);
	UFUNCTION(BlueprintImplementableEvent)
		void Blueprint_AddPlayer(int PlayerNr, const FString& name);
	UFUNCTION(BlueprintImplementableEvent)
		void Blueprint_ResetPlayer();

	//���۽� ��ġ �����ִ� �뵵
	UFUNCTION(BlueprintCallable)
		void MoveAndRotation(FVector Loc, int vYaw);

	// ���� ������, ���� ����
	void ReconnectMessage();
	void PhotonDisconnect();

	// ���� ���� ���� ���� 
	virtual void setRegion(ExitGames::LoadBalancing::Client* Client) override;

	// ���� ���
	virtual void ErrorCheckMessage(const FString& message, int error) override;

	// ���� ����� �ʱ�ȭ
	virtual void InitPlayers(void);
	// ���� ����� �߰�
	virtual void AddPlayers(int playerNr, const ExitGames::Common::JString& playerName, bool local, const ExitGames::Common::Hashtable& Custom) override;
	// ���� ����� ����
	virtual void RemovePlayer(int playerNr) override;
	// ���� ����� ������Ƽ ������Ʈ
	virtual void updatePlayerProperties(int playerNr, const Hashtable& changes) override;
	// ���� ����� �� ���� ���� // ������Ƽ ������Ʈ
	virtual void updateRoomProperties(const Hashtable& changes) override;

	// ���� ����� ���� ���� ������Ʈ
	virtual void GetMoveAndRotation(int playerNr, int vX, int vY, int vZ, int vYaw) override;

	//����� �ִϸ��̼� ���� ������Ʈ
	virtual void GetPlayerAnim(int playerNr, int Anim) override;
	// Connect
	virtual void ConnectComplete(void) override;

	virtual void CreateChannelComplete(const ExitGames::Common::JString& map, const ExitGames::Common::JString& channel) override;
	virtual void CreateRoomComplete(const ExitGames::Common::JString& map) override;
	virtual void JoinRoomComplete(const ExitGames::Common::JString& map, const ExitGames::Common::JString& channel) override;
	virtual void JoinOrCreateComplete(const FString& RoomFullName) override;

	virtual void LeaveRoomComplete(void) override;
	virtual void CurrentRoomInfo(const ExitGames::Common::JString& name, nByte Count, nByte MaxCount) override;

	//ģ�� ��û �ޱ�
	virtual void RecvFriendRequest(int PlayerNum, const FString& FriendPlayFabID) override;
public:
	// ĳ���� ���� ������(�ƹ�Ÿ...) �߰� �Լ�
	UFUNCTION(BlueprintCallable)
		void InputCharacterInfo(FString _key, FString _value);
	UFUNCTION(BlueprintCallable)
		void SendPlayerInfo();
	UFUNCTION(BlueprintImplementableEvent)
		void ConnectPhotonCHat();

	//�濡 ���� ���� playfab���� ���� �÷��̾� �����͸� ä���ֱ�
	UFUNCTION(BlueprintNativeEvent)
		void InitPlayerData();
	virtual void InitPlayerData_Implementation();

	//�ִϸ��̼� ���� �����͸� �����ִ� �Լ�
	UFUNCTION(BlueprintCallable)
		void SendCostumeParts(FString Parts);

	//�ִϸ��̼� ���� �����͸� �����ִ� �Լ�
	UFUNCTION(BlueprintCallable)
		void InputAnimationState(enum_PlayerAnimationState _State);

	// ģ�� ��û
	UFUNCTION(BlueprintCallable)
		void SendFriendInfo(int Target, FString PlayfabID);


protected:
	//�÷��̾� �ڽ�Ƭ ����
	int DataCount = 0;
	////////////////////////////////////
	/////////// ä�� ///////////////////   cpp 140
protected:
	ExitGames::LoadBalancing::Client* dummy_pClient;
	PhotonListner_Solaseado* dummy_pListener;

	bool IsChangingRoom = false;
public:

	// �� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<int, int> ChannelList;

	// �溯��
	UFUNCTION(BlueprintCallable)
		void ChangeRoom(int Number);

	UFUNCTION(BlueprintCallable)
		void OpenDummy();

	UFUNCTION(BlueprintCallable)
		virtual	void UpdateRoomList(const TMap<int, int>& Channel_Count) override;

	UFUNCTION(BlueprintCallable)
		void CloseDummy();

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeViewCount(const TMap<int, int>& Channel_Count, const TArray<FString>& PlayerNames);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetMyChannelNumber();

	////////////////////////////////////////////////ä�� ��


	////////////////////////////////////////////////
	////�̵�����////////////////////////////////////////
	UFUNCTION(BlueprintCallable)
		void PhotonMove(FVector Loc);
	UFUNCTION(BlueprintCallable)
		void PhotonStop();
	UFUNCTION(BlueprintCallable)
		void PhotonFinish(FVector Loc);

	virtual void UpdateMove(int PlayerNum, FVector Loc) override;
	virtual void UpdateStop(int PlayerNum) override;
	virtual void UpdateStopFinished(int PlayerNum, FVector Loc) override;

	////////////////////////////////////////////////�̵� ��


	//NPC ����///////////
public:
	//NPC ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<class APawn_NPC*> NPCList;
	// ���� NPC
	UPROPERTY(EditAnywhere)
		TSubclassOf<class APawn_NPC> targetNPC;

	//NPC ���� ���������̺�
	UDataTable* CustomNPCDataTable;

	virtual void InitNpc() override;
	virtual void AddNpc(const FString& NpcId, FVector Loc, FVector Rot, FVector Scale) override;
	virtual void ClearNpc() override;

	//NPC �̵�
	UFUNCTION(BlueprintCallable)
		void NPCMove(const APawn_NPC* pawnNPC, FVector NpcLocation);

	//NPC ���� ��///////////
};