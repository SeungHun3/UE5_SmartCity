// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/Voice/Public/VoiceModule.h"
#include "PhotonVoice-cpp/inc/VoiceAudio.h"
#include "Actor_PhotonAudioIn.generated.h"

class IAudioInFactory
{
public:
	virtual ~IAudioInFactory() {}
	virtual class AActor_PhotonAudioIn* createAudioIn() = 0;
	virtual void destroyAudioIn(class AActor_PhotonAudioIn*) = 0;
};

class AudioIn : public ExitGames::Voice::IAudioPusher<short>
{
public:
	// Sets default values for this actor's properties
	AudioIn(class IAudioInFactory*);
	virtual ~AudioIn();

	//IAudioPusher
	virtual void setCallback(void*, void(*)(void*, const ExitGames::Voice::Buffer<short>&));
	//1�ʵ��� ���ø� �� Ƚ���� ��´�
	virtual int getSamplingRate(void) const;
	//���� ä���� ��´�.
	virtual int getChannels(void) const;
	virtual ExitGames::Common::JString getError(void) const;
	virtual ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr, bool withTypes = false) const;

public:
	virtual void MuteInputSound(bool mute);

private:

	class IAudioInFactory* mpAudioInFactory;

	class AActor_PhotonAudioIn* mpActor;
	ExitGames::Common::JString mError;
};

UCLASS()
class TEST_LIDARSAMPLE_API AActor_PhotonAudioIn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActor_PhotonAudioIn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//	virtual void Tick(float DeltaTime) override;
	
	//ĸó�� ����
	TSharedPtr<IVoiceCapture> mspCapture;
	FTimerHandle mTimer;

	//mute
	bool bMute = false;

	//�ݹ��Լ�
	void setCallback(void* callbackOpaque, void(*callback)(void*, const ExitGames::Voice::Buffer<short>&));

	////����ũ �Է� ��հ� 
	short aveMike = 0.0f;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckSoundIn(float buff);

private:
	//Ÿ�̸� ������ //���⼭ ��� �Էµ� ���带 ĸó�ؼ� �������ش�.
	void onTimer();

	void* mpCallbackOpaque;
	void(*mpCallback)(void*, const ExitGames::Voice::Buffer<short>&);
	ExitGames::Common::JString mError;

	void AveageMike(const short* buf);

	UFUNCTION(BlueprintCallable)
	float GetAverMike();
};
