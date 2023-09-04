// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_SettingSlot.generated.h"

/**
 * 
 */
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_SettingSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class USlider* Slider_Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* ProgressBar_Slot;

public:
	UFUNCTION(BlueprintCallable)
		void SetVolume(float Value);
	UFUNCTION(BlueprintCallable)
		void SetProgressPercent(float Input);
};
