// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent_PlayfabStore.h"
#include "Pawn_Player.h"

#include "Core/PlayFabClientAPI.h"
#include "GameModeBase_Solaseado.h"
#include "Widget/Widget_Popup.h"
#include "PlayFabJsonObject.h"
#include "PlayFabJsonValue.h"
using namespace PlayFab;

// Sets default values for this component's properties
UActorComponent_PlayfabStore::UActorComponent_PlayfabStore()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UActorComponent_PlayfabStore::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UActorComponent_PlayfabStore::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Playfab Store Error 
void UActorComponent_PlayfabStore::StoreErrorScript(const PlayFab::FPlayFabCppError& error)
{
	FString str = error.GenerateErrorReport();
	UE_LOG(LogTemp, Log, TEXT("// Playfab Store Error Script ( %s ):: %s"), *error.ErrorName, *str);
	// ������ ���� ����
	// "WrongPrice" || "WrongVirtualCurrency") // ItemNotFound , StoreNotFound
	if (error.ErrorName == "InsufficientFunds")
	{
		// �� ���� Error
		UE_LOG(LogTemp, Log, TEXT("// Playfab Store Error _ InsufficientFunds "));
		// �˾�
		
		AGameModeBase_Solaseado* GM_Solaseado = Cast<AGameModeBase_Solaseado>(GetWorld()->GetAuthGameMode());
		GM_Solaseado->CreatePopup();
		//WidgetPopup->CheckPopup(enum_PopupStyle::Popup1Key, FText::FromString("fefefefef"), FText::FromString("fefefef"), enum_PopupRun::Cancel);
	}
}

void UActorComponent_PlayfabStore::getStoreItemList(const FString& CatalogVersion, const FString& StoreID)
{

	if (CatalogVersion.IsEmpty() || StoreID.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("// Empty CatalogVersion or StoreID"));
		return;
	}

	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	ClientModels::FGetStoreItemsRequest request;

	// ���� ���� ����
	request.CatalogVersion = CatalogVersion;
	FString version = request.CatalogVersion;
	request.StoreId = StoreID;
	FString StoreId = request.StoreId;

	ClientAPI->GetStoreItems(
		request,
		UPlayFabClientAPI::FGetStoreItemsDelegate::CreateLambda([&, version, StoreId](const ClientModels::FGetStoreItemsResult& result) {

			TArray<FITemInfo> ShopDatas;

			for (auto it : result.Store)
			{
				if (it.VirtualCurrencyPrices.Contains("GC"))
				{
					//GC������ ������ �ִ� Store �߿���
					FITemInfo infoTemp;
					infoTemp.ItemID = it.ItemId;
					infoTemp.ItemPrice = *it.VirtualCurrencyPrices.Find("GC");
					infoTemp.StoreID = StoreId;
					infoTemp.VirtualCurrency = "GC";
					infoTemp.CatalogVersion = version;
					ShopDatas.Push(infoTemp);

				}
			}
			// ���� ����� �迭�� �Ѱ��ֱ�
			if (PlayerOwner)
			{
				PlayerOwner->UpdateStore(ShopDatas);
			}

			}), 

		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_PlayfabStore::StoreErrorScript)
		);
}

void UActorComponent_PlayfabStore::PurchaseItem(FITemInfo Item)
{
	
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	// ������ ������ ������ ID�� ����
	PlayFab::ClientModels::FPurchaseItemRequest request;
	request.CatalogVersion = Item.CatalogVersion;
	request.ItemId = Item.ItemID;
	request.VirtualCurrency = "GC";
	request.Price = Item.ItemPrice;
	request.StoreId = Item.StoreID;

	ClientAPI->PurchaseItem(
		request,
		PlayFab::UPlayFabClientAPI::FPurchaseItemDelegate::CreateLambda([&](const PlayFab::ClientModels::FPurchaseItemResult& result) {

				for (auto it : result.Items)
				{
					UE_LOG(LogTemp, Log, TEXT("// PurchaseItem id : %s"), *it.ItemId);

					// ���� ������ ����. ���� ���� �������� ó��
					PurchaseNewItem.ItemId = it.ItemId;
					PurchaseNewItem.ItemClass = it.ItemClass;
					PurchaseNewItem.ItemInstanceId = it.ItemInstanceId;
					PurchaseNewItem.UnitPrice = it.UnitPrice;
					PurchaseNewItem.RemainingUses = it.RemainingUses;
				}
				//���� �ٻ�� �������� ������Ʈ
				UpdateCoin();
			}), 
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_PlayfabStore::StoreErrorScript)
		);
}

void UActorComponent_PlayfabStore::UpdateCoin()
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	PlayFab::ClientModels::FGetUserInventoryRequest request;
	
	ClientAPI->GetUserInventory(
		request,
		PlayFab::UPlayFabClientAPI::FGetUserInventoryDelegate::CreateLambda([&](const PlayFab::ClientModels::FGetUserInventoryResult& result) {

			// ���� ������ ������ ���� ���� ������ ������ ���Ͽ� > ���� �߰��ϱ�
			// InventoryProperty; // result.Inventory
			//for (auto it : result.Inventory)
			//{
			// 
			//}
			// 
			// ������ ���� ������ 1���� ��� ���� ó�� ��� .
			if (PlayerOwner) {
				if (!PurchaseNewItem.ItemId.IsEmpty())
				{
					InventoryProperty.Add(PurchaseNewItem);
					PlayerOwner->Blueprint_AddInventoryItem(PurchaseNewItem);
					PurchaseNewItem.Clear();
				}
			}
			// ������ ���� �� ���� ���� ���� ó��
			const int* money = result.VirtualCurrency.Find("GC");
			if (PlayerOwner && money)
			{
				PlayerOwner->CoinUpdate(money[0]);
				UE_LOG(LogTemp, Log, TEXT("// MyMoney : %d"), money[0]);
			}

			}));
}
