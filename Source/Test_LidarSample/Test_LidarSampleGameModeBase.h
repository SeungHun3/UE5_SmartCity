// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"

#include "Test_LidarSampleGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FLiDARQuizData : public FTableRowBase
{
	GENERATED_BODY()

public:
	//- ����
	UPROPERTY(BlueprintReadWrite)
		FString Key;
	//- ��
	UPROPERTY(BlueprintReadWrite)
		FString Value1;
	//- ����
	UPROPERTY(BlueprintReadWrite)
		FString Value2;
};

UCLASS()
class TEST_LIDARSAMPLE_API ATest_LidarSampleGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//UPROPERTY(BlueprintReadWrite)
	//	class AActor_Photon* PhotonClient;

protected:
	ATest_LidarSampleGameModeBase();

	UPROPERTY()
		class UDataTable* LiDARQuizData;

public:
	virtual void StartPlay() override;

	FLiDARQuizData* getLiDARQuiz(const FString& row);

	// ���� �ִϸ��̼� ���� ���� �̺�Ʈ
	void changeAnim(uint8 anim);

	// �ȵ���̵� ��ġ ���� Ȱ��ȭ
	UFUNCTION(BlueprintCallable)
		void CheckLocationEnabled();
	UFUNCTION(BlueprintImplementableEvent)
		void CheckLocationEnabledEvent();
	//// ��Ʈ���� ���� �ε� 
	//void LoadStreamLevel(const FString& Level);
};
