// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "Engine/DataTable.h"
#include "Struct_Customizing.h"

#include "Components/ActorComponent.h"
#include "ActorComponent_Playfab.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateFriend);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateFriendSlot);


// ��Ģ�� ���� 
USTRUCT(BlueprintType)
struct FBadNameTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString BadString;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST_LIDARSAMPLE_API UActorComponent_Playfab : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorComponent_Playfab();

	// PlayFab Property
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString PlayFabID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString LastLoginTime;

	// Playfab DisplayName
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString UserCharacterName;
	// ���� �÷��� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString > UserTitleData;

	// ĳ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCheckerCreateChracter = false;
	// �г��� ��Ģ�� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bForbidden = false;

	////////////////////////////////////////////////////////////
	// ����������= { �α���, �⼮, �̼�}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, int>  PlayFab_Statistics;

	// ����Ʈ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuest_Info> MyQuest_Info;	// ���� ���� ����Ʈ�� ���

	//����
	FString AchieveName = "achieve_";


	////////////////////////////////
	//ģ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FFriendStruct> FriendList;	//���� ģ�� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FFriendStruct> BlockList;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FFriendStruct> FriendRequestList;	//ģ�� ��û ���

	UPROPERTY(BlueprintAssignable)
	FUpdateFriend UpdateFriend;

	//ģ��â ���� ������Ʈ 
	UPROPERTY(BlueprintAssignable)
	FUpdateFriendSlot UpdateFriendSlot;
	//////////////////////////////////////////

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Playfab ��ũ��Ʈ ȣ�� ����
	void ErrorScript(const PlayFab::FPlayFabCppError& error);
	// Playfab ��ũ��Ʈ ȣ�� �ݺ�
	void SuccessScriptEvent(const PlayFab::ClientModels::FExecuteCloudScriptResult& result);

	FString ErrorFunctionName;

	// Player Playfab ������ ������ ���� ���ε�
	class APawn_Player* PlayerOwner;

	// �κ��丮 ������ ���� ����Ʈ
	TArray<FItemproperty> InventoryProperty;

	// ��Ģ�� ������ ���̺�
	UDataTable* BadNameTable;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ��Ģ�� üũ 
	bool FindCheckBadName(const FString& name);

	// Playfab ���� �α���
	UFUNCTION(BlueprintCallable)
		void Login_Custom(const FString& customid);
	UFUNCTION(BlueprintCallable)
		void Login_Email(const FString& email, const FString& pw);
	// ���� �г��� ������Ʈ (�ߺ� üũ, ����)
	UFUNCTION(BlueprintCallable)
		void updateUserTitleName(const FString& DisplayName);
	// �ΰ��� ���� �� ���� ������ ���� �ε�
	UFUNCTION(BlueprintCallable)
		void getIngamePlayerData();
	// �κ��丮 ������ ���� ������
	UFUNCTION(BlueprintCallable)
		const TArray<FItemproperty> getInventoryItemList() { return InventoryProperty; }

	//////////////////////////////////////////////////////////////////
	//// Playfab ��ũ��Ʈ ȣ��
	//// ��ũ��Ʈ �Ű����� x
	UFUNCTION(BlueprintCallable)
		void ScriptCustomNotField(const FString& FunctionName);
	//// ��ũ��Ʈ �Ű����� o
	UFUNCTION(BlueprintCallable)
		void ScriptCustom(const FString& FunctionName, const FString& FieldName, const FString& value);
	//// ��ũ��Ʈ �Ű����� �迭
	UFUNCTION(BlueprintCallable)
		void ScriptCustomArray(const FString& FunctionName, const FString& FieldName, const TArray<FString>& StringArray);

	////////////////////////////////////////////////////////////
	// �ڽ�Ƭ
	// �����ITemIDs�� �迭�� PlayFab �� FunctionName ��ũ��Ʈ�� ����
	UFUNCTION(BlueprintCallable)
	void UploadMyCustom(const FString& FunctionName, const FString& FieldName, const TArray<int> ItemIDs);




	/////////////////////////////////////////////////////////////
	//����Ʈ
	
	// ����Ʈ ����ü�迭���� ���ǿ� �´� �ε��� ��ȯ
	int FindQuestInfo_Index(const FString& QuestName);
	// PlayFab_Statistics Ž�� -> ����Ʈ ������ Title������ Ž�� �� ����ü Add
	void CheckQuestInfo();
	// Json������ �״�� ������ ����ü�����
	FQuest_Info MakeQuestInfo(const FString& QuestName, const FString& JsonParseData);
	// ����Ʈ ���̺� ��������
	UDataTable* FindQuestTable(const FString& QuestName);
	// ����Ʈ Playfab TitleData������Ʈ
	void Quest_Update_Title(const FString& QuestName);
	// ����Ʈ Playfab Statistic������Ʈ
	void Quest_Update_Statistic(const FString& QuestName, enum_Quest_Update Update);

	FQuest_Info SetQuestInfo(const FString& QuestName, int Step);
	//���� ���൵�� ���� ���̺� �� ���ڻ̾Ƽ� �迭�������� // ������Ƽ Quest_Step �� ���൵�� Ž�� (FQuest_Info Quest; Quest.Quest_Step;)
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<int> GetQuestRowNames(const FString& QuestStepProp, class UDataTable* QuestTable);

	int QuestTotalStepcount(const FString& QuestName);


	UFUNCTION(BlueprintCallable)
	void Quest_Start(const FString& QuestName);
	UFUNCTION(BlueprintCallable)
	bool Quest_Finish(const FString& QuestName, int index);
	UFUNCTION(BlueprintCallable)
	void Quest_Next(const FString& QuestName);
	UFUNCTION(BlueprintCallable)
	void Quest_Drop(const FString& QuestName);

	void Quest_Complete(const FString& QuestName);

	bool Quest_Check_isDoing(const FString& QuestName);
	bool Quest_Check_IsComplete(const FString& QuestName);
	bool Quest_Check_IsLastFinished(const FString& QuestName);

	UFUNCTION(BlueprintCallable)
	enum_Quest_Condition Quest_Check_Condition(const FString& QuestName);

	////////////////////////////////////////////////////////////
	// ����, �÷����� �������� Name, Value(int) <-- �α��ν� ������Ʈ�� Statistics ������
	UFUNCTION(BlueprintCallable)
		void UpdateAchievement(int AchieveNumber);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		TArray<FString> GetAchievement();
	//�����__���� �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
		void Debug_ClearAchievement();


	// �⼮������ ��������
	UFUNCTION(BlueprintCallable)
	bool Is_Today_Checked();
	// �⼮ ī���� ��������
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int Get_Checking_Count();
	// �÷�����-> ���� ����ް� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void Update_Check_Change(class UWidget_CheckingAttandance_Main* Widget,int coin);
	//20�� �ϷẸ��
	UFUNCTION(BlueprintCallable)
	void Daily_20th_Reward(class UWidget_CheckingAttandance_Main* Widget,int Coin);

	//�����__�⼮üũ �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
	void Debug_Clear_Attandance(class UWidget_CheckingAttandance_Main* Widget);
	UFUNCTION(BlueprintCallable)
	void Debug_Clear_Daily(class UWidget_CheckingAttandance_Main* Widget);
	//�����
	
	
	////////////////////////////////////////////////////////////
	// Playfab ��ũ��Ʈ ȣ�� �ݺ� �̺�Ʈ ó��
	void ScriptResponseEvent(FJsonValue* value);

	//////////////////////////////////////////////////////////// 
	// Return Callback Event 
	// �α��� ����
	void SuccessLogin(bool NewlyCreated);
	// ���� Ÿ��Ʋ ������ ��������
	void getUserTitleData(FString targetTitle = FString(""));
	// ���� �г��� 
	void getUserTitleName();
	// ���� �κ��丮 ����
	void getInventoryList();
	// ���� ������
	void getStatisticsEvent();
	// ���� ����
	void getNoticeEvent(int NoticeCount);

	///////////////////////
	//ģ�� ����
	// 
	// ģ�� ����Ʈ ����
	UFUNCTION(BlueprintCallable)
	void getFriendListEvent();

	// ģ�� ������Ʈ
	void UpdateFriendList(const PlayFab::ClientModels::FGetFriendsListResult& result);

	// ģ�� ������Ʈ
	UFUNCTION(BlueprintCallable)
	void CallUpdateFriend();

	// ģ�� ������Ʈ
	UFUNCTION(BlueprintCallable)
	bool UpdateFriendLoginTime(FString UserID,FDateTime LoginTime);

};
