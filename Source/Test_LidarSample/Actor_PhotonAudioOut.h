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

// ���� ��� �߰� Ŭ���� ����
// ���� ���̺� ����
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
	//����� ������Ʈ : �̰ɷ� ���� ���� ��κ� ��Ʈ���ϸ� �ȴ�.
	UAudioComponent* mpAudioComponent;
	// ���� ���̽� �÷��̾� ����
	ExitGames::Voice::IAudioOut<short>* mpPlayer;
	
	// �÷��̾� ����
	bool bMmute = false;
	float fVolume; // ���Ұ� ��� ���� �����
	FString UserID; // ���� ���̵� : ��κ� �̰ɷ� �˻�
	int PlayerNr;
	////////////////////////////
public:
	//Get/Set ���� �Լ���
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
