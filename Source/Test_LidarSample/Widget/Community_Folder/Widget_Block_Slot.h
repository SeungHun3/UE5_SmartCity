// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_Community_Slot.h"
#include "Widget_Block_Slot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_RemoveBlock, const FFriendStruct&, InputInfo);
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_Block_Slot : public UWidget_Community_Slot
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* TextBlock_Button;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Unlocked;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_RemoveBlock Dispatcher_RemoveBlock;
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void Click_Unlock();
	
};
