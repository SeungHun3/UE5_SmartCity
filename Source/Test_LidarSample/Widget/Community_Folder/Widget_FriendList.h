// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_FriendList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDispatcher_OpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDispatcher_OpenBlock);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_CompleteFriendlist, const int&, InputNum);

UCLASS()
class TEST_LIDARSAMPLE_API UWidget_FriendList : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UScrollBox* ScrollBox_FriendList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Request;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Block;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Button_Request;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Button_Block;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_BackPannel;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_OpenRequest Dispatcher_OpenRequest;
	UPROPERTY(BlueprintAssignable)
	FDispatcher_OpenBlock Dispatcher_OpenBlock;
	UPROPERTY(BlueprintAssignable)
	FDispatcher_CompleteFriendlist Dispatcher_CompleteFriendlist;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWidget_Friend_Slot> SpawnSlot;
public:
	UFUNCTION(BlueprintCallable)
	FTimespan GetElapsedTime(FDateTime LogoutTime); // http 요청에 사용할 함수
	
	void ShowLoginTIme();

public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void Click_OpenRequest();
	UFUNCTION()
	void Click_OpenBlock();

	UFUNCTION(BlueprintCallable)
	void UpdateFriendList();
	UFUNCTION(BlueprintCallable)
	void OpenWBFriendList();
	UFUNCTION(BlueprintCallable)
	void CompleteFriendList();
	UFUNCTION(BlueprintCallable)
	void SetTotalFriendNum();
	UFUNCTION(BlueprintCallable)
	void CloseWBFriendList();
	UFUNCTION(BlueprintCallable)
	void DeleteFirend(const FFriendStruct& PlayfabInfo);
	UFUNCTION(BlueprintCallable)
	void ChangeFriendToBlock(const FFriendStruct& PlayfabInfo);

};
