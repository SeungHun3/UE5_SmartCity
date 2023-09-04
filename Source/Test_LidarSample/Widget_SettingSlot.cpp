// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_SettingSlot.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"

void UWidget_SettingSlot::SetVolume(float Value)
{
	if (Slider_Slot)
	{
		Slider_Slot->SetValue(Value);
	}
}

void UWidget_SettingSlot::SetProgressPercent(float Input)
{
	if (ProgressBar_Slot)
	{
		
		ProgressBar_Slot->SetPercent(Input);
	}
}
