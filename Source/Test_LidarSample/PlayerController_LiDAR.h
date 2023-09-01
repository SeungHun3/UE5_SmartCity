// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_LiDAR.generated.h"

/**
 * 
 */
UCLASS()
class TEST_LIDARSAMPLE_API APlayerController_LiDAR : public APlayerController
{
	GENERATED_BODY()
	
public:
	FVector moveForard;
	FVector moveRight;

	// �ӵ� m/s
	UPROPERTY(EditAnywhere)
		float AutoMoveSpeed = 120.f;

protected:
	// ��ǲ 
	UFUNCTION()
		void moveForwardEvent(float axis);
	UFUNCTION()
		void moveRightEvent(float axis);

	// �ڵ��̵�
	UFUNCTION()
		void moveLocation();
	
	FVector CurrentLoc;
	FVector TargetLoc;

	bool bAutoMoveChecker;

	// ĳ���� ����
	class ACharacter_Player* Player;
	// ���� ��� 
	class ATest_LidarSampleGameModeBase* gm;

	// �ڵ��̵� ��Ʈ�� �ڵ鷯
	FTimerHandle AutomoveHandle;
public:

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
		void AutoMove(FVector Loc);
	UFUNCTION(BlueprintCallable)
		void StopAutoMove();

	UFUNCTION(BlueprintImplementableEvent)
		void AutoMoveGoalEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void AutoMoveCameraRotation(FRotator rot);
};
