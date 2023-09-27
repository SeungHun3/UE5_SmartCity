// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/Voice/Public/VoiceModule.h"
#include "PhotonVoice-cpp/inc/AudioUtil.h"
#include "PhotonVoice-cpp/inc/Buffer.h"
#include "PhotonVoice-cpp/inc/AudioOutdelayControl.h"
#include "Actor_PhotonAudioOut.generated.h"

class IAudioOutFactory
{
public:
	virtual ~IAudioOutFactory(void) {}
	virtual class AActor_PhotonAudioOut* createAudioOut(void) = 0;
	virtual void destroyAudioOut(class AActor_PhotonAudioOut*) = 0;
};

// 사운드 출력 추가 클래스 정보
// 사운드 웨이브 설정
UCLASS()
class TEST_LIDARSAMPLE_API UProcedural : public USoundWaveProcedural
{
	GENERATED_BODY()
public:
	UProcedural(const FObjectInitializer& ObjectInitializer);
	//	virtual Audio::EAudioMixerStreamDataFormat::Type GetGeneratedPCMDataFormat() const override;
	void init(int ringBufSize);
	void write(ExitGames::Voice::Buffer<short>& buf, int offset);
	int64 getReadPos(void);
	virtual int32 OnGeneratePCMAudio(TArray < uint8 >& OutAudio, int32 NumSamples) override;


protected:
	ExitGames::Voice::Buffer<short> mRingBuf;
	std::atomic<int64> mReadPos;
};

UCLASS()
class TEST_LIDARSAMPLE_API AActor_PhotonAudioOut : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AActor_PhotonAudioOut();
	void init(const ExitGames::Voice::ILogger& logger, ExitGames::Voice::AudioOutDelayConfig delayConfig, const ExitGames::Common::JString& logPrefix,
		bool debugInfo, int InputPlayerNr, FString InputUserID);
	void finit(void);
	void service(void);
	ExitGames::Voice::IAudioOut<short>* getPlayer(void) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//오디오 컴포넌트 : 이걸로 사운드 관련 대부분 컨트롤하면 된다.
	UAudioComponent* mpAudioComponent;
	// 포톤 보이스 플레이어 정보
	ExitGames::Voice::IAudioOut<short>* mpPlayer;
	
	// 플레이어 정보
	bool bMmute = false;
	float fVolume; // 음소거 대비 볼륨 저장용
	FString UserID; // 포톤 아이디 : 대부분 이걸로 검색
	int PlayerNr;
	////////////////////////////
public:
	//Get/Set 관련 함수들
	void SetMute(bool bInput);
	bool GetMuteState();
	void SetVolume(float volume);
	float GetVolume();
	FString GetUserID()
	{
		return UserID;
	};
	int GetPlayerNr()
	{
		return PlayerNr;
	};
	UFUNCTION(BlueprintCallable)
	float GetAverMike();
};
