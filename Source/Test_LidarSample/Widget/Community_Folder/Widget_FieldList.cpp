// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_FieldList.h"
#include "Widget_Field_Slot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include"..\..\Pawn_Player.h"
#include "..\..\ActorComponent_PlayfabStore.h"
#include "../../GameModeBase_Solaseado.h"
#include "../../Actor_PhotonVoice.h"
#include "../../Actor_SolaseadoPhoton.h"

void UWidget_FieldList::NativeConstruct()
{
	Super::NativeConstruct();
	Button_ReFresh->OnClicked.AddUniqueDynamic(this, &UWidget_FieldList::Click_Refresh);
}

//채널에 있는 플레이어 목록을 업데이트 합니다.
void UWidget_FieldList::Click_Refresh()
{
	ScrollBox_FiedlList->ClearChildren();

	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());
	class AGameModeBase_Solaseado* gamemode = Cast<AGameModeBase_Solaseado>(GetWorld()->GetAuthGameMode());

	if (Player && gamemode)
	{
		for (auto it : gamemode->PhotonCloud->PlayerList)
		{
			if (it->PlayerName != Player->PlayerName)
			{
				UWidget_Field_Slot* SlotWidget = CreateWidget<UWidget_Field_Slot>(GetWorld(), SpawnSlot);
				if (SlotWidget)
				{
					ScrollBox_FiedlList->AddChild(SlotWidget);
					SlotWidget->TextBlock_TitleID->SetText(FText::FromString(it->PlayerName));
					SlotWidget->InitSoundSetting(
					gamemode->PhotonVoice->GetSoundVolume(it->PlayerName),
					gamemode->PhotonVoice->GetMute(it->PlayerName));
					SlotWidget->FriendInfo.TitleID = it->PlayerName;
					SlotWidget->Dispatcher_AddBlock.AddDynamic(this, &UWidget_FieldList::AddBlock);
				}
			}
		}
		updateFieldList();
	}

}

//새로고침 : 채널에 있는 플레이어 목록을 업데이트 합니다.
void UWidget_FieldList::Refresh()
{
	Click_Refresh();
}

//친구 목록 갱신
void UWidget_FieldList::updateFieldList()
{

	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());
	
	if (Player)
	{
		Player->BP_ActorComponent_Playfab->UpdateFriend.AddDynamic(this, &UWidget_FieldList::CompleteFreindList);
		Player->BP_ActorComponent_Playfab->getFriendListEvent();
	}
}

//친구 목록 갱신 콜백 -> 채널에 있는 유저 중 친구를 구별한다.
void UWidget_FieldList::CompleteFreindList()
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->UpdateFriend.RemoveDynamic(this, &UWidget_FieldList::CompleteFreindList);
		for (auto it : ScrollBox_FiedlList->GetAllChildren())
		{
			UWidget_Field_Slot* fieldSlot =Cast<UWidget_Field_Slot>(it);
			if (
				Player->BP_ActorComponent_Playfab->FriendList.Find(fieldSlot->FriendInfo.TitleID) == nullptr
				&& Player->BP_ActorComponent_Playfab->BlockList.Find(fieldSlot->FriendInfo.TitleID) == nullptr)
			{
				fieldSlot->Button_Block->SetVisibility(ESlateVisibility::Visible);
				fieldSlot->Button_FriendRequest->SetVisibility(ESlateVisibility::Visible);
			}
		}
		Dispatcher_CompleteFieldList.Broadcast(ScrollBox_FiedlList->GetAllChildren().Num());
	}
}

//채널에 있는 유저를 차단한다.
void UWidget_FieldList::AddBlock(const FFriendStruct& PlayfabInfo)
{
	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());

	if (Player)
	{
		Player->BP_ActorComponent_Playfab->ScriptCustom("AddBlockList", "targetPlayer", PlayfabInfo.TitleID);
	}
}
