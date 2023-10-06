// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_Community_Slot.h"
#include "Widget_FriendRequestList_Slot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_AgreeFriend, const FFriendStruct&, InputInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_DisagreeFriend, const FFriendStruct&, InputInfo);

UCLASS()
class TEST_LIDARSAMPLE_API UWidget_FriendRequestList_Slot : public UWidget_Community_Slot
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* TextBlock_Agree;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* TextBlock_Disagree;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Agree;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Disagree;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_AgreeFriend Dispatcher_AgreeFriend;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_DisagreeFriend Dispatcher_DisagreeFriend;
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Click_AgreeButton();
	UFUNCTION()
	void Click_DisagreeButton();
};
