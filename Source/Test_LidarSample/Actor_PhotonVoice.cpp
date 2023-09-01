// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_PhotonVoice.h"
#include "Photon/SH_PhotonVoiceListener.h"
#include "GameFramework/Actor.h"
#include "Photon/Console.h"
#include "Actor_PhotonAudioIn.h"
#include "Actor_PhotonAudioOut.h"
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
	srand(GETTIMEMS());
	mpPhotonLib = new SH_PhotonVoiceListener(ExitGames::Common::JString(TCHAR_TO_UTF8(*AppID)), ExitGames::Common::JString(TCHAR_TO_UTF8(*appVersion)), this, this,this);
}


void AActor_PhotonVoice::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);
	delete mpPhotonLib;
}

// ����ũ ĸó, �ν�
AActor_PhotonAudioIn* AActor_PhotonVoice::createAudioIn()
{
	return mPhotonAudioIn=GetWorld()->SpawnActor<AActor_PhotonAudioIn>();
}
void AActor_PhotonVoice::destroyAudioIn(AActor_PhotonAudioIn* a)
{
	GetWorld()->DestroyActor(a);
}

// ���̽� ê ��� 
AActor_PhotonAudioOut* AActor_PhotonVoice::createAudioOut()
{
	mPhotonAudioOut.Add(GetWorld()->SpawnActor<AActor_PhotonAudioOut>());
	return mPhotonAudioOut[mPhotonAudioOut.Num()-1];
}

void AActor_PhotonVoice::destroyAudioOut(AActor_PhotonAudioOut* a)
{
	mPhotonAudioOut.Remove(a);
	GetWorld()->DestroyActor(a);
}

// ���� ���̽� ������Ʈ
void AActor_PhotonVoice::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (mpPhotonLib)
	{
		mpPhotonLib->update();
	}
}

// ���� ���̽� ����. Playfab ID �����ϱ�.
void AActor_PhotonVoice::Connect(const FString& Vociename)
{
	Console::get().writeLine(L"// Connecting...");
	mpPhotonLib->connect(TCHAR_TO_UTF8(*Vociename));
}
void AActor_PhotonVoice::Disconnect(void)
{
	mpPhotonLib->disconnect();
}

//�� ���带 �������� ��۷� ��ȯ�Ѵ�.
void AActor_PhotonVoice::ToggleEcho(void)
{
	mpPhotonLib->toggleEcho();
}

//���濡 ������ �Ϸ������ ȣ�����ش�.
void AActor_PhotonVoice::Voice_ConnectComplete(void)
{
	ConnectComplete();
}

//���� �Էº� ���Ұ�
void AActor_PhotonVoice::SetMuteIn(bool bInput)
{
	if (mPhotonAudioIn)
	{
		mPhotonAudioIn->mute = bInput;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("//couldn't find the PhotonAudioIn."));
	}
}

//���� ��º� ���Ұ�
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

//���� Ȱ��ȭ/��Ȱ��ȭ
void AActor_PhotonVoice::SetMute(bool bInput)
{
	if (mpPhotonLib->isconnected)
	{
		SetMuteIn(bInput);
		SetMuteOut(bInput);
	}
}

//���� Ȱ��ȭ/��Ȱ��ȭ
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

void AActor_PhotonVoice::Voice_ChangeOrJoinRoom(const FString& RoomFullName)
{
	mpPhotonLib->SetRoomName(RoomFullName);
	if (getIsChanging()) // change���̶�� leaveRoom
	{
		mpPhotonLib->mLoadBalancingClient.opLeaveRoom();
	}
	else
	{
		mpPhotonLib->JoinRoom();
	}
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
