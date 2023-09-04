// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct_Customizing.h"
#include "Widget_Option.generated.h"

/**
 * 
 */
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_Option : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
		void SetOptionStyle(enum_OptionStyle style);
	UFUNCTION(BlueprintCallable)
		void InitTabButton();
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateButtonStyle();
	//버튼 클릭
	UFUNCTION()
		void Clicked_Sound();
	UFUNCTION()
		void Clicked_Alarm();
	UFUNCTION()
		void Clicked_Account();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentTabNumber = 0;
	//Overlay
	UPROPERTY(meta = (BindWidget))
		class UOverlay* Overlay_Sound;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* Overlay_Alarm;
	UPROPERTY(meta = (BindWidget))
		class UOverlay* Overlay_Account;
	//Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Tab_BtnSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Tab_BtnAlarm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Tab_BtnAccount;

};
