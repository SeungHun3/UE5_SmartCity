// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWriteText.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

//���� �ε�
bool UReadWriteText::LoadLocalTxt(FString FileName, TArray<FString>& LoadSoundArray, TArray<FString>& LoadMuteArray)
{
	//TArray<FString> OutputArray;
	FString FileContent;
	FString FilePath = FPaths::ProjectDir() + FileName;
	// ���� ������ �迭 ������ ����
	TArray<FString> SaveTypeArray;

	if (FFileHelper::LoadFileToString(FileContent, *FilePath))
	{
		//�Ľ�
		FileContent.ParseIntoArray(SaveTypeArray, TEXT("\n"), true);
		
		if (SaveTypeArray.Num() >= 2)
		{
			SaveTypeArray[0].ParseIntoArray(LoadSoundArray, TEXT(","), true); //Sound
			SaveTypeArray[1].ParseIntoArray(LoadMuteArray, TEXT(","), true); //Mute
		}
		// �迭 ���� üũ, �ʱ�ȭ
		if (LoadSoundArray.Num() != 4)
		{
			LoadSoundArray.Init("5", 4);
		}
		else
		{
			// LoadSoundArray�� ��Ұ� 0~10 �������� Ȯ���ϰ� �ƴϸ� ��� 5�� �ʱ�ȭ
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
		if (LoadMuteArray.Num() != 4)
		{
			LoadMuteArray.Init("0", 4);
		}
		else
		{
			// LoadMuteArray�� ��Ұ� 0 �Ǵ� 1���� Ȯ���ϰ� �ƴϸ� ��� 0���� �ʱ�ȭ
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
		return true;
	}
	else 
	{
		// ���� �б� ����
		return false;
	}
}
//���� ����
bool UReadWriteText::SaveLocalTxt(TArray<FString> SaveSoundArray, TArray<FString> SaveMuteArray, FString FileName)
{
	// ���� ������ �迭 ������ ����
	TArray<FString> SaveTypeArray;
	// ������ �迭 Join
	SaveTypeArray.Add(FString::Join(SaveSoundArray, TEXT(","))); //Sound
	SaveTypeArray.Add(FString::Join(SaveMuteArray, TEXT(","))); //Mute
	
	// �迭 ���� üũ, �ʱ�ȭ
	if (SaveSoundArray.Num() != 4)
	{
		SaveSoundArray.Init("5", 4);
	}
	else
	{
		// SaveSoundArray�� ��Ұ� 0~10 �������� Ȯ���ϰ� �ƴϸ� ��� 5�� �ʱ�ȭ
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
	if (SaveMuteArray.Num() != 4)
	{
		SaveMuteArray.Init("0", 4);
	}
	else
	{
		// SaveMuteArray�� ��Ұ� 0 �Ǵ� 1���� Ȯ���ϰ� �ƴϸ� ��� 0���� �ʱ�ȭ
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
	//�� Ÿ���� ���� ���ڷ� Join
	FString OutPutString = FString::Join(SaveTypeArray, TEXT("\n"));

	// �ؽ�Ʈ ���Ϸ� ����
	if (FFileHelper::SaveStringToFile(OutPutString, *(FPaths::ProjectDir() + FileName)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
