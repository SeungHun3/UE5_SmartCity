// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_Community_Slot.h"
#include "Widget_Friend_Slot.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class FriendTimeType : uint8
{
	Years = 0,
	Months,
	Days,
	Hours,
	Minute,		// 10sec
	Second,	// infinite
	Max,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_DeleteFriend, const FFriendStruct&, InputInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_BlockFriend, const FFriendStruct&, InputInfo);
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_Friend_Slot : public UWidget_Community_Slot
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Logout;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Delete;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Block;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Delete;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Block;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_DeleteFriend Dispatcher_DeleteFriend;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_BlockFriend Dispatcher_BlockFriend;

public:
	UFUNCTION(BlueprintCallable)
	void SetLogoutTimeToText(FTimespan InputTimeSpan);

public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void Click_Block();
	UFUNCTION()
	void Click_Delete();
};
