// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Http.h>
#include "GameFramework/Actor.h"
#include "Actor_HTTPRequest.generated.h"


UCLASS()
class TEST_LIDARSAMPLE_API AActor_HTTPRequest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_HTTPRequest(const class FObjectInitializer& ObjectInitializer);

	FHttpModule* Http;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HttpCallCount = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SettingFriend(const FString& UserID, const FDateTime& LastLoginTime);

	UFUNCTION(BlueprintCallable)
	void GetUserLogoutTime(FString UserID);

	UFUNCTION(BlueprintCallable)
	void HttpCall(const FString& HttpLink); // http 요청에 사용할 함수

	// http GET 요청 직후 호출되는 콜백함수
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	FString sParsingHTTP(FString ExtractedHttp, FString InputString);

	UFUNCTION(BlueprintCallable)
	bool GetLastLogoutTime(FString ExtractedHttp);

	
};
