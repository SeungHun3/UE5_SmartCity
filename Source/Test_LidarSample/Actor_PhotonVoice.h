// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Player.h"
#include "Photon/SH_PhotonVoiceListener.h"
#include "Photon/SH_PhotonVoiceBasic.h"
#include "Actor_PhotonVoice.generated.h"

UCLASS()
class TEST_LIDARSAMPLE_API AActor_PhotonVoice : public AActor, public IAudioInFactory, public IAudioOutFactory, public SH_PhotonVoiceBasic
{
	GENERATED_BODY()

protected:
	void BeginPlay();
	void EndPlay(const EEndPlayReason::Type endPlayReason);
	void Tick(float DeltaSeconds);



public:
	AActor_PhotonVoice();



	virtual AActor_PhotonAudioIn* createAudioIn();
	virtual void destroyAudioIn(AActor_PhotonAudioIn*);
	virtual AActor_PhotonAudioOut* createAudioOut();
	virtual void destroyAudioOut(AActor_PhotonAudioOut*);

	virtual void updateState(int state, const ExitGames::Common::JString& stateStr, const ExitGames::Common::JString& joinedRoomName)
	{
		updateState(state, FString(stateStr.UTF8Representation().cstr()), FString(joinedRoomName.UTF8Representation().cstr()));
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
		void updateState(int32 state, const FString& stateStr, const FString& joinedRoomName);

	virtual void addPlayer(int playerNr, const ExitGames::Common::JString& playerName, bool local)
	{
		addPlayer(playerNr, FString(playerName.UTF8Representation().cstr()), local);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
		void addPlayer(int32 playerNr, const FString& playerName, bool local);

	virtual void updateRoomList(const ExitGames::Common::JVector<ExitGames::Common::JString>& roomNames)
	{
		TArray<FString> a;
		for (unsigned int i = 0; i < roomNames.getSize(); ++i)
			a.Push(FString(roomNames[i].UTF8Representation().cstr()));
		updateRoomList(a);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "Demo")
		void updateRoomList(const TArray<FString>& roomNames);

	// end of Demo BaseView overrrides 

	// Exposing demo properties to blueprint 
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Demo)
	//	bool gridSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
		FString serverAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
		FString AppID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
		FString appVersion;

	// Exposing demo methods to blueprint 
	UFUNCTION(BlueprintCallable, Category = "Demo")
		void Connect(const FString& Vociename);
	UFUNCTION(BlueprintCallable, Category = "Demo")
		void Disconnect();
private:
	UPROPERTY(EditAnywhere, Category = "Demo")
		bool automove;
	UPROPERTY(EditAnywhere, Category = "Demo")
		bool useGroups;

	void PostEditChangeProperty(struct FPropertyChangedEvent& e);

	
	
	//오디오 입출력 관리
	ExitGames::Voice::IAudioPusher<short>* audioInFactory();
	ExitGames::Voice::IAudioOut<short>* audioOutFactory();

	//에러문구 출력
	virtual void ErrorCheckMessage(const FString& message, int error) override;

	//보이스 연결 완료
	virtual void Voice_ConnectComplete(void) override;
	//친구 업데이트 완료시 출력
	virtual void Voice_UpdateFriend(void) override;

	//내 목소리 에코 테스트용
	UFUNCTION(BlueprintCallable)
	void ToggleEcho();
public:
	//입력 오디오
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class AActor_PhotonAudioIn> getAudioIn;

	//출력 오디오
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class AActor_PhotonAudioOut> getAudioOut;

	//내 마이크 입력 오디오 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor_PhotonAudioIn* mPhotonAudioIn;

	//플레이어들의 마이크 출력 오디오 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AActor_PhotonAudioOut*> mPhotonAudioOut;

public:
	//플레이어 출력
	UFUNCTION(BlueprintCallable, Category = "PhotonAudio")
		void SetMuteIn(bool bInput);
	UFUNCTION(BlueprintCallable, Category = "PhotonAudio")
		void SetMuteOut(bool bInput);
	UFUNCTION(BlueprintCallable, Category = "PhotonAudio")
		void SetMute(bool bInput);

	//포톤 보이스에 연결됐을때
	UFUNCTION(BlueprintImplementableEvent)
	void ConnectComplete();

	UFUNCTION(BlueprintCallable)
	uint8 GetInMikeVolume();

	class SH_PhotonVoiceListener* mpPhotonLib;
	bool getIsChanging();
	void setIschanging(bool Change);
	void Voice_ChangeOrJoinRoom(const FString& RoomFullName);


	//UFUNCTION(BlueprintCallable)
	//void UpdateFriendAudio();

public:
	ExitGames::LoadBalancing::Client* PhotonClient;

	// 플레이어 유저
	class APawn_Player* LocalPlayer;

	//리스너에서 유저 관리하기위해서 여기서 로그인
	void ConnectLogin(const FString& username);

	//블랙리스트 관련 음성 업데이트
	UFUNCTION(BlueprintCallable)
	void UpdateFriendVoice();

	//유저 음성 조절
	UFUNCTION(BlueprintCallable)
	bool SetSoundVolume(FString UserID, float volume);

	//유저 음성 얻기
	UFUNCTION(BlueprintCallable)
	float GetSoundVolume(FString UserID);

	//음소거 상태 얻기
	UFUNCTION(BlueprintCallable)
	bool GetMute(FString UserID);
};
