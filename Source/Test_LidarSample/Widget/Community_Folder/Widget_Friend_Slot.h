// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget_Community_Slot.h"
#include "Widget_Friend_Slot.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class FriendTimeType : uint8
{
	Years = 0,
	Months,
	Days,
	Hours,
	Minute,		// 10sec
	Second,	// infinite
	Max,
};



UCLASS()
class TEST_LIDARSAMPLE_API UWidget_Friend_Slot : public UWidget_Community_Slot
{
	GENERATED_BODY()
	
};
