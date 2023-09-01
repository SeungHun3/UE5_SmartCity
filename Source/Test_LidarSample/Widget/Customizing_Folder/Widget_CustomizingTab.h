// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Widget_CustomizingTab.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChange_Slot, int, TabNumber);

/**
 * 
 */
UCLASS()
class TEST_LIDARSAMPLE_API UWidget_CustomizingTab : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadWrite)
		class UHorizontalBox* Slot_Container;
	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadWrite)
		class UTextBlock* Explain_TEXT;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray <class UDataTable*> DataTable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class APawn_Player* Costume_Pawn;


	// change Slot Dispatcher
	UPROPERTY(BlueprintAssignable)
		FChange_Slot Change_Slot_Delegate;
	// ���ε��� �̺�Ʈ�� ȣ���ϱ�
	UFUNCTION(BlueprintCallable)
		void Change_Slot_Broadcast(int TabNumber);

	// �����Ҷ� ������ ���Ե� ���ε�ó��
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
		bool Begin_Tab_Bind_Slot(int TabNumber) ;
	virtual bool Begin_Tab_Bind_Slot_Implementation(int TabNumber);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Gender_Visible(bool Is_Gender);

};
