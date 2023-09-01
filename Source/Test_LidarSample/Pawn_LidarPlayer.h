// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_LidarPlayer.generated.h"

UENUM(BlueprintType)
enum class enum_TypeLiDAR : uint8
{
	Adult = 0,
	Baby,
	Custom,
};

UCLASS()
class TEST_LIDARSAMPLE_API APawn_LidarPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawn_LidarPlayer();

	// ���̴� ĳ���� �νĹ�ȣ
	int PlayerID;

	// ���̴� ĳ���� Ÿ�� 
	enum_TypeLiDAR TypeLiDAR = enum_TypeLiDAR::Custom;

	// ���̴� Ư�� ���� ������ ���� 
	FString ZoneNumber = FString();

	// ĳ���� �̵� �ӵ�
	UPROPERTY(BlueprintReadWrite)
		FVector MoveVelocity;

	// �ȱ�
	UPROPERTY(BlueprintReadWrite)
		bool bWalk;

	// VIsibility
	UPROPERTY(BlueprintReadWrite)
		bool bCheckVisibility = false;

	UPROPERTY(BlueprintReadWrite)
		class ACharacter_Player* player;

	FVector CurrentLoc = FVector::ZeroVector;
	FVector NextLoc = FVector::ZeroVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle LerpLocationHandle;
	FTimerHandle LerpRotationHandle;

	float CurrentRot = 0.f;
	float NextRot = 0.f;

	UFUNCTION()
		void setLocationLerp();
	UFUNCTION()
		void setRotationLerp();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �ӵ� 
	void setActorVelocity(FVector vel);

	// ��ġ �̵� ���� ó��
	UFUNCTION(BlueprintCallable)
		void setActorLocationLerp(FVector loc);	

	void setActorRotationLerp(float fYaw);

	void setPlayerTypeLiDAR(const FString& type);
	void setZoneNum(const FString& zone); // data.zone_num[0]

	UFUNCTION(BlueprintImplementableEvent)
		void setNameTag(const FString& name);
	UFUNCTION(BlueprintImplementableEvent)
		void setCharacterType(enum_TypeLiDAR type);
	UFUNCTION(BlueprintImplementableEvent)
		void setZoneNumber(const FString& num);
	// TAA  opacity controll
	UFUNCTION(BlueprintImplementableEvent)
		void setOpacityVIsibility(bool visible);
};
