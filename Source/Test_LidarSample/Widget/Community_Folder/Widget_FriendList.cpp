// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_FriendList.h"

//한국 시간에 맞춰서 로그아웃 시간을 계산해준다,
FTimespan UWidget_FriendList::GetElapsedTime(FDateTime LogoutTime)
{
	FDateTime DateTime= LogoutTime;

	FDateTime NowTime = FDateTime::UtcNow();
	FTimespan NineHours(9, 0, 0);
	NowTime = NowTime + NineHours;//한국 시간에 맞춰서 9시간을 더해준다

	FTimespan ElapsedTime = NowTime - DateTime;
	
	UE_LOG(LogTemp, Log, TEXT("// GetLastLogoutSeconds :: %s"), *NowTime.ToString());

	UE_LOG(LogTemp, Log, TEXT("// GetLastLogoutSeconds :: %d ,%d ,%d"), ElapsedTime.GetHours(), ElapsedTime.GetMinutes(), ElapsedTime.GetSeconds());

	return ElapsedTime;
}