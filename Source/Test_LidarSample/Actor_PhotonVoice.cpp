// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_PhotonVoice.h"
#include "Photon/SH_PhotonVoiceListener.h"
#include "GameFramework/Actor.h"
#include "Photon/Console.h"
#include "Actor_PhotonAudioIn.h"
#include "Actor_PhotonAudioOut.h"
#include "ActorComponent_PlayfabStore.h"
#include "Kismet/GameplayStatics.h"

using namespace ExitGames::Voice;

#define Voice_AppID "4688d25f-b9ff-449a-a2a4-5c081f77fd40"

// bd96a445-11ab-4bc6-bb45-71b681b17273
AActor_PhotonVoice::AActor_PhotonVoice() : 
	serverAddress(TEXT("ns.exitgames.com")), // ns.exitgames.com // ns.photonengine.io
	AppID(TEXT(Voice_AppID)),
	appVersion(TEXT("1.0.1")),
	mpPhotonLib(NULL)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AActor_PhotonVoice::BeginPlay(void)
{
	Super::BeginPlay();
}


void AActor_PhotonVoice::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);
	delete mpPhotonLib;
}

// 마이크 캡처, 인식
AActor_PhotonAudioIn* AActor_PhotonVoice::createAudioIn()
{
	return mPhotonAudioIn=GetWorld()->SpawnActor<AActor_PhotonAudioIn>(getAudioIn,FTransform());
}
void AActor_PhotonVoice::destroyAudioIn(AActor_PhotonAudioIn* a)
{
	
	GetWorld()->DestroyActor(a);
}

// 보이스 챗 출력 
AActor_PhotonAudioOut* AActor_PhotonVoice::createAudioOut()
{
	mPhotonAudioOut.Add(GetWorld()->SpawnActor<AActor_PhotonAudioOut>(getAudioOut, FTransform()));

	return mPhotonAudioOut[mPhotonAudioOut.Num()-1];
}

void AActor_PhotonVoice::destroyAudioOut(AActor_PhotonAudioOut* a)
{
	mPhotonAudioOut.Remove(a);
	GetWorld()->DestroyActor(a);
}

// 포톤 보이스 업데이트
void AActor_PhotonVoice::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (mpPhotonLib)
	{
		mpPhotonLib->update();
	}
}

// 포톤 보이스 접속. Playfab ID 통일하기.
void AActor_PhotonVoice::Connect(const FString& Vociename)
{
	ConnectLogin(Vociename);
}

void AActor_PhotonVoice::ConnectLogin(const FString& username)
{
	Console::get().writeLine(L"// Connecting...");

	srand(GETTIMEMS());
	mpPhotonLib = new SH_PhotonVoiceListener(this, this, this);
	PhotonClient = new ExitGames::LoadBalancing::Client(*mpPhotonLib, TCHAR_TO_UTF8(*AppID), TCHAR_TO_UTF8(*appVersion),
		ExitGames::Photon::ConnectionProtocol::DEFAULT, false, ExitGames::LoadBalancing::RegionSelectionMode::SELECT, false); //  (nByte)0U, false, ExitGames::LoadBalancing::RegionSelectionMode::SELECT
	mpPhotonLib->SetClient(PhotonClient);
	mpPhotonLib->connect(TCHAR_TO_UTF8(*username), TCHAR_TO_UTF8(*serverAddress));
}



void AActor_PhotonVoice::Disconnect(void)
{
	mpPhotonLib->disconnect();
}

//내 사운드를 들을건지 토글로 전환한다.
void AActor_PhotonVoice::ToggleEcho(void)
{
	mpPhotonLib->toggleEcho();
}

//포톤에 연결이 완료됐을때 호출해준다.
void AActor_PhotonVoice::Voice_ConnectComplete(void)
{
	ConnectComplete();

	LocalPlayer = Cast<APawn_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
	LocalPlayer->BP_ActorComponent_Playfab->UpdateFriend.AddDynamic(this, &AActor_PhotonVoice::UpdateFriendVoice);
}

//사운드 입력부 음소거
void AActor_PhotonVoice::SetMuteIn(bool bInput)
{
	if (mPhotonAudioIn)
	{
		mPhotonAudioIn->bMute = bInput;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("//couldn't find the PhotonAudioIn."));
	}
}

//사운드 출력부 음소거
void AActor_PhotonVoice::SetMuteOut(bool bInput)
{
	if (mPhotonAudioOut.Num())
	{
		for (auto it: mPhotonAudioOut)
		{
			it->SetMute(bInput);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("//couldn't find the PhotonAudioOut."));
	}
}

//사운드 활성화/비활성화
void AActor_PhotonVoice::SetMute(bool bInput)
{
	if (mpPhotonLib->isconnected)
	{
		SetMuteIn(bInput);
		SetMuteOut(bInput);
	}
}

//사운드 활성화/비활성화
uint8 AActor_PhotonVoice::GetInMikeVolume()
{
	if (mPhotonAudioIn)
	{
		return (uint8)mPhotonAudioIn->aveMike;
	}
	return 0;
}

bool AActor_PhotonVoice::getIsChanging()
{
	return mpPhotonLib->IsChanging;
}

void AActor_PhotonVoice::setIschanging(bool Change)
{
	mpPhotonLib->IsChanging = Change;
}

//채널 이동
void AActor_PhotonVoice::Voice_ChangeOrJoinRoom(const FString& RoomFullName)
{
	mpPhotonLib->SetRoomName(RoomFullName);
	if (getIsChanging()) // change중이라면 leaveRoom
	{
		mpPhotonLib->mLoadBalancingClient->opLeaveRoom();
	}
	else
	{
		mpPhotonLib->JoinRoom();
	}
}

//PlayFab 친구 갱신시 호출
void AActor_PhotonVoice::UpdateFriendVoice()
{
	Voice_UpdateFriend();
}

//갱신된 차단 리스트에 등록되어 있으면 음소거
//블랙리스트 음소거
void AActor_PhotonVoice::Voice_UpdateFriend()
{
	if (mPhotonAudioOut.Num())
	{
		LocalPlayer = Cast<APawn_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());
		for (auto it : mPhotonAudioOut)
		{
			FString str = it->GetUserID();

			if (LocalPlayer->BP_ActorComponent_Playfab->BlockList.Find(str))
			{
				it->SetMute(true);
				UE_LOG(LogTemp, Log, TEXT("//Voice_UpdateFriend mute On :: %s"), *str);
			}
			else
			{
				it->SetMute(false);
				UE_LOG(LogTemp, Log, TEXT("//Voice_UpdateFriend mute Off :: %s"), *str);
			}
		}
	}
}


//에러 체크 메시지
void AActor_PhotonVoice::ErrorCheckMessage(const FString& message, int error)
{
	UE_LOG(LogTemp, Log, TEXT("// ErrorCheckMessage :: %s"), *message);
	UE_LOG(LogTemp, Log, TEXT("// ErrorCode :: %d"), error);
}

//입력된 유저 음량 입력
bool AActor_PhotonVoice::SetSoundVolume(FString UserID, float volume)
{
	for (auto it : mPhotonAudioOut)
	{
		FString str = it->GetUserID();
		if (str == UserID)
		{
			it->SetVolume(volume);
			return true;
		}
	}
	return false;
}

//입력된 유저의 음량 얻기
float AActor_PhotonVoice::GetSoundVolume(FString UserID)
{
	for (auto it : mPhotonAudioOut)
	{
		FString str = it->GetUserID();
		if (str == UserID)
		{

			UE_LOG(LogTemp, Log, TEXT("//GetSoundVolume uSER :: %s flosdt : " ), *str, it->GetVolume());
			return it->GetVolume();
		}
	}
	return 0.0f;
}

//입력된 유저의 음소거 상태 얻기
bool AActor_PhotonVoice::GetMute(FString UserID)
{
	for (auto it : mPhotonAudioOut)
	{
		FString str = it->GetUserID();
		if (str == UserID)
		{
			return it->GetMuteState();
		}
	}
	return false;
}


#if WITH_EDITOR
void AActor_PhotonVoice::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property) ? e.Property->GetFName() : NAME_None;
	//if(PropertyName == GET_MEMBER_NAME_CHECKED(AActor_PhotonVoice, automove))
	//	SetAutomove(automove);
	//else if(PropertyName == GET_MEMBER_NAME_CHECKED(AActor_PhotonVoice, useGroups))
	//	SetUseGroups(useGroups);
}
#endif
