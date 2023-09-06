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
	Entire, //��ü ����
	Bgm, //�����
	Effect, //ȿ����
	Voice, //����
	//Max,
};
UENUM(BlueprintType)
enum class enum_Alarm : uint8
{
	Consent, //�˸� ����
	Event, //����, �̺�Ʈ �˸�
	Fatigue, // �Ƿε� �˸�
	//Max,
};
UCLASS()
class TEST_LIDARSAMPLE_API UReadWriteText : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//���� �ε�
	UFUNCTION(BlueprintCallable, Category = "TxtSave", meta = (Keywords = "LoadLocal"))
		static bool LoadLocalTxt(FString FileName, TArray<FString>& LoadSoundArray, TArray<FString>& LoadMuteArray, TArray<FString>& LoadAlarmArray);
	//���� ����
	UFUNCTION(BlueprintCallable, Category = "TxtSave", meta = (Keywords = "SaveLocal"))
		static bool SaveLocalTxt(TArray<FString> SaveSoundArray, TArray<FString> SaveMuteArray, TArray<FString> SaveAlarmArray, FString FileName);
};
