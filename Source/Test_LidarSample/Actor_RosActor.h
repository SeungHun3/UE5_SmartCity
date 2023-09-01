// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "LiDAR/Lidar_Basic.h"
#include "Actor_RosActor.generated.h"

UCLASS()
class TEST_LIDARSAMPLE_API AActor_RosActor : public AActor, public Lidar_Basic
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_RosActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString connectIP = FString("183.105.113.142");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 connectPort = 9090;

	// ���� �÷��̾� ĳ���� Ŭ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APawn_LidarPlayer> SpawnPlayer;
	
	// ���̴� ���� ĳ����  // ���̴� ���� ������  TArray<FLidarData> ArrayPlayerLidarData;
	TArray<class APawn_LidarPlayer*> ArrayPlayers;
	
	TSharedPtr<FROSBridgeHandler> ROSBridgeHandler;
	TSharedPtr<FROSBridgePublisher> Publisher; 

	// ���̴� ���� �� ID
	TArray<int> CheckUserID;
	TArray<int> CheckRemoveID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
		void CheckTimer();
	float CurrentTimer;
	FTimerHandle CheckTimerHandle;

	void setPlayerDataAverage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ( ���̴� ) ������ ó�� Lidar_Basic
	void RemoveLidarData(int id);

	// ( ���̴� ) ������ ó�� Lidar_Basic
	virtual void updateLidar(FLidarData data) override;

	// ( ���̴� ) ���� ������ ����
	virtual void RemovePlayerData() override;

	// ���� ������ ���� 
	void AddPlayerData(FLidarData data);

	// ���� ������ ����
	void updatePlayerData(int PlayerId);
};
