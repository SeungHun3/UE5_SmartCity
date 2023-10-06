// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\Struct_Customizing.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Community_Slot.generated.h"

/**
 * 
 */
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_Community_Slot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_TitleID;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_BackPannel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_URL;
	
	UPROPERTY(BlueprintReadWrite)
	FFriendStruct FriendInfo;
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetTextTitleID(const FText &TitleID);
};
