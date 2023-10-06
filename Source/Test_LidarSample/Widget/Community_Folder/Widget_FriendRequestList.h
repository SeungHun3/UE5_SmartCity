// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\Struct_Customizing.h"
#include "Blueprint/UserWidget.h"
#include "Widget_FriendRequestList.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDispatcher_OffRequest);

UCLASS()
class TEST_LIDARSAMPLE_API UWidget_FriendRequestList : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_BackGround;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* ScrollBox_FriendList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Close;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Close;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_OffRequest Dispatcher_OffRequest;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWidget_FriendRequestList_Slot> SpawnSlot;
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Click_OffRequest();

	UFUNCTION(BlueprintCallable)
	void UpdateRequestList();
	UFUNCTION(BlueprintCallable)
	void AgreeFriend(const FFriendStruct& PlayfabInfo);
	UFUNCTION(BlueprintCallable)
	void DeleteFriend(const FFriendStruct& TitleID);
};
