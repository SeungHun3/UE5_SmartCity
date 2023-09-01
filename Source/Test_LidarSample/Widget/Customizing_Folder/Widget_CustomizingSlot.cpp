// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_CustomizingSlot.h"
#include "Components/Button.h"
#include "Widget_CustomizingTab.h"
#include "Widget_Customizing.h"

#include "../../Pawn_Player.h"

#include "../../GameModeBase_Solaseado.h"
#include "../../PlayerState_Solaseado.h"

//#include "Kismet/KismetArrayLibrary.h"



int UWidget_CustomizingSlot::GetSlotNumber()
{
	// �θ��������� �� ���Ե��� ��ȣ�� �Ű�
	// ������ ���̺��� ��(�ε�����)���� �����ϱ� ���� �뵵
	return 	GetParent()->GetChildIndex(this);
}

void UWidget_CustomizingSlot::Init_Slot_Implementation(APawn_Player* MyCostumePawn, const TArray<class UDataTable*>& MyDataTable, UWidget_CustomizingTab* CustomizingTab)
{
	//GM�� �ִ� PlayerState�� �����ͼ� ������ ������
	AGameModeBase_Solaseado* GM_Solaseado = Cast<AGameModeBase_Solaseado>(GetWorld()->GetAuthGameMode());
	PS = GM_Solaseado->get_PlayerState_Solaseado();

	// ��������
	SlotCount = GetSlotNumber();
	CostumePawn = MyCostumePawn;
	DataTable = MyDataTable;
	Widget_CustomizingTab = CustomizingTab;
	// Ŀ���͸���¡ �� ������ ��������Ʈ �Լ��� ���ε��ϱ�
	Widget_CustomizingTab->Change_Slot_Delegate.AddDynamic(this, &UWidget_CustomizingSlot::Change_Slot);
	// Ŭ���̺�Ʈ ���ε�
	Slot_BTN->OnClicked.AddDynamic(this, &UWidget_CustomizingSlot::ClickEvent);

}
// TabNumber 0 = Gender / 1~4 = Ŀ���͸���¡
void UWidget_CustomizingSlot::Change_Slot_Implementation(int TabNumber)
{
	if (TabNumber != 0)
	{
		//TabNumber�� ���� ���������̺� �迭 ����-> Change Image
		FName rowName = FName(*FString::FromInt(SlotCount+1));
		Customizing_Struct = *DataTable[TabNumber - 1]->FindRow<FCustomizing_Struct>(rowName, "");

		//�����ߴ� �������� �˻� ��
		//�������Ʈ���� �̹��� �ٲ� �� �ְ� BP �۾����� �ѱ�
		int itemID = FCString::Atoi(*Customizing_Struct.ItemID);
		if (PS->Widget_Customizing->ITemIDs.IsValidIndex(PS->Widget_Customizing->TabNumber - 1))
		{
			if (PS->Widget_Customizing->ITemIDs[PS->Widget_Customizing->TabNumber - 1] == itemID)
			{
				Change_Image(Customizing_Struct.Image, true);
			}
			else{ Change_Image(Customizing_Struct.Image, false); }
		}
		else { Change_Image(Customizing_Struct.Image, false); }
	}
}

void UWidget_CustomizingSlot::ClickEvent()
{	
	//�ε��� ������ �߰� //TabNumber 1~4
	if (!PS->Widget_Customizing->ITemIDs.IsValidIndex(PS->Widget_Customizing->TabNumber - 1))
	{
		// ������ư ��Ȱ��ȭ�Ǿ��־� ���������� ����
		PS->Widget_Customizing->ITemIDs.Add(FCString::Atoi(*Customizing_Struct.ItemID));
		// ������ư ���ֱ�
		PS->Widget_Customizing->Button_Next->SetIsEnabled(true);
	}
	// ������ �ٲٱ�
	else  {	PS->Widget_Customizing->ITemIDs[PS->Widget_Customizing->TabNumber - 1] = FCString::Atoi(*Customizing_Struct.ItemID);	}


	// ���� �ִ� �޽��� ����ü�� ������ ä���־��ش�
	CostumePawn->ChangeMesh(Customizing_Struct.ClassName, Customizing_Struct.Mesh);


	//Ŭ���� ���� ǥ��
	Widget_CustomizingTab->Change_Slot_Broadcast(PS->Widget_Customizing->TabNumber);


}

