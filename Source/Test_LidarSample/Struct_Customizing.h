// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/SkeletalMesh.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"

#include "Struct_Customizing.generated.h"
/**
 *
 */

 // ������ ����
USTRUCT(BlueprintType)
struct FItemproperty
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemInstanceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int RemainingUses=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int UnitPrice =0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString colorData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bItemEquipment = false;

public:
		void Clear() {
		ItemId.Empty();
		ItemInstanceId.Empty();
		ItemClass.Empty();
		RemainingUses = -1;
		UnitPrice = -1;
	};
};

USTRUCT(Atomic, BlueprintType)
struct FITemInfo
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ItemPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString StoreID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString VirtualCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString CatalogVersion;

};

USTRUCT(Atomic, BlueprintType)
struct FCustomizing_Struct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ClassName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Image;
};

class TEST_LIDARSAMPLE_API Struct_Customizing
{
public:
	Struct_Customizing();
	~Struct_Customizing();
};

///�˾�////

UENUM(BlueprintType)
enum class enum_PopupRun : uint8
{
	Exit,
	Login,
	CreateRoom,
	// �κ� , ä�� 
	JoinRoom,
	// �κ� , ä�� 
	LeaveRoom, // inLobby
	Channel,
	// ĳ���� ����
	CreateCharacter,
	// ������ ����
	Purchage,
	// ������ �Ǹ�
	Sell,
	// ����
	Notice,
	// ģ�� �߰� �޼��� ������
	AddFriendMessage,
	// ģ�� �߰�
	AddFriend,
	MoveChannel,
	Cancel,
	// Ű ���� �˾�
	Tutorial,
	StartStoryTutorial,
};
//�˾� ��Ÿ��
UENUM(BlueprintType)
enum class enum_PopupStyle : uint8
{
	Popup1Key,
	Popup2Key,
};


// ����, ���������̺��
USTRUCT(Atomic, BlueprintType)
struct FChecking_Reward : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Gold = 0;
};

// ����Ʈ �������
USTRUCT(Atomic,BlueprintType)
struct FQuest_List : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UDataTable* QuestData;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
		FString Explain;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int reward =0;
};

USTRUCT(Atomic, BlueprintType)
struct FQuest_Info : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Quest_Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Quest_Step = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<bool> IsFinished;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* QuestTable = nullptr;
};

UENUM(BlueprintType)
enum class enum_Quest_Condition : uint8
{
	NotStart, // ���۰���
	IsDoing, // ������
	LastFinished,// ������ ����ޱ��ư Ŭ����������
	IsComplete, // �Ϸ�

};
UENUM()
enum class enum_Quest_Update : uint8	//���� ������Ʈ��
{
	Drop, // ����Ʈ����
	Start, // ����
	Complete, // �Ϸ�
};
//����
USTRUCT(Atomic, BlueprintType)
struct FAchieve_Info : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Explain;

};

//Npc Main
USTRUCT(Atomic, BlueprintType)
struct FDialogueNpc_List : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int NpcName = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* NpcDialogueTable;
};
//Npc ��ȭ ����
USTRUCT(Atomic, BlueprintType)
struct FDialogueNpc_Content :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int idx = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Message;
	//��ư ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int keyNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Key1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FText Key2;
	//�̺�Ʈ�� �ִ� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool RowEvent = false;
};
//Npc ��ȭâ
UENUM(BlueprintType)
enum class enum_DialogueStyle : uint8
{
	Dialogue1Key,
	Dialogue2Key,
};


//ģ��
USTRUCT(BlueprintType)
struct FFriendStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TitleID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString PlayFabID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDateTime LastLogin;
};


// �ɼ� ��Ÿ��
UENUM(BlueprintType)
enum class enum_OptionStyle : uint8
{
	OptionSound, //����
	OptionAlarm, //�˸�
	OptionAccount, //����
};
//�ɼ� ����
UENUM(BlueprintType)
enum class enum_Option : uint8
{
	Logout, //�α׾ƿ�
	GameExit, //��������
	Withdrawal, //����Ż��
	AlarmAgreement, //�˸�����
};