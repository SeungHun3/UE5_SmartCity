// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_NpcMark.generated.h"

/**
 * 
 */
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_NpcMark : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//Npc �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextBlock_NpcName;
	//ȥ�㸻 �ؽ�Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextBlock_Soliloquy;
	//ȥ�㸻 �ڽ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* SizeBox_Soliloquy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ExclamationMark;
	//Ÿ���� ȿ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MyText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Writing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartTimeStamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumCharsToDisplay;
public:
	UFUNCTION(BlueprintCallable)
		void WriteSoliloquy(const FString text);
	UFUNCTION(BlueprintCallable)
		void WriteSoliloquyText();
	UFUNCTION(BlueprintCallable)
		void SetVisibleSoliloquyBox(bool visible);
	UFUNCTION(BlueprintCallable)
		void SetNpcName(const FText NpcName);
	UFUNCTION(BlueprintCallable)
		void SetSoliloquyText(const FText Soliloquy);
	UFUNCTION(BlueprintCallable)
		void ExclamationMarkVisibility(ESlateVisibility Visible, FLinearColor MarkColor);
};