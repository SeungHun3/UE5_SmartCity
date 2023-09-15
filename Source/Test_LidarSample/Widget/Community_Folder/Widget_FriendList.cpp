// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_FriendList.h"

//�ѱ� �ð��� ���缭 �α׾ƿ� �ð��� ������ش�,
FTimespan UWidget_FriendList::GetElapsedTime(FDateTime LogoutTime)
{
	FDateTime DateTime= LogoutTime;

	FDateTime NowTime = FDateTime::UtcNow();
	FTimespan NineHours(9, 0, 0);
	NowTime = NowTime + NineHours;//�ѱ� �ð��� ���缭 9�ð��� �����ش�

	FTimespan ElapsedTime = NowTime - DateTime;
	
	UE_LOG(LogTemp, Log, TEXT("// GetLastLogoutSeconds :: %s"), *NowTime.ToString());

	UE_LOG(LogTemp, Log, TEXT("// GetLastLogoutSeconds :: %d ,%d ,%d"), ElapsedTime.GetHours(), ElapsedTime.GetMinutes(), ElapsedTime.GetSeconds());

	return ElapsedTime;
}