// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_BlockList.h"
#include "Widget_Block_Slot.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "../../Pawn_Player.h"
#include "..\..\ActorComponent_PlayfabStore.h"

void UWidget_BlockList::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Close->OnClicked.AddUniqueDynamic(this, &UWidget_BlockList::Click_Close);
}

//차단목록 갱신
void UWidget_BlockList::UpdateBlockList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		ScrollBox_BlockList->ClearChildren();

		for (auto it : Player->BP_ActorComponent_Playfab->BlockList)
		{
			UWidget_Block_Slot* SlotWidget = CreateWidget<UWidget_Block_Slot>(World, SpawnSlot);
			ScrollBox_BlockList->AddChild(SlotWidget);
			SlotWidget->SetTextTitleID(FText::FromString(it.Value.TitleID));
			SlotWidget->FriendInfo = it.Value;
			SlotWidget->Dispatcher_RemoveBlock.AddDynamic(this, &UWidget_BlockList::RemovePlayfabBlock);
		}
	}

}

//차단창 끄기
void UWidget_BlockList::Click_Close()
{
	Dispatcher_OffBlockList.Broadcast();
}

//PlayFab의 차단 리스트에서 해제해준다.
void UWidget_BlockList::RemovePlayfabBlock(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		//플레이팹에서 차단을 풀어준다.
		Player->BP_ActorComponent_Playfab->ScriptCustom("RemoveFriendList", "targetPlayer",PlayfabInfo.PlayFabID);
		RemoveBlock(PlayfabInfo);
	}
}

//클라이언트의 차단 리스트에서 해제해준다.
void UWidget_BlockList::RemoveBlock(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->BlockList.Remove(PlayfabInfo.TitleID);
		UpdateBlockList();
	}
}
