// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_CheckingAttandance_Main.h"
#include "Widget_CheckingAttandance_Slot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "../../GameInstance_Solaseado.h"
#include "../../Pawn_Player.h"
#include "../../ActorComponent_PlayfabStore.h"


UWidget_CheckingAttandance_Main::UWidget_CheckingAttandance_Main(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/*
	//���Լ���(�������Ʈ)
	static ConstructorHelpers::FClassFinder<UWidget_CheckingAttandance_Slot> SlotWidget(TEXT("/Game/Project/Widget/Checking/WB_CheckingAttandance_Slot.WB_CheckingAttandance_Slot_C"));
	if (SlotWidget.Succeeded())
	{
		SlotClass = SlotWidget.Class;
	}*/
}
void UWidget_CheckingAttandance_Main::NativeConstruct()
{
	Get_Reward_BTN->OnClicked.AddDynamic(this, &UWidget_CheckingAttandance_Main::PressReward);
	Button_SpecialReward->OnClicked.AddDynamic(this, &UWidget_CheckingAttandance_Main::PressSpecial);
	Button_SpecialReward->SetIsEnabled(false); // 20�� �����ư ����

	Begin_Bind_Setting();

	Super::NativeConstruct();
}

void UWidget_CheckingAttandance_Main::Begin_Bind_Setting()
{
	Main_gridPanel->ClearChildren();
	SlotArray.Empty();
	const int FixView = 4; // �������� ���� ����

	//Reward ���̺� ����
	UGameInstance_Solaseado* MyInstance = Cast<UGameInstance_Solaseado>(GetGameInstance());
	class UDataTable* Reward_Table = MyInstance->GetCheckingRewardTables();
	// ���� ����
	TableLength = Reward_Table->GetRowNames().Num();
	MyPlayer = Cast<APawn_Player>(GetOwningPlayerPawn());
	IsTodayChecked = MyPlayer->BP_ActorComponent_Playfab->Is_Today_Checked();

	// �⼮�� �� Ȯ�� �Ŀ�
	Checking_Count = MyPlayer->BP_ActorComponent_Playfab->Get_Checking_Count();

	if (Checking_Count >= TableLength) // ��ü������ �� �޾Ҵٸ�
	{
		Checking_Count = TableLength;
		IsTodayChecked = true;
	}

	// �ѱ��̸�ŭ Slot���� �� �⼮�Ͽ� ���� Array�� �����
	for (int i = 0; i < TableLength; i++)
	{
		//��������
		UWidget_CheckingAttandance_Slot* SlotWidget = CreateWidget<UWidget_CheckingAttandance_Slot>(GetWorld(), SlotClass);

		// Slot��  �⼮ �˻�
		(i < Checking_Count) ? SlotWidget->Set_Slot(i,true): SlotWidget->Set_Slot(i, false);
		SlotArray.Add(SlotWidget); // �迭�� ����ְ�		
	}

	// �⼮���ο� ���� ����ޱ� ��ư Ȱ��ȭ
	Get_Reward_BTN->SetIsEnabled(!IsTodayChecked);
	// ���ñ��� �⼮ �����ϼ�
	if (IsTodayChecked)
	{
		TodayCount(Checking_Count);
	}
	else
	{
		Checking_Count += 1;
		TodayCount(Checking_Count);
		//���� ������ ���� �ʾҴٸ�
		//���ó�¥ ���� ����
		FName CountSTR(*FString::FromInt(Checking_Count));
		TodayReward = *Reward_Table->FindRow<FChecking_Reward>(CountSTR, "");
		SlotArray[Checking_Count-1]->Today_Slot();
	}
	

	//���ñ��� FixView ������ �����ְ���
	int ViewCount = static_cast<int>((Checking_Count - 1) / FixView);
	for (int i = 0; i < FixView; i++)
	{
		int count = ViewCount * FixView + i;
		Main_gridPanel->AddChildToUniformGrid(SlotArray[count], 0, i);
	}
	
}

void UWidget_CheckingAttandance_Main::PressReward()
{	// Ŭ���� ��ư �����ְ�
	Get_Reward_BTN->SetIsEnabled(false);
	// ������ ���� üũ�ߴٴ� ǥ��� ���� ������Ʈ
	MyPlayer->BP_ActorComponent_Playfab->Update_Check_Change(this,TodayReward.Gold);

}

void UWidget_CheckingAttandance_Main::PressSpecial()
{
	Button_SpecialReward->SetIsEnabled(false);

	//Special Reward ���̺� ����
	UGameInstance_Solaseado* MyInstance = Cast<UGameInstance_Solaseado>(GetGameInstance());
	class UDataTable* Special_Table = MyInstance->GetSpecialRewardTables();

	if (Special_Table->FindRow<FChecking_Reward>("Daily_20th_Reward", ""))
	{
		//Ư������ ��û
		FChecking_Reward Special_Reward = *Special_Table->FindRow<FChecking_Reward>("Daily_20th_Reward", "");
		MyPlayer->BP_ActorComponent_Playfab->Daily_20th_Reward(this, Special_Reward.Gold);
	}

}

void UWidget_CheckingAttandance_Main::ChangeSlot()
{
	// ==>PressReward()
	if (Checking_Count-1<TableLength)
	{
		SlotArray[Checking_Count-1]->Set_Slot(Checking_Count-1,true);
		MyPlayer->BP_ActorComponent_Playfab->UpdateCoin();
	}
	if (Checking_Count == TableLength) // 20�� ��ä���
	{
		Button_SpecialReward->SetIsEnabled(true); //��ư ����
	}
}

void UWidget_CheckingAttandance_Main::ChangeSpecial_Implementation()
{
	MyPlayer->BP_ActorComponent_Playfab->UpdateCoin();
}


//������
void UWidget_CheckingAttandance_Main::Debug_ClearAttandance()
{
	MyPlayer->BP_ActorComponent_Playfab->Debug_Clear_Attandance(this);
}

void UWidget_CheckingAttandance_Main::Debug_ClearCheck()
{
	MyPlayer->BP_ActorComponent_Playfab->Debug_Clear_Daily(this);

}
//������� �����
