// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Popup.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/HorizontalBox.h"

void UWidget_Popup::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Confirm->OnClicked.AddDynamic(this, &UWidget_Popup::ClickedConfirm);
	Button_NO->OnClicked.AddDynamic(this, &UWidget_Popup::ClickedNo);
	Button_Yes->OnClicked.AddDynamic(this, &UWidget_Popup::ClickedYes);
}
//1Key Ȯ�� ��ư
void UWidget_Popup::ClickedConfirm()
{
	PopupClear.Broadcast();
	PopupConfirmEvent.Broadcast(PopupRun);
}
//2Key ��� ��ư
void UWidget_Popup::ClickedNo()
{
	bIsClickedNo = true;
	if (PopupRun != enum_PopupRun::Tutorial)
	{
		PopupClear.Broadcast();
		PopupCancelEvent.Broadcast(PopupRun);
	}
}
//2key Ȯ�� ��ư : �̺�Ʈ ó��
void UWidget_Popup::ClickedYes()
{
	//PopupRun
	PopupRunEvent.Broadcast(PopupRun);
	PopupClear.Broadcast();
}
void UWidget_Popup::MenuOff()
{
	bIsClikedNo = false;
	PopupClear.Broadcast();
	PopupCancelEvent.Broadcast(PopupRun);
}
//�˾�â style ����
void UWidget_Popup::AddPopup(enum_PopupStyle style, FText message, FText title, enum_PopupRun run, FText btn_yes, FText btn_no, FText btn_confirm, bool slot_Image)
{
	//���
	Overlay_Caption->SetVisibility(ESlateVisibility::Collapsed);
	// ���� �ؽ�Ʈ, ���� ǥ�� �߰�
	Overlay_Sub->SetVisibility(ESlateVisibility::Collapsed);
	// ���
	Overlay_RedText->SetVisibility(ESlateVisibility::Collapsed);

	CanvasPanel_Checkpopup->SetVisibility(ESlateVisibility::Visible);
	switch (style)
	{
	case enum_PopupStyle::Popup1Key:
	{
		CanvasPanel_1Key->SetVisibility(ESlateVisibility::Visible);
		CanvasPanel_2Key->SetVisibility(ESlateVisibility::Collapsed);
		SetButtonText(TextBlock_Confirm, btn_confirm);
		break;
	}
	case enum_PopupStyle::Popup2Key:
	{
		CanvasPanel_1Key->SetVisibility(ESlateVisibility::Collapsed);
		CanvasPanel_2Key->SetVisibility(ESlateVisibility::Visible);
		SetButtonText(TextBlock_Yes, btn_yes);
		SetButtonText(TextBlock_No, btn_no);
		break;
	}
	}
	if (slot_Image)
	{
		setPopupSlotImage();
	}
	PopupRun = run;
	TextBlock_Notice->SetText(message);
	TextBlock_Title->SetText(title);
}
//  ��� ���� �߰� 
void UWidget_Popup::setRedText(FText red)
{
	if (red.IsEmpty())
	{
		Overlay_RedText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Overlay_RedText->SetVisibility(ESlateVisibility::HitTestInvisible);
		TextBlock_RedText->SetText(red);
	}
}
//���� �̹���
void UWidget_Popup::setPopupSlotImage()
{
	Overlay_Image->SetVisibility(ESlateVisibility::Visible);
	setItemTexture();
}
//���� �̹��� �ݱ�
void UWidget_Popup::ClosePopupSlotImage()
{
	Overlay_Image->SetVisibility(ESlateVisibility::Collapsed);
}

void UWidget_Popup::setButtonEnabled()
{
	Button_Confirm->SetIsEnabled(true);
	Button_NO->SetIsEnabled(true);
	Button_Yes->SetIsEnabled(true);
}

void UWidget_Popup::setButtonDisabled()
{
	Button_Confirm->SetIsEnabled(false);
	Button_NO->SetIsEnabled(false);
	Button_Yes->SetIsEnabled(false);
}
//��ư �ؽ�Ʈ ����
void UWidget_Popup::SetButtonText(UTextBlock* ButtonTextBlock, FText ButtonText)
{
	ButtonTextBlock->SetText(ButtonText);
}
