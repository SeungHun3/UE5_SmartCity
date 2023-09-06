// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWriteText.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

//파일 로드
bool UReadWriteText::LoadLocalTxt(FString FileName, TArray<FString>& LoadSoundArray, TArray<FString>& LoadMuteArray, TArray<FString>& LoadAlarmArray)
{
	//TArray<FString> OutputArray;
	FString FileContent;
	FString FilePath = FPaths::ProjectDir() + FileName;
	// 여러 종류의 배열 저장을 위해
	TArray<FString> SaveTypeArray;

	if (FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		//파싱
		FileContent.ParseIntoArray(SaveTypeArray, TEXT("\n"), true);
		
		if (SaveTypeArray.Num() >= 3)
		{
			SaveTypeArray[0].ParseIntoArray(LoadSoundArray, TEXT(","), true); //Sound
			SaveTypeArray[1].ParseIntoArray(LoadMuteArray, TEXT(","), true); //Mute
			SaveTypeArray[2].ParseIntoArray(LoadAlarmArray, TEXT(","), true); //Alarm
		}
		// 사운드 배열 길이 체크, 초기화
		if (LoadSoundArray.Num() != 4)
		{
			LoadSoundArray.Init("5", 4);
		}
		else
		{
			// LoadSoundArray의 요소가 0~10 사이인지 확인하고 아니면 모두 5로 초기화
			for (const FString& Sound : LoadSoundArray)
			{
				int32 SoundValue = FCString::Atoi(*Sound);
				if (SoundValue < 0 || SoundValue > 10)
				{
					LoadSoundArray.Init("5", 4);
					break;
				}
			}
		}
		// 음소거 배열 길이 체크, 초기화
		if (LoadMuteArray.Num() != 4)
		{
			LoadMuteArray.Init("0", 4);
		}
		else
		{
			// LoadMuteArray의 요소가 0 또는 1인지 확인하고 아니면 모두 0으로 초기화 
			for (const FString& Mute : LoadMuteArray)
			{
				int32 MuteValue = FCString::Atoi(*Mute);
				if (MuteValue != 0 && MuteValue != 1)
				{
					LoadMuteArray.Init("0", 4);
					break;
				}
			}
		}
		// 알림 배열 길이 체크, 초기화
		if (LoadAlarmArray.Num() != 3)
		{
			LoadAlarmArray.Init("0", 3);
		}
		else
		{
			// LoadAlarmArray의 요소가 0 또는 1인지 확인하고 아니면 모두 0으로 초기화 // 0: 알림 거부 상태, 1: 알림 허용 상태
			for (const FString& Alarm : LoadAlarmArray)
			{
				int32 AlarmValue = FCString::Atoi(*Alarm);
				if (AlarmValue != 0 && AlarmValue != 1)
				{
					LoadAlarmArray.Init("0", 3);
					break;
				}
			}
		}
		return true;
	}
	else 
	{
		// 파일 읽기 실패
		return false;
	}
}
//파일 저장
bool UReadWriteText::SaveLocalTxt(TArray<FString> SaveSoundArray, TArray<FString> SaveMuteArray, TArray<FString> SaveAlarmArray, FString FileName)
{
	// 여러 종류의 배열 저장을 위해
	TArray<FString> SaveTypeArray;
	// 가져온 배열 Join
	SaveTypeArray.Add(FString::Join(SaveSoundArray, TEXT(","))); //Sound
	SaveTypeArray.Add(FString::Join(SaveMuteArray, TEXT(","))); //Mute
	SaveTypeArray.Add(FString::Join(SaveAlarmArray, TEXT(","))); //Alarm
	
	// 사운드 배열 길이 체크, 초기화
	if (SaveSoundArray.Num() != 4)
	{
		SaveSoundArray.Init("5", 4);
	}
	else
	{
		// SaveSoundArray의 요소가 0~10 사이인지 확인하고 아니면 모두 5로 초기화
		for (const FString& Sound : SaveSoundArray)
		{
			int32 SoundValue = FCString::Atoi(*Sound);
			if (SoundValue < 0 || SoundValue > 10)
			{
				SaveSoundArray.Init("5", 4);
				break;
			}
		}
	}
	// 음소거 배열 길이 체크, 초기화
	if (SaveMuteArray.Num() != 4)
	{
		SaveMuteArray.Init("0", 4);
	}
	else
	{
		// SaveMuteArray의 요소가 0 또는 1인지 확인하고 아니면 모두 0으로 초기화
		for (const FString& Mute : SaveMuteArray)
		{
			int32 MuteValue = FCString::Atoi(*Mute);
			if (MuteValue != 0 && MuteValue != 1)
			{
				SaveMuteArray.Init("0", 4);
				break;
			}
		}
	}
	// 알람 배열 길이 체크, 초기화
	if (SaveAlarmArray.Num() != 3)
	{
		SaveAlarmArray.Init("0", 3);
	}
	else {
		// SaveAlarmArray의 요소가 0 또는 1인지 확인하고 아니면 모두 0으로 초기화
		for (const FString& Alarm : SaveAlarmArray)
		{
			int32 AlarmValue = FCString::Atoi(*Alarm);
			if (AlarmValue != 0 && AlarmValue != 1)
			{
				SaveAlarmArray.Init("0", 3);
				break;
			}
		}
	}
	//각 타입은 개행 문자로 Join
	FString OutPutString = FString::Join(SaveTypeArray, TEXT("\n"));

	// 텍스트 파일로 저장
	if (FFileHelper::SaveStringToFile(OutPutString, *(FPaths::ProjectDir() + FileName)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
