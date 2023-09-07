// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Basic.h"

#include "Engine/DataTable.h"
#include "Pawn_NPC.generated.h"

UENUM(BlueprintType)
enum class enum_NPCAnimationStateC : uint8
{
	Error = 0,
	Idle,
	Walk,
	Death,
};

UENUM(BlueprintType)
enum class enum_NPCMultiType : uint8
{
	Single = 0,
	Multi,
};


USTRUCT(Atomic, BlueprintType)
struct FNpcCustomizing_Struct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector NpcLocation = FVector(0.0f, 0.0f,0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NpcName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		enum_NPCMultiType Type = enum_NPCMultiType::Single;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* Boddy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* Hair;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* Top;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* Bottoms;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USkeletalMesh* Shoes;
	
};


UCLASS()
class TEST_LIDARSAMPLE_API APawn_NPC : public APawn_Basic
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawn_NPC();

	// ĳ���� ���̷��� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* NameTagComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* LogComp;
	// ä�� �޼��� ��� ��ǳ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UWidgetComponent* MessageComp;


	// ���� �ִϸ��̼� ����
	UPROPERTY(BlueprintReadWrite)
	enum_NPCAnimationStateC PlayerAnimState;

	// NPC ��Ƽ Ÿ��(�¶������� �ƴ��� Ÿ������ ����)
	UPROPERTY(BlueprintReadWrite)
	enum_NPCMultiType eNPCType = enum_NPCMultiType::Single;

	// ��ȣ�ۿ� ���� ����. false�϶� ��ȣ�ۿ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIntercation = false;

	// �г��� ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNametag = false;

	// ȸ�� �� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotationNPC = false;

	// ȸ�� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NPCTurnSpeed = 120.0f;

	//���� ����Ÿ�̸��� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float 	fMoveTimeNow = 0.0f;

	//���� ���� 1 or -1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float 	fRoationDirection = 1.0f;

	//ĳ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	//ĳ���� ID (�������� �����ϴµ� ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterID;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bCheckOutline = true;


public:	

	UFUNCTION(BlueprintCallable)
		void setOutline();
	UFUNCTION(BlueprintCallable)
		void ClearOutline();

	UFUNCTION(BlueprintCallable)
		void SetName(const FText& Name);

	UFUNCTION(BlueprintCallable)
		void OnNameTag();

	UFUNCTION(BlueprintCallable)
		void SelfDestroy();

	//********��� ������ �Լ� ���***************************//
	// 
	//NPC ��ȣ�ۿ��

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual	void NPCInteraction();

	//��Ƽ���� NPC �̵��Լ�
	UFUNCTION(BlueprintCallable)
	void NPCMoveToLocation(FVector FLoc);

	// Nav�޽ñ�� �̵� �Լ�
	UFUNCTION(BlueprintImplementableEvent)
	void NPCMove(FVector FLoc); 

	//NPC �ʱ�ȭ
	UFUNCTION(BlueprintImplementableEvent)
	void InitNPC(const FString& ID);
};
