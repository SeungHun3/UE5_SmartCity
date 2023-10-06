// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\Struct_Customizing.h"
#include "Blueprint/UserWidget.h"
#include "Widget_BlockList.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDispatcher_OffBlockList);

UCLASS()
class TEST_LIDARSAMPLE_API UWidget_BlockList : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* ScrollBox_BlockList;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_BackGround;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Close;

	UPROPERTY(BlueprintAssignable)
	FDispatcher_OffBlockList Dispatcher_OffBlockList;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class UWidget_Block_Slot> SpawnSlot;
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateBlockList();

	UFUNCTION()
	void Click_Close();
	UFUNCTION(BlueprintCallable)
	void RemovePlayfabBlock(const FFriendStruct& PlayfabInfo);

	UFUNCTION(BlueprintCallable)
	void RemoveBlock(const FFriendStruct& PlayfabInfo);

};
