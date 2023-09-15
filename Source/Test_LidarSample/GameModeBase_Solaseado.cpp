// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase_Solaseado.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "PlayerState_Solaseado.h"
#include "Actor_HTTPRequest.h"

void AGameModeBase_Solaseado::BeginPlay()
{
	Super::BeginPlay();

}

void AGameModeBase_Solaseado::OpenStreamLevel(enum_Level level)
{
	// �߰� ���� ���� // 
	FLevel CheckLevel;
	CheckLevel.level = level;
	FLatentActionInfo latent;
	UGameplayStatics::LoadStreamLevel(this, FName(CheckLevel.getLevelName()), true, true, latent);
	StreamLevelFinish.Broadcast(CheckLevel.getLevelName());
}

void AGameModeBase_Solaseado::CloseStreamLevel(enum_Level level)
{
	FLevel CheckLevel;
	CheckLevel.level = level;
	FLatentActionInfo latent;
	UGameplayStatics::UnloadStreamLevel(this, FName(CheckLevel.getLevelName()), latent, false);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Load Streaming Level Binding Event
void AGameModeBase_Solaseado::LoadStreamEvent()
{
	FLatentActionInfo latent;
	latent.CallbackTarget = this;
	latent.UUID = GetUniqueID();
	latent.Linkage = 0;
	latent.ExecutionFunction = "LoadStreamLevelFinish";

	UGameplayStatics::LoadStreamLevel(this, FName(OpenLevel), true, true, latent);
}

void AGameModeBase_Solaseado::LoadStreamLevelFinish()
{
	CurrentLevel = OpenLevel;
	// ����Ʈ ȭ�� ��ȯ.
	FadeOut();
	// ���� �̵��� ���� ��ġ �̵� , ���� ��ȯ �� �߰� �۾� ����.
	StreamLevelFinish.Broadcast(FOpenLevel.getLevelName());
}
// ���̵� �� & �ƿ�
// ���� �ε�� ȭ�� ��Ʈ�� 
void AGameModeBase_Solaseado::StartFadeOut(float From, float To, float Duration)
{
	bool TempFade;
	From >= 1.f ? TempFade = true : TempFade = false;
	if (CheckFade != TempFade)
	{
		CheckFade = TempFade;
		UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(From, To, Duration, FLinearColor(0.f, 0.f, 0.f, 1.f), false, true);
	}
}
// ���̵� ��
void AGameModeBase_Solaseado::FadeIn()
{
	if (FadeOn)
		StartFadeOut(0.f, 1.f, FadeDuration);
}
// ���̵� �ƿ�
void AGameModeBase_Solaseado::FadeOut()
{
	if (FadeOn)
		StartFadeOut(1.f, 0.f, FadeDuration);
}
void AGameModeBase_Solaseado::CreatePopup()
{
	WidgetCreate_Popup();
	InsufficientFunds_Popup();
}
//////////////////////////////////////////////////////////////////////////////////////////
// ���� �̵� �Լ�
// BlueprintCallable Func
void AGameModeBase_Solaseado::MoveLevel(enum_Level level, bool bFade)
{
	FOpenLevel.level = level;
	OpenLevel = FOpenLevel.getLevelName();
	// Fade in&out ��� 
	FadeOn = bFade;
	// ȭ�� ��ȯ. Duration time
	FadeIn();

	GetWorld()->GetTimerManager().SetTimer(FadeTimer, this, &AGameModeBase_Solaseado::LoadLevel, 0.01f, false, FadeDuration);
}
// ���� ���� �̵� 
void AGameModeBase_Solaseado::FastMoveLevel(enum_Level level)
{
	FOpenLevel.level = level;
	OpenLevel = FOpenLevel.getLevelName();
	ChangeLevel(OpenLevel);
}
// 
void AGameModeBase_Solaseado::LoadLevel()
{
	ChangeLevel(OpenLevel);
}

void AGameModeBase_Solaseado::ChangeLevel(const FString& openLevel, bool bFade)
{
	FLatentActionInfo latent;
	latent.CallbackTarget = this;
	latent.UUID = GetUniqueID();
	latent.Linkage = 0;
	OpenLevel = openLevel;

	if (OpenLevel.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("// ChangeLevel String Empty "));
		return;
	}

	// ���� ���µ� ������ ���� �ϴ��� Ȯ���ϱ�.
	// �ε� ���� ��ε� �� ��ǥ ���� �ε� �ϱ�
	if (CurrentLevel != "")
	{
		latent.ExecutionFunction = "LoadStreamEvent";
		UE_LOG(LogTemp, Log, TEXT("// UnloadStreamLevel :: %s "), *CurrentLevel);
		UGameplayStatics::UnloadStreamLevel(this, FName(CurrentLevel), latent, false);
	}
	else
	{
		latent.ExecutionFunction = "LoadStreamLevelFinish";
		UGameplayStatics::LoadStreamLevel(this, FName(OpenLevel), true, true, latent);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
// Seamless Load Level System
// ���� �������� �ε��� �̿� ���� üũ
TArray<FString> AGameModeBase_Solaseado::Check_OutsideLevel(const FString& targetLevel)
{
	TArray<FString> OutSideLevel;
	OutSideLevel.Empty();

	// 1. �̸� �и� SimPoly ���� �ѹ��� ���
	FString str = "SimPoly";
	FString LevelName = targetLevel;
	if (LevelName.IsEmpty())
		return OutSideLevel;

	const TCHAR* str1 = FCString::Strstr(*LevelName, *str);
	FString Level_Num = FString(str1 + str.Len());

	// ���� ���� 1~16 ����. ȯ��Ŀ��� �迭���� 0 ~ 15 ��� . �߰� ��ȯ�� +1 ���� ����.
	int LevelRow = getTargetLevelRow(Level_Num);
	int LevelCol = getTargetLevelCol(Level_Num);

	// �߰� ���  > 8�� �߰�
	bool bUp = false;
	bool bDown = false;
	bool bRight = false;
	bool bLift = false;

	if (LevelRow + 1 < MaxRow)
	{
		// +1 �߰� ��ȯ��.
		OutSideLevel.Add(str + FString::FromInt((LevelRow + 1) * MaxCol + LevelCol + 1));
		bUp = true;
	}
	if (LevelRow - 1 >= 0)
	{
		OutSideLevel.Add(str + FString::FromInt((LevelRow - 1) * MaxCol + LevelCol + 1));
		bDown = true;
	}
	// MaxCol;
	if (LevelCol + 1 < MaxCol)
	{
		OutSideLevel.Add(str + FString::FromInt((LevelRow)*MaxCol + LevelCol + 1 + 1));
		bRight = true;
	}
	if (LevelCol - 1 >= 0)
	{
		OutSideLevel.Add(str + FString::FromInt((LevelRow)*MaxCol + LevelCol - 1 + 1));
		bLift = true;
	}

	//// 8�� �߰�
	//if (bUp)
	//{
	//	if (bRight)
	//	{
	//		OutSideLevel.Add(str + FString::FromInt((LevelRow + 1) * MaxCol + (LevelCol + 1) + 1));
	//	}
	//	if (bLift)
	//	{
	//		OutSideLevel.Add(str + FString::FromInt((LevelRow + 1) * MaxCol + (LevelCol - 1) + 1));
	//	}
	//}
	//if (bDown)
	//{
	//	if (bRight)
	//	{
	//		OutSideLevel.Add(str + FString::FromInt((LevelRow - 1) * MaxCol + (LevelCol + 1) + 1));
	//	}
	//	if (bLift)
	//	{
	//		OutSideLevel.Add(str + FString::FromInt((LevelRow - 1) * MaxCol + (LevelCol - 1) + 1));
	//	}
	//}
	return OutSideLevel;
}

// �ֺ� �̿� ���� ��ε�.
void AGameModeBase_Solaseado::Seamless_UnloadOutsideLevel(const TArray<FString>& unload)
{
	Check_UnloadLevel = unload;
	// OpenLevel ����
	Check_UnloadLevel.Remove(OpenLevel);
	// Check_UnloadLevel ��� ��ü ��ε� �۾�.
	Check_UnloadOutsideLevel();
}
void AGameModeBase_Solaseado::Seamless_LoadOutsideLevel(const TArray<FString>& unload)
{
	Check_LoadLevel = unload;
	// CurrentLevel ����
	Check_LoadLevel.Remove(CurrentLevel);
	// Check_UnloadLevel ��� ��ü ��ε� �۾�.
	Check_LoadOutsideLevel();
}

// seamless ���� ��ε�� OpenLevel�� ��ε� ����.
void AGameModeBase_Solaseado::Check_UnloadOutsideLevel()
{
	// �̿� ���� ��ε� �̸�
	FString Check_OutsideUnloadLevel;
	if (Check_UnloadLevel.Num() > 0)
	{
		Check_OutsideUnloadLevel = Check_UnloadLevel[0];
	}
	else
	{
		// �̿� ���� ��ε� �Ϸ� �� > �ʿ� ���� �ε� �۾� �߰�.
		Seamless_LoadOutsideLevel(Check_OutsideLevel(OpenLevel));
		return;
	}

	FLatentActionInfo latent;
	latent.CallbackTarget = this;
	latent.UUID = GetUniqueID();
	latent.Linkage = 0;

	latent.ExecutionFunction = "Check_UnloadOutsideLevel";
	UGameplayStatics::UnloadStreamLevel(this, FName(Check_OutsideUnloadLevel), latent, false);

	Check_UnloadLevel.Remove(Check_OutsideUnloadLevel);
}
// seamless ���� ��ε�� OpenLevel�� ��ε� ����.
void AGameModeBase_Solaseado::Check_LoadOutsideLevel()
{
	// �̿� ���� ��ε� �̸�
	FString Check_OutsideloadLevel;
	if (Check_LoadLevel.Num() > 0)
	{
		if (Check_LoadLevel.IsValidIndex(0))
			Check_OutsideloadLevel = Check_LoadLevel[0];
	}
	else
	{
		// Finished >> �ʿ� ���� �ε� �Ϸ� �� ó��.
		CurrentLevel = OpenLevel;
		UE_LOG(LogTemp, Log, TEXT("// Test _ Seamless OutsideLevel Check_LoadLevel"));
		return;
	}

	FLatentActionInfo latent;
	latent.CallbackTarget = this;
	latent.UUID = GetUniqueID();
	latent.Linkage = 0;

	latent.ExecutionFunction = "Check_LoadOutsideLevel";
	UGameplayStatics::LoadStreamLevel(this, FName(Check_OutsideloadLevel), true, true, latent);

	Check_LoadLevel.Remove(Check_OutsideloadLevel);
}
//  Seamless Level Load 
void AGameModeBase_Solaseado::SeamlessLevelLoad(const FString& openLevel)
{
	// ���� ���� ���� üũ.
	OpenLevel = openLevel;
	// ��ε� ���� �Ϸ� �� �ֺ� ���� �ε� �۾�.
	// ���� ���� (CurrentLevel) �ش��ϴ� �̿� ���� ó��.
	UE_LOG(LogTemp, Log, TEXT("// SeamlessLevelLoad CurrentLevel( %s ) "), *CurrentLevel);
	Seamless_UnloadOutsideLevel(Check_OutsideLevel(CurrentLevel));
}

APlayerState_Solaseado* AGameModeBase_Solaseado::get_PlayerState_Solaseado()
{
	return 	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerState<APlayerState_Solaseado>();
}
