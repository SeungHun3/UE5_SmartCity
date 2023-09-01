// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_CustomizingTab.h"
#include "Widget_CustomizingSlot.h"
#include "Components/HorizontalBox.h"
#include "../../Pawn_Player.h"

#include "../../GameInstance_Solaseado.h"

void UWidget_CustomizingTab::Change_Slot_Broadcast(int TabNumber)
{
	Change_Slot_Delegate.Broadcast(TabNumber);
}

bool UWidget_CustomizingTab::Begin_Tab_Bind_Slot_Implementation(int TabNumber)
{
	UGameInstance_Solaseado* MyInstance = Cast<UGameInstance_Solaseado>(GetGameInstance());
	DataTable = MyInstance->GetDataTables();

	
	// �޽� ����Ʈ ó��
	Costume_Pawn->BeginDefalutMesh();
	// scene���� �Լ� ���ε�ó��
	Costume_Pawn->Bind_Init(this);

	// �� ���Ե� init �Լ� ����, ȣ��� ���ε��Լ� ó��
	for (auto CustomSlot : Slot_Container->GetAllChildren())
	{
		if (CustomSlot)
		{
			Cast<UWidget_CustomizingSlot>(CustomSlot)->Init_Slot(Costume_Pawn, DataTable, this);
		}
	}
	Gender_Visible(true);
	//�� ���Ե� ���ε� �� �������� ȣ��
	Change_Slot_Broadcast(TabNumber);

	return true;
}
