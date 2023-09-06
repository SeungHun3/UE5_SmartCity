// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteText.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class enum_Sound : uint8
{
	Entire, //전체 사운드
	Bgm, //배경음
	Effect, //효과음
	Voice, //음성
	//Max,
};
UENUM(BlueprintType)
enum class enum_Alarm : uint8
{
	Consent, //알림 동의
	Event, //공지, 이벤트 알림
	Fatigue, // 피로도 알림
	//Max,
};
UCLASS()
class TEST_LIDARSAMPLE_API UReadWriteText : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//파일 로드
	UFUNCTION(BlueprintCallable, Category = "TxtSave", meta = (Keywords = "LoadLocal"))
		static bool LoadLocalTxt(FString FileName, TArray<FString>& LoadSoundArray, TArray<FString>& LoadMuteArray, TArray<FString>& LoadAlarmArray);
	//파일 저장
	UFUNCTION(BlueprintCallable, Category = "TxtSave", meta = (Keywords = "SaveLocal"))
		static bool SaveLocalTxt(TArray<FString> SaveSoundArray, TArray<FString> SaveMuteArray, TArray<FString> SaveAlarmArray, FString FileName);
};
