// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Field_Slot.h"
#include "../../GameModeBase_Solaseado.h"
#include "../../Actor_PhotonVoice.h"
#include "../../Actor_SolaseadoPhoton.h"
#include"..\..\Pawn_Player.h"
#include "..\..\ActorComponent_PlayfabStore.h"



//차단하기 클릭
void UWidget_Field_Slot::Click_Block()
{
	Button_Block->SetVisibility(ESlateVisibility::Collapsed);
	Button_FriendRequest->SetVisibility(ESlateVisibility::Collapsed);
	Dispatcher_AddBlock.Broadcast(FriendInfo);
}

//친구 신청 버튼 클릭
void UWidget_Field_Slot::Click_FriendRequest()
{
	Button_Block->SetVisibility(ESlateVisibility::Collapsed);
	Button_FriendRequest->SetVisibility(ESlateVisibility::Collapsed);

	UWorld* World = GetWorld();
	APawn_Player* Player = Cast<APawn_Player>(World->GetFirstPlayerController()->GetPawn());
	class AGameModeBase_Solaseado* gamemode = Cast<AGameModeBase_Solaseado>(GetWorld()->GetAuthGameMode());

	if (Player && gamemode)
	{
		Player->BP_ActorComponent_Playfab;
		for (auto it : gamemode->PhotonCloud->PlayerList)
		{
			if ((FString)it->PlayerName == (FString)FriendInfo.TitleID)
			{
				gamemode->PhotonCloud->SendFriendInfo(it->PlayerNr, Player->BP_ActorComponent_Playfab->PlayFabID);
			}
		}
	}
}

//볼륨값 조절하기
void UWidget_Field_Slot::Change_Value(float value)
{
	class AGameModeBase_Solaseado* gamemode = Cast<AGameModeBase_Solaseado>(GetWorld()->GetAuthGameMode());
	if(ProgressBar_Volume)
	{
		ProgressBar_Volume->SetPercent(value);
	}
	gamemode->PhotonVoice->SetSoundVolume(FriendInfo.TitleID, value);
}

//초기 볼륨값 세팅
void UWidget_Field_Slot::InitSoundSetting(float volume, bool mute)
{
	if (ProgressBar_Volume && Slider_Volume)
	{
		ProgressBar_Volume->SetPercent(volume);
		Slider_Volume->SetValue(volume);
	}
}


void UWidget_Field_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Block->OnClicked.AddDynamic(this, &UWidget_Field_Slot::Click_Block);
	Button_FriendRequest->OnClicked.AddDynamic(this, &UWidget_Field_Slot::Click_FriendRequest);
	Slider_Volume->OnValueChanged.AddDynamic(this, &UWidget_Field_Slot::Change_Value);
}

