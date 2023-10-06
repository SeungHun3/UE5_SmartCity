// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_FieldList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDispatcher_CompleteFieldList, const int&, InputNum);

UCLASS()
class TEST_LIDARSAMPLE_API UWidget_FieldList : public UUserWidget
{
	GENERATED_BODY()
	
		//uslider
		//uprogressbar
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* ScrollBox_FiedlList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextBlock_Button_ReFresh;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_ReFresh;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_CompleteFieldList Dispatcher_CompleteFieldList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWidget_Field_Slot> SpawnSlot;
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Click_Refresh();

	UFUNCTION(BlueprintCallable)
	void Refresh();

	UFUNCTION(BlueprintCallable)
	void updateFieldList();

	UFUNCTION(BlueprintCallable)
	void CompleteFreindList();

	UFUNCTION(BlueprintCallable)
	void AddBlock(const FFriendStruct& PlayfabInfo);
};
