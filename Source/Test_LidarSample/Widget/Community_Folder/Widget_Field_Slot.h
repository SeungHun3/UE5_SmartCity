// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Widget_Community_Slot.h"
#include "Widget_Field_Slot.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_AddBlock, const FFriendStruct&, InputInfo);
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_Field_Slot : public UWidget_Community_Slot
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_FriendRequest;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Block;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_FriendRequest;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Block;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* Slider_Volume;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ProgressBar_Volume;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_AddBlock Dispatcher_AddBlock;
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Click_FriendRequest();
	UFUNCTION()
	void Click_Block();
	UFUNCTION()
	void Change_Value(float value);

	UFUNCTION(BlueprintCallable)
	void InitSoundSetting(float volume, bool mute);
};
