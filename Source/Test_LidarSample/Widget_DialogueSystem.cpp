// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_DialogueSystem.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include <cmath>

#include "GameInstance_Solaseado.h"

void UWidget_DialogueSystem::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Next->OnClicked.AddUniqueDynamic(this, &UWidget_DialogueSystem::OnNextButtonClicked);
	Button_First->OnClicked.AddUniqueDynamic(this, &UWidget_DialogueSystem::OnFirstButtonClicked);
	Button_Second->OnClicked.AddUniqueDynamic(this, &UWidget_DialogueSystem::OnSecondButtonClicked);
}
//1key
//���� ��ư Ŭ��
void UWidget_DialogueSystem::OnNextButtonClicked()
{
	if (bOnNext)
	{
		CurrentPage++;
		NextMessage();
	}
}
//2Key �߿���
// ù ��° ��ư ���ε� �Լ�
void UWidget_DialogueSystem::OnFirstButtonClicked()
{
	OnNextButtonClicked();
}
// �� ��° ��ư ���ε� �Լ�
void UWidget_DialogueSystem::OnSecondButtonClicked()
{
	OnNextButtonClicked();
}
//�޼��� �ڽ� ����
void UWidget_DialogueSystem::RemoveMessageBox()
{
	//RemoveFromParent();
	SetVisibility(ESlateVisibility::Collapsed);
}
//��ȭ ������ �ʱ�ȭ
void UWidget_DialogueSystem::PageReset()
{
	CurrentPage = 1;
}
//��ȭâ ��Ÿ�� ����, ���� �߰�
void UWidget_DialogueSystem::AddMessageBox(enum_DialogueStyle style, const FText title, const FText& Message, const FText& FirstBtnText, const FText& SecondBtnText)
{
	SetVisibility(ESlateVisibility::Visible);

	TextBlock_Title->SetText(title);
	TextBlock_Message->SetText(Message);
	Button_FirstText->SetText(FirstBtnText);
	Button_SecondText->SetText(SecondBtnText);

	switch (style)
	{
		case enum_DialogueStyle::Dialogue1Key:
		{
			Overlay_1Key->SetVisibility(ESlateVisibility::Visible);
			Overlay_2Key->SetVisibility(ESlateVisibility::Collapsed);
			break;
		}
		case enum_DialogueStyle::Dialogue2Key:
		{
			Overlay_1Key->SetVisibility(ESlateVisibility::Collapsed);
			Overlay_2Key->SetVisibility(ESlateVisibility::Visible);
			break;
		}
	}
}
//��� ��ư ����
void UWidget_DialogueSystem::SetAllButton(bool IsEnabled)
{
	if (IsEnabled) 
	{
		Button_Next->SetIsEnabled(IsEnabled);
		Button_First->SetIsEnabled(IsEnabled);
		Button_Second->SetIsEnabled(IsEnabled);
		Button_Accept->SetIsEnabled(IsEnabled);
		Button_Drop->SetIsEnabled(IsEnabled);
	}
	else 
	{
		Button_Next->SetIsEnabled(IsEnabled);
		Button_First->SetIsEnabled(IsEnabled);
		Button_Second->SetIsEnabled(IsEnabled);
		Button_Accept->SetIsEnabled(IsEnabled);
		Button_Drop->SetIsEnabled(IsEnabled);
	}
}

void UWidget_DialogueSystem::Write(const FString text)
{
	UWorld* World = GetWorld();
	//��ȭ�� ���� ���� �ƴ� ��
	if (!Writing) 
	{
		Writing = true;
		MyText = text;
		//���� ���尡 ��ȿ�� ��� �ð� ���
		if (World) {
			StartTimestamp = UKismetSystemLibrary::GetGameTimeInSeconds(World);
		}
		//��ȭ ���� �ʱ�ȭ
		TextBlock_Message->SetText(FText::GetEmpty());//�޼��� �����
	}
}
//��ȭ �۾� Ÿ���� ȿ��
void UWidget_DialogueSystem::WriteText()
{
	UWorld* World = GetWorld();
	//�����ӿ� ǥ�õǾ�� �ϴ� ���� �� ���, �����Ӵ� ǥ���� ���� �� ��� �� ���� ���� ����
	float DeltaTime = (UKismetSystemLibrary::GetGameTimeInSeconds(World) - StartTimestamp) * 20;
	NumCharsToDisplay = FMath::Clamp(static_cast<int32>(std::floor(DeltaTime)), 0, MyText.Len());

	//ȭ�鿡 �ؽ�Ʈ�� �κ������� ǥ��
	FString Substring = MyText.Mid(0, NumCharsToDisplay);
	TextBlock_Message->SetText(FText::FromString(Substring));

	//�ۼ� �Ϸ� Ȯ��
	if (NumCharsToDisplay >= MyText.Len()) 
	{
		SetAllButton(true);
		Writing = false;
	}
	else 
	{
		SetAllButton(false);
	}
}