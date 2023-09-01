// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ShopMain.h"
#include "Widget_ShopSlot.h"


void UWidget_ShopMain::Min_Price_Sort(TArray<class UWidget_ShopSlot*> SlotArray)
{
    // ������ ������ �ִ� ���ݼ����� �������� ����
	for (int i = 0; i < SlotArray.Num(); i++) /*Num == size*/
	{
        int minIndex = i;
        for (int j = i + 1; j < SlotArray.Num(); ++j)
        {
            if (SlotArray[j]->Item_Infomation.ItemPrice < SlotArray[minIndex]->Item_Infomation.ItemPrice)
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            UWidget_ShopSlot* tempSlot = SlotArray[i];
            SlotArray[i] = SlotArray[minIndex];
            SlotArray[minIndex] = tempSlot;
        }
	}

}
