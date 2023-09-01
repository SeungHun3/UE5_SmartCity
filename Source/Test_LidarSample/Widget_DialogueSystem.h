// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Struct_Customizing.h"
#include "Widget_DialogueSystem.generated.h"

/**
 * 
 */
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_DialogueSystem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//NPC ��ȭ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextBlock_Message;
	//NPC ��ȭ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextBlock_Title;
	//1Key
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UOverlay* Overlay_1Key;
	//2Key
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UOverlay* Overlay_2Key;
	//1Key Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_Next;
	//2Key Button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_First;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Button_FirstText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_Second;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Button_SecondText;
	//��ȭ ��ŵ ��ư
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_Skip;
	//���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentPage = 1;
	//���� ��ȭ�� �Ѿ �� �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bOnNext = true;
	//����Ʈ ��ȭ ������ �ƴ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsQuestDialogue = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* QuestTable;
	//����Ʈ ��ư
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_Accept;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Button_Drop;
public:
	//Ÿ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Writing = false;
	//Ÿ���� ȿ�� �ؽ�Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MyText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartTimestamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumCharsToDisplay;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FText> MessageList;

public:
	virtual void NativeConstruct();
	//1Key ��ư
	UFUNCTION()
		void OnNextButtonClicked();
	//2Key ��ư
	UFUNCTION()
		void OnFirstButtonClicked();
	UFUNCTION()
		void OnSecondButtonClicked();
	UFUNCTION(BlueprintCallable)
		virtual void RemoveMessageBox();
	UFUNCTION(BlueprintCallable)
		void PageReset();
	UFUNCTION(BlueprintCallable)
		virtual void AddMessageBox(enum_DialogueStyle style, const FText title, const FText& Message, const FText& FirstBtnText, const FText& SecondBtnText);
	UFUNCTION(BlueprintImplementableEvent)
		void NextMessage();
	UFUNCTION(BlueprintCallable)
		void SetAllButton(bool IsEnabled);
	UFUNCTION(BlueprintCallable)
		void Write(const FString text);
	UFUNCTION(BlueprintCallable)
		void WriteText();
};
