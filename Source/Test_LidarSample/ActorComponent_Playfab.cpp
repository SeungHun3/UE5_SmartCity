// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent_Playfab.h"
#include "Pawn_Player.h"
#include "Engine/Engine.h"
#include "Widget/Checking_Folder/Widget_CheckingAttandance_Main.h"
#include "GameInstance_Solaseado.h"
#include "GameModeBase_Solaseado.h"

#include "Core/PlayFabClientAPI.h"
#include "Misc/DateTime.h"
#include "Actor_HTTPRequest.h"

#include "PlayFabJsonObject.h"
#include "PlayFabJsonValue.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"


using namespace PlayFab;

// Sets default values for this component's properties
UActorComponent_Playfab::UActorComponent_Playfab()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

		// ��Ģ�� ����Ʈ > �г���
	static ConstructorHelpers::FObjectFinder<UDataTable> FindBadNameData(TEXT("/Game/Project/DataTable/Data_BadName"));
	if (FindBadNameData.Succeeded())
		BadNameTable = FindBadNameData.Object;
}

// Called when the game starts
void UActorComponent_Playfab::BeginPlay()
{
	Super::BeginPlay();

	// Playfab title ID
	GetMutableDefault<UPlayFabRuntimeSettings>()->TitleId = TEXT("C9B19");
	// Player Pawn 
	PlayerOwner = Cast<APawn_Player>(GetOwner());
}

// Called every frame
void UActorComponent_Playfab::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Playfab Script // Login
// Custom Login 
void UActorComponent_Playfab::Login_Custom(const FString& customid)
{
	// Ŀ���� ���̵�
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	ClientModels::FLoginWithCustomIDRequest request;

	request.CreateAccount = true;
	request.CustomId = customid;
	
	ClientAPI->LoginWithCustomID(
		request,
		UPlayFabClientAPI::FLoginWithCustomIDDelegate::CreateLambda([&](const ClientModels::FLoginResult& result) {

			PlayFabID = result.PlayFabId;
			LastLoginTime = result.LastLoginTime.mValue.ToString();

			UE_LOG(LogTemp, Log, TEXT("// custom Login Success :: %s ( %s )"), *PlayFabID, *LastLoginTime);
			// UE_LOG(LogTemp, Log, TEXT("// SessionTicket :: ( %s )"), *result.AuthenticationContext->GetClientSessionTicket());

			// ���� ���� ���� üũ
			// getServerTitleData(result.NewlyCreated);

			// �α��� ���� // NewlyCreated �ֱ� ���� ���� �Ǵ�
			SuccessLogin(result.NewlyCreated);

			}),
		FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}
// Email Login 
void UActorComponent_Playfab::Login_Email(const FString& email, const FString& pw)
{
	// ���� ���̵� ó�� 
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	ClientModels::FLoginWithEmailAddressRequest request;

	request.Email = email;
	request.Password = pw;

	ClientModels::FPurchaseItemRequest asd;
	
	ClientAPI->PurchaseItem(asd);


	ClientAPI->LoginWithEmailAddress(
		request,
		UPlayFabClientAPI::FLoginWithEmailAddressDelegate::CreateLambda([&](const ClientModels::FLoginResult& result) {

			PlayFabID = result.PlayFabId;
			LastLoginTime = result.LastLoginTime.mValue.ToString();
			UE_LOG(LogTemp, Log, TEXT("// EmailAddress Login Success :: %s ( %s )"), *PlayFabID, *LastLoginTime);
			// UE_LOG(LogTemp, Log, TEXT("// SessionTicket :: ( %s )"), *result.AuthenticationContext->GetClientSessionTicket());
			// getUserTitleData();

			}),
		FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}

// Playfab Script
// �÷����� ��ũ��Ʈ �̺�Ʈ ȣ��
// // �Ű����� x 
void UActorComponent_Playfab::ScriptCustomNotField(const FString& FunctionName)
{
	ErrorFunctionName = FunctionName;
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = FunctionName;
	request.GeneratePlayStreamEvent = true;

	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateUObject(this, &UActorComponent_Playfab::SuccessScriptEvent),
		FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
	);

}
// // �Ű����� o
void UActorComponent_Playfab::ScriptCustom(const FString& FunctionName, const FString& FieldName, const FString& value)
{
	ErrorFunctionName = FunctionName;
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	UE_LOG(LogTemp, Log, TEXT("// ScriptCustom :: %s , value :: %s "), *ErrorFunctionName, *value);

	TSharedPtr<FJsonObject> outerWrapper = MakeShareable(new FJsonObject());
	outerWrapper->SetStringField(FieldName, value);

	auto functionParameter = PlayFab::FJsonKeeper();
	functionParameter.readFromValue(MakeShareable(new FJsonValueObject(outerWrapper)));

	PlayFab::ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = FunctionName;
	request.FunctionParameter = functionParameter;
	request.GeneratePlayStreamEvent = true;

	ClientAPI->ExecuteCloudScript(
		request,
		PlayFab::UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateUObject(this, &UActorComponent_Playfab::SuccessScriptEvent),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript));

}
// // �Ű����� �迭 
void UActorComponent_Playfab::ScriptCustomArray(const FString& FunctionName, const FString& FieldName, const TArray<FString>& StringArray)
{
	ErrorFunctionName = FunctionName;
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());

	if (!JsonObj.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("// JSonObj isValid "));
		return;
	}

	TArray< TSharedPtr<FJsonValue> > EntriesArray;

	for (auto String : StringArray)
	{
		EntriesArray.Add(MakeShareable(new FJsonValueString(String)));
	}

	JsonObj->SetArrayField(FieldName, EntriesArray);

	PlayFab::ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = FunctionName;
	request.FunctionParameter = JsonObj;
	request.GeneratePlayStreamEvent = true;

	ClientAPI->ExecuteCloudScript(
		request,
		PlayFab::UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateUObject(this, &UActorComponent_Playfab::SuccessScriptEvent),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript));

}

// ��ũ��Ʈ ȣ�� ���� �̺�Ʈ 
// Playfab �ܺ� ��ũ��Ʈ ȣ��� ���� ��� ������ ������ ��
void UActorComponent_Playfab::SuccessScriptEvent(const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
{
	UE_LOG(LogTemp, Log, TEXT("// SuccessScriptEvent:: %s"), *result.FunctionName);
	FJsonValue* value = result.FunctionResult.GetJsonValue().Get();
	if (!value->IsNull())
	{
		ScriptResponseEvent(value);
	}
	else
		UE_LOG(LogTemp, Log, TEXT("// SuccessScriptEvent Not return Data"));
}

// ��ũ��Ʈ ȣ�� ����
void UActorComponent_Playfab::ErrorScript(const PlayFab::FPlayFabCppError& error)
{
	FString str = error.GenerateErrorReport();

	UE_LOG(LogTemp, Log, TEXT("// Error Script ( %s ):: %s"), *error.ErrorName, *str);

	// "Item not found" ������ ���Ž� �߸��� ���� �Է�
	if (error.ErrorName == "Item not found")
	{
		UE_LOG(LogTemp, Log, TEXT("// Item not found"));
		return;
	}
	// �ߺ� �̸� , �̸� ���� ���� ���� (string 3 ~24)
	if (error.ErrorName == "NameNotAvailable" || "InvalidParams")
	{
		// CheckerCharacterName = error.ErrorName;
		bCheckerCreateChracter = false;
		// ���н� ���� ������ ���� 
		if (PlayerOwner)
			PlayerOwner->CustomizingWidgetNameChecker(bCheckerCreateChracter);
	}
	//// ���̵� ��
	//if (error.ErrorName == "AccountBanned")
	//{
	//	// ���� , ���� ����
	//	// FString EmaliCheck;
	//	AddPopupWidget(nullptr, enum_PopupStyle::Popup1Key, enum_PopupRun::Cancel,
	//		FText::FromString(str + "If you want to check the Ban information, contact ad@ma.com."), FText::FromString("AccountBanned"));
	//}
}

//////////////////////////////////////////////////////
// �⼮üũ 
// ���� üũ�ߴ��� Ȯ�ο���
bool UActorComponent_Playfab::Is_Today_Checked()
{
	if (PlayFab_Statistics.Contains("Is_Checked_Daily")) // �÷����� �������忡�� ���� üũ�� �ߴ��� Ȯ�� �Ŀ�
	{
		int32 isChecked = *PlayFab_Statistics.Find("Is_Checked_Daily");
		// ���� üũ�� �ߴٸ� = true, ���ߴٸ� false
		if (isChecked == 1)
		{
			return true;
		}
		else { return false; }
		
	}
	else //Is_Checked_Daily�� Ű�� ���ٸ�
	{
		ScriptCustomNotField("Default_Statistics");
		UE_LOG(LogTemp, Log, TEXT("// NoData : Is_Checked_Daily"));
		return false;

	}
}
// ���ñ��� üũ�� �⼮�ϼ�
int UActorComponent_Playfab::Get_Checking_Count()
{
	if (PlayFab_Statistics.Contains("Month_Reward_Count")) // �⼮ ī��Ʈ ������ �����ͼ� ��ȯ
	{
		return *PlayFab_Statistics.Find("Month_Reward_Count");
	}
	else // �⼮ �����Ͱ� ���ٸ� 
	{
		ScriptCustomNotField("Default_Statistics");
		UE_LOG(LogTemp, Log, TEXT("// NoData : Month_Reward_Count"));
		return 0;
	}
}

//PlayFab���� daily check ������Ʈ + checkCount ++ -> return checkCount 
void UActorComponent_Playfab::Update_Check_Change(UWidget_CheckingAttandance_Main* Widget, int coin)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI(); 
	TSharedPtr<FJsonObject> outerWrapper = MakeShareable(new FJsonObject());
	outerWrapper->SetNumberField("Coin", coin);
	auto functionParameter = PlayFab::FJsonKeeper();
	functionParameter.readFromValue(MakeShareable(new FJsonValueObject(outerWrapper)));
	
	ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = "Update_Attandance";
	request.GeneratePlayStreamEvent = true;
	request.FunctionParameter = functionParameter;

	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateLambda([&, Widget](const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
			{
				TSharedPtr<FJsonObject> result_Object = result.FunctionResult.GetJsonValue()->AsObject();
				TArray<FString> Keys;
				result_Object->Values.GetKeys(Keys); // ���ϰ� �ϳ��� Key �� ������


				if ((Keys[0] == "Month_Reward_Count")&& result_Object->Values.Contains(Keys[0]))
				{
						int CheckingCount = static_cast<int>(result_Object->Values.Find(Keys[0])->Get()->AsNumber()); // ���Ϲ��� Count��
						
						if (PlayFab_Statistics.Contains(Keys[0]) && PlayFab_Statistics.Find(Keys[0]))
						{
							PlayFab_Statistics.Add(Keys[0], CheckingCount); // Map ������ �����Ű��
							// ������ ����
							Widget->ChangeSlot();
						}
				}
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}
// 20�� ����
void UActorComponent_Playfab::Daily_20th_Reward(UWidget_CheckingAttandance_Main* Widget, int Coin)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	TSharedPtr<FJsonObject> outerWrapper = MakeShareable(new FJsonObject());
	outerWrapper->SetNumberField("Coin", Coin);
	auto functionParameter = PlayFab::FJsonKeeper();
	functionParameter.readFromValue(MakeShareable(new FJsonValueObject(outerWrapper)));

	ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = "Coin_Grant";
	request.GeneratePlayStreamEvent = true;
	request.FunctionParameter = functionParameter;

	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateLambda([&, Widget](const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
			{
				//������ �����ְ��� �����Լ� ȣ��
				Widget->ChangeSpecial();
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);

}


// ������ �⼮üũ �ʱ�ȭ
void UActorComponent_Playfab::Debug_Clear_Attandance(UWidget_CheckingAttandance_Main* Widget)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = "Clear_Attandance";
	request.GeneratePlayStreamEvent = true;
	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateLambda([&, Widget](const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
			{
				PlayFab_Statistics.Add("Month_Reward_Count", 0);
				PlayFab_Statistics.Add("Is_Checked_Daily", 0);

				//������ �����ְ��� �����Լ� ȣ��
				Widget->Debug_Finished_Clear();
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}
// ������ ���� �⼮���� �ʱ�ȭ
void UActorComponent_Playfab::Debug_Clear_Daily(UWidget_CheckingAttandance_Main* Widget)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = "Clear_CheckDaily";
	request.GeneratePlayStreamEvent = true;
	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateLambda([&, Widget](const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
			{
				PlayFab_Statistics.Add("Is_Checked_Daily", 0);

				//������ �����ְ��� �����Լ� ȣ��
				Widget->Debug_Finished_DailyClear();
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}


//////////////////////////////////////////////////////
// �⼮üũ ��




// Playfab Response
// �÷����� ��ũ��Ʈ �ݺ� �̺�Ʈ
void UActorComponent_Playfab::ScriptResponseEvent(FJsonValue* value)
{
	FString Selection;

	TArray<FString> getFieldNames;
	value->AsObject().Get()->Values.GetKeys(getFieldNames);

	if (getFieldNames.Num() > 0)
		Selection = getFieldNames[0];

	// CallBack String Data
	FString getStringData = value->AsObject().Get()->GetStringField(Selection);
	UE_LOG(LogTemp, Log, TEXT("// getStringData :: %s"), *getStringData);

	// �÷��̾� ������ ������ ������Ʈ
	if (Selection == "UpdataDefaultValue")
	{
		UE_LOG(LogTemp, Log, TEXT("// Playfab _ updateDefaultValue :: %s"), *getStringData);
		getUserTitleData();
	}
	else if (Selection == "Createcharacter")
	{
		UE_LOG(LogTemp, Log, TEXT("// Playfab _ Createcharacter :: %s"), *getStringData);
		// Test _ ĳ���� ���� �Ϸ�� ���� ���� ���� ó�� 
		if (PlayerOwner)
			PlayerOwner->Blueprint_CreateCharacter();
	}
	else if (Selection == "Create_Grant")
	{
		UE_LOG(LogTemp, Log, TEXT("// Playfab _ GrantSuccess__call__getUserTitleData"));
		// �������� �������� �ְ� �κ��丮�� �ְ� Ÿ��Ʋ �����ͷ� ���� -> Ÿ��Ʋ ������ ���������� �����̵�����
		//getIngamePlayerData();
		getUserTitleData();
	}
	else if (Selection == "updateEquipmentItem")
	{
		for (auto it : InventoryProperty)
		{
			if (it.ItemInstanceId == getStringData)
			{
				UserTitleData.Add(it.ItemClass, getStringData);
				break;
			}
		}

		//������ ��� ���� �̺�Ʈ.
		UE_LOG(LogTemp, Log, TEXT("// Playfab _ updateEquipmentItem :: %s"), *getStringData);
		if (PlayerOwner)
			PlayerOwner->Blueprint_UpdateEquipmentItem(getStringData);
	}

	else if (Selection == "AddFriend")
	{
		getFriendListEvent();
	}
	else if (Selection == "RemoveFriend")
	{
		getFriendListEvent();
	}
	else if (Selection == "AddBlock")
	{
		getFriendListEvent();
	}
	else if (Selection == "BlockFriend")
	{
		getFriendListEvent();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Return Callback Event 

void UActorComponent_Playfab::SuccessLogin(bool NewlyCreated)
{
	// ù ���� ���ο� ���� ����Ʈ ������ �߰� 
	if (NewlyCreated)
	{
		UE_LOG(LogTemp, Log, TEXT("// NewlyCreated "));
		ScriptCustomNotField("UpdataDefaultValue");
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("// getUserTitleData "));
		getUserTitleData();
	}
}

void UActorComponent_Playfab::getUserTitleData(FString targetTitle)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	PlayFab::ClientModels::FGetUserDataRequest request;

	// ã���� �ϴ� Ÿ��Ʋ // ������ = ��ü Ÿ��Ʋ ������
	TArray<FString> targetKey;
	targetKey.Add(targetTitle);

	request.Keys = targetKey;
	request.PlayFabId = PlayFabID;
	// ���� ������ ã��
	ClientAPI->GetUserData(
		request,
		PlayFab::UPlayFabClientAPI::FGetUserDataDelegate::CreateLambda([&](const PlayFab::ClientModels::FGetUserDataResult& result) {

			TArray<FString> getKeys;
			result.Data.GetKeys(getKeys);
			for (auto it : getKeys)
			{
				const PlayFab::ClientModels::FUserDataRecord* record = result.Data.Find(it);
				if (record)
				{
					UserTitleData.Add(it, record->Value);
					// ĳ���� ���� ���� üũ
					if (FString(it)== FString("Createcharacter"))
					{
						if (*record->Value == FString("1"))
						{
							bCheckerCreateChracter = true;
							UE_LOG(LogTemp, Log, TEXT("// Checker Create Chracter :: %d "), true);
						}
						else
						{
							bCheckerCreateChracter = false;
							UE_LOG(LogTemp, Log, TEXT("// Checker Create Chracter :: %d "), false);
						}
					}
					// ģ�е�
					else if (it == FString("Fellowship"))
					{
						// Fellowship = record->Value;
						UE_LOG(LogTemp, Log, TEXT("// GetUserData() Fellowship :: %s "), *record->Value);
					}
					else {
						UE_LOG(LogTemp, Log, TEXT("// GetUserData() Key ( %s ) , Value ( %s ) "), *it, *record->Value);
					}
				}
			}
			// ĳ���� ���� ����
			if (PlayerOwner)
				PlayerOwner->InitPlayFabUserTitleData(bCheckerCreateChracter);

			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}
// �ΰ��� ���ӽ� �÷��̾� ������ �ε� 
// �ε� ���� �߰� �ϱ�. �׽�Ʈ ���� ������ �ӽ� �߰�
// PlayerOwner->Check_getIngameLoadingCount()
void UActorComponent_Playfab::getIngamePlayerData()
{
	// Check_getIngameLoadingCount :: ���� ���� �����͸� �ҷ����� �Լ� ���� üũ
	// Test Count :: 1
	if (PlayerOwner)
		PlayerOwner->Test_LoadingCount = 1;

	// Test_LoadingCount Check :: o
	// UserCharacterName // Playfab Display Name üũ
	getUserTitleName();

	// Test_LoadingCount Check :: x
	// �κ��丮 ����
	getInventoryList();
	// ����,����Ʈ ������ üũ
	getStatisticsEvent();
	// ����Ʈ ������ üũ
	//GetMyQuestToServer("Tutorial");
	// ���� ���� üũ
	getNoticeEvent(3);
	//ģ�� ��� ������Ʈ
	getFriendListEvent();
	// updateUserTitleData(UserTitleData);
}
// ���� �г���
void UActorComponent_Playfab::getUserTitleName()
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	PlayFab::ClientModels::FGetAccountInfoRequest request;
	request.PlayFabId = PlayFabID;

	ClientAPI->GetAccountInfo(
		request,
		PlayFab::UPlayFabClientAPI::FGetAccountInfoDelegate::CreateLambda([&](const PlayFab::ClientModels::FGetAccountInfoResult& result) {

			UserCharacterName = result.AccountInfo.Get()->TitleInfo->DisplayName;
			UE_LOG(LogTemp, Log, TEXT("// getAccountInfo titleName :: %s "), *UserCharacterName);
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("// getAccountInfo titleName :: %s "), *UserCharacterName));
			}
			if (PlayerOwner)
				PlayerOwner->Check_getIngameLoadingCount();
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}
// ������ �κ��丮 
void UActorComponent_Playfab::getInventoryList()
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	PlayFab::ClientModels::FGetUserInventoryRequest request;

	ClientAPI->GetUserInventory(
		request,
		PlayFab::UPlayFabClientAPI::FGetUserInventoryDelegate::CreateLambda([&](const PlayFab::ClientModels::FGetUserInventoryResult& result) {

			InventoryProperty.Reset();
			UE_LOG(LogTemp, Log, TEXT("// getInventoryList :: Len ( %d )"), result.Inventory.Num());

			for (auto it : result.Inventory)
			{
				FItemproperty Invendata;
				Invendata.ItemId = it.ItemId;
				Invendata.ItemInstanceId = it.ItemInstanceId;
				Invendata.ItemClass = it.ItemClass;
				Invendata.RemainingUses = it.RemainingUses;
				Invendata.UnitPrice = it.UnitPrice;

				// ������ �÷� ���� ���� ��� ������ �ʿ�.
				// ������ ���� ������ ���̺�� ������ ó�� ���? 
				/*if (it.CustomData.Contains("Color"))
				{
					FString* colorData = it.CustomData.Find("Color");
					Invendata.colorData = colorData[0];
					UE_LOG(LogTemp, Log, TEXT("// getInventoryList :: Color ( %s )"), *Invendata.colorData);
				}
				else
					UE_LOG(LogTemp, Log, TEXT("// Error : getInventoryList(id : %s ) Not Find ColorData"), *it.ItemId);*/

				// Ŀ���͸����� ���� ������ ���� ����.
				/*if (CheckUsertitleData("Body", Invendata)) {

					TTuple<FName, FName> ChangeBody(FName(*Invendata.ItemId), FName(*Invendata.colorData));
					CustomData.Body = ChangeBody;
				}
				else if (CheckUsertitleData("Eyebrow", Invendata)) {
					TTuple<FName, FName> ChangeBody(FName(*Invendata.ItemId), FName(*Invendata.colorData));
					CustomData.Eyebrow = ChangeBody;
				}
				else if (CheckUsertitleData("Eyes", Invendata)) {
					TTuple<FName, FName> ChangeBody(FName(*Invendata.ItemId), FName(*Invendata.colorData));
					CustomData.Eyes = ChangeBody;
				}
				else if (CheckUsertitleData("Hair", Invendata)) {
					TTuple<FName, FName> ChangeBody(FName(*Invendata.ItemId), FName(*Invendata.colorData));
					CustomData.Hair = ChangeBody;
				}
				else if (CheckUsertitleData("Lips", Invendata)) {
					TTuple<FName, FName> ChangeBody(FName(*Invendata.ItemId), FName(*Invendata.colorData));
					CustomData.Lips = ChangeBody;
				}*/
				// �κ��丮 ������ ���� ����Ʈ
				InventoryProperty.Add(Invendata);

				// �κ��丮 ���� ���� �߰� �Լ� ����
				// updateInventoryItem(Invendata);
			}

			if (result.VirtualCurrency.Contains("GC"))
			{
				const int* findVC = result.VirtualCurrency.Find("GC");
				if (PlayerOwner)
				{
					PlayerOwner->VirtualCoin = FString::FromInt(findVC[0]);
					PlayerOwner->updateInventory();
				}
			}

			// �ε� ī���� üũ �� ���� ���� ���� ���� Ȯ��. // �ε� ī���� ���� 
			/*if (PlayerOwner)
				PlayerOwner->Check_getIngameLoadingCount();*/
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}
// �س��� ��Ģ�� üũ�ϱ�
bool UActorComponent_Playfab::FindCheckBadName(const FString& name)
{
	TArray<FName> outputBadNameCheckRow = BadNameTable->GetRowNames();

	// �г��� ���� ���鹮�� x
	for (auto it : outputBadNameCheckRow)
	{
		FBadNameTable* data = BadNameTable->FindRow<FBadNameTable>(*it.ToString(), "");
		int32 Bad = name.Find(data->BadString);
		if (Bad >= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("// Bad String Find Point Number :: %d "), Bad);
			return true;
		}
	}
	return false;
}
// ���� �г��� ������Ʈ (�ߺ� üũ, ����)
void UActorComponent_Playfab::updateUserTitleName(const FString& DisplayName)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	PlayFab::ClientModels::FUpdateUserTitleDisplayNameRequest request;
	request.DisplayName = DisplayName;

	if (FindCheckBadName(DisplayName))
	{
		// ��Ģ�� ó�� �߰� �ϱ�.
		if (PlayerOwner)
			PlayerOwner->updateDisplayNameEvent(false);
		UE_LOG(LogTemp, Error, TEXT("// Forbidden Name :: %s "), *DisplayName);
		return;
	}

	ClientAPI->UpdateUserTitleDisplayName(
		request,
		PlayFab::UPlayFabClientAPI::FUpdateUserTitleDisplayNameDelegate::CreateLambda([&](const PlayFab::ClientModels::FUpdateUserTitleDisplayNameResult& result) {

			UserCharacterName = result.DisplayName;
			bCheckerCreateChracter = true;

			UE_LOG(LogTemp, Log, TEXT("// Success Create DisplayName :: %s "), *result.DisplayName);
			
			if (PlayerOwner)
				PlayerOwner->updateDisplayNameEvent(bCheckerCreateChracter);

			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
	);
}
// ���� ������ getStatisticsEvent
// �̼� ������ & ���(��ŷ) ������ 
void UActorComponent_Playfab::getStatisticsEvent()
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	PlayFab::ClientModels::FGetPlayerStatisticsRequest request;
	
	ClientAPI->GetPlayerStatistics(
		request,
		PlayFab::UPlayFabClientAPI::FGetPlayerStatisticsDelegate::CreateLambda([&](const PlayFab::ClientModels::FGetPlayerStatisticsResult& result) {
			// ������ ��赥���� ä���ֱ�
			for (auto it : result.Statistics)
			{
				PlayFab_Statistics.Add(it.StatisticName, it.Value);
			}
			CheckQuestInfo();
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
		);
}

// ���� ���� �������� NoticeCount ������ ���� ��
void UActorComponent_Playfab::getNoticeEvent(int NoticeCount)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();

	PlayFab::ClientModels::FGetTitleNewsRequest request;
	request.Count = NoticeCount;

	ClientAPI->GetTitleNews(
		request,
		PlayFab::UPlayFabClientAPI::FGetTitleNewsDelegate::CreateLambda([&](const PlayFab::ClientModels::FGetTitleNewsResult& result) {

			for (auto it : result.News)
			{
				// ���� �˾��� ���� ���� ó��
				// ���� // ����
				// updateNotice(it.Title, it.Body);
				UE_LOG(LogTemp, Log, TEXT("// getNotice Body :: %s "), *it.Body);
			}
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}


/////////////////////////////////Grant -> getinGamePlayerData -> level�̵�
// FunctionName : Create_Grant  == �ڽ�Ƭ �÷����տ� ���ε� 
void UActorComponent_Playfab::UploadMyCustom(const FString& FunctionName, const FString& FieldName,const TArray<int> ItemIDs)
{
	TArray<FString> FirstCostumeData;

	for (auto it : ItemIDs)
	{
		FirstCostumeData.Push(FString::FromInt(it));
	}
	// Ŀ���� ������ �ϴ� ����
	ScriptCustomArray(FunctionName, FieldName, FirstCostumeData);
}

//����Ʈ ����ü ã�� ��ã�Ҵٸ� -1
int UActorComponent_Playfab::FindQuestInfo_Index(const FString& QuestName)
{
	int Index = -1;
	for (int i = 0; i < MyQuest_Info.Num(); i++)
	{	// ���������� �ִٸ� 
		if (MyQuest_Info[i].Quest_Name == QuestName) { Index = i;	break; }
	}

	return Index;
}
// ����Ʈ PlayFab_Statistics ����, value 0 = ���۰���, value 1 = ������, value 2 = �Ϸ�
void UActorComponent_Playfab::CheckQuestInfo()
{
	TArray<FString> QuestKey;
	PlayFab_Statistics.GetKeys(QuestKey); 
	for (auto it : QuestKey)
	{	// ������������ ����Ʈ�� �߷���  value�� 1�ΰ͵鸸 == �������� ����Ʈ�� ���
		if ((it.Left(5) == "Quest") && (UserTitleData.Contains(it)) && (1 == *PlayFab_Statistics.Find(it))) 
		{
			FString val = *UserTitleData.Find(it);
			FQuest_Info Quest = MakeQuestInfo(it, val);
			MyQuest_Info.Add(Quest); //������ �־��ֱ�
		}
	}
}
// ����Ʈ JSon���� == QuestName : ParseData{ ����ܰ�+index(Key) : �ϷῩ��(Value),  ����ܰ�+index(Key) : �ϷῩ��(Value)... }
FQuest_Info UActorComponent_Playfab::MakeQuestInfo(const FString& QuestName, const FString& JsonParseData) //ex) ParseData = {"1/n0":true,"1/n1":false}
{
	//json������ ��ȯ
	FQuest_Info Quest;
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonParseData);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	TArray<FString> StepKeys;
	JsonObject->Values.GetKeys(StepKeys);


	// ����Ʈ �̸�����
	Quest.Quest_Name = QuestName;
		TArray<FString> StepParse;
	StepKeys[0].ParseIntoArray(StepParse, TEXT("/n"));
	//����Ʈ ���൵
	Quest.Quest_Step = FCString::Atoi(*StepParse[0]);
	
	//���൵ �κ� Ŭ����üũ
	int indexSize = JsonObject->Values.Num();
	
	for (int i = 0; i < indexSize; i++)
	{
		FString KeyIndex = StepParse[0] + FString("/n") + FString::FromInt(i);
		Quest.IsFinished.Add(JsonObject->Values.Find(KeyIndex)->Get()->AsBool());
	}

	return Quest;
}

UDataTable* UActorComponent_Playfab::FindQuestTable(const FString& QuestName)
{
	UGameInstance_Solaseado* MyInstance = Cast<UGameInstance_Solaseado>(GetWorld()->GetGameInstance());
	UDataTable* Quest_Table = MyInstance->GetQuestTables();
	FQuest_List* CurrQuest = Quest_Table->FindRow<FQuest_List>(FName(*QuestName), "FindQuestTable_NoQuest");
	if (CurrQuest == nullptr)	{ return nullptr;	}

	return CurrQuest->QuestData;
}
// ����Ʈ UserTitleData Update
void UActorComponent_Playfab::Quest_Update_Title(const FString& QuestName)
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	TSharedPtr<FJsonObject> JsonSubObj = MakeShareable(new FJsonObject());
	TArray< TSharedPtr<FJsonValue> > EntriesArray;

	EntriesArray.Add(MakeShareable(new FJsonValueString(QuestName))); // ����Ʈ�̸�
	int CurrQuest = FindQuestInfo_Index(QuestName);
	if (CurrQuest == -1)
	{
		UE_LOG(LogTemp, Log, TEXT("// Invalid_Struct "));
		return;
	}

	FString Step = FString::FromInt(MyQuest_Info[CurrQuest].Quest_Step);

	for (int i = 0; i < MyQuest_Info[CurrQuest].IsFinished.Num(); i++)
	{
		FString StepIndex = Step + FString("/n") + FString::FromInt(i);			// ����Ʈ ���൵ 
		JsonSubObj->SetBoolField(StepIndex, MyQuest_Info[CurrQuest].IsFinished[i]);// ����Ʈ �ϷῩ��
	}
	EntriesArray.Add(MakeShareable(new FJsonValueObject(JsonSubObj))); // ����Ʈ ����  // Key(���൵ + /n + ����Ʈindex ) :Value (�ϷῩ��)

	JsonObj->SetArrayField("Quest", EntriesArray);


	//������ ����
	PlayFab::ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = "UpdateQuest";
	request.FunctionParameter = JsonObj;
	request.GeneratePlayStreamEvent = true;
	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateLambda([&](const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
			{
				
			}),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);

}

void UActorComponent_Playfab::Quest_Update_Statistic(const FString& QuestName, enum_Quest_Update Update)
{
	const enum_Quest_Update QuestCondition = Update;
	int JsonVal = static_cast<int>(QuestCondition);


	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	TArray< TSharedPtr<FJsonValue> > EntriesArray;
	EntriesArray.Add(MakeShareable(new FJsonValueString(QuestName)));
	EntriesArray.Add(MakeShareable(new FJsonValueNumber(JsonVal)));
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	JsonObj->SetArrayField("statistic", EntriesArray);

	PlayFab::ClientModels::FExecuteCloudScriptRequest request; // ���������Ϳ� ����Ʈ Value 2�� �־��ֱ�
	request.FunctionName = "Update_Statistic";
	request.FunctionParameter = JsonObj;
	request.GeneratePlayStreamEvent = true;
	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateLambda([&, QuestName, QuestCondition](const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
			{
				FQuest_Info Quest;
				int checkIndex;
				int ENUM_Val = static_cast<int>(QuestCondition);
				switch (QuestCondition)
				{
				case enum_Quest_Update::Drop:// ����Ʈ����

					PlayFab_Statistics.Add(QuestName, ENUM_Val); // ���������� 0 ���� ����
					for (int i = 0; i < MyQuest_Info.Num(); i++) // ����Ʈ �����������
					{
						if (PlayFab_Statistics.Contains(QuestName)) { PlayFab_Statistics.Add(QuestName, 0); }
						if (MyQuest_Info[i].Quest_Name == QuestName) { MyQuest_Info.RemoveAt(i);	break; }
					}
					PlayerOwner->Quest_Drop(QuestName);
					break;


				case enum_Quest_Update::Start:	//����Ʈ ����

					PlayFab_Statistics.Add(QuestName, ENUM_Val); // ���������� �߰�
					checkIndex = FindQuestInfo_Index(QuestName);					// �ʱ�ȭ
					if (checkIndex != -1) { MyQuest_Info.RemoveAt(checkIndex); }	//�����̸��� ����Ʈ���� �ִٸ� �����
					Quest = SetQuestInfo(QuestName, 1); // ����Ʈ �������� ����ü ��Ƽ�
					MyQuest_Info.Add(Quest); // �迭�� �߰� �� 
					Quest_Update_Title(QuestName); // Ÿ��Ʋ �����Ϳ� ������Ʈ�ϱ�

					PlayerOwner->Quest_Start(Quest);
					break;

				case enum_Quest_Update::Complete:
					// ����ޱ� // ������ Ÿ��Ʋ ������ �����
					for (int i = 0; i < MyQuest_Info.Num(); i++) // ����Ʈ �����������
					{
						if (PlayFab_Statistics.Contains(QuestName)) { PlayFab_Statistics.Add(QuestName, ENUM_Val); }
						if (MyQuest_Info[i].Quest_Name == QuestName) { MyQuest_Info.RemoveAt(i);	break; }
					}
					PlayerOwner->Quest_Complete(QuestName);
					break;
				}

			}));

}

FQuest_Info UActorComponent_Playfab::SetQuestInfo(const FString& QuestName, int Step)
{
	FQuest_Info Quest;
	Quest.Quest_Name = QuestName;	// ����Ʈ �̸�
	Quest.Quest_Step = Step;		// ����Ʈ ���൵
	Quest.IsFinished.Reset();

	UDataTable* MyQuestTable = FindQuestTable(QuestName);
	TArray<int> RowNames = GetQuestRowNames(FString::FromInt(Step), MyQuestTable);
	int IndexSize = RowNames.Num();
	if (IndexSize == 0) { return Quest; } // ���൵�� ���ٸ� Quest.IsFinished.Reset() �� ��Ű�� Ż��

	for (int i = 0; i < IndexSize; i++)
	{
		Quest.IsFinished.Add(false); // ����Ʈ �ϷῩ��
	}

	//����Ʈ(���õȵ��������̺�)�� ������ �ִ� ���������̺�(��ȭ�ý���)
	FString TablePropName = FString::FromInt(RowNames[0]); // ���� ���൵�� ���� �ε����� ù��°�� ���������̺�
	FQuest_Info CurrQuestTable = *MyQuestTable->FindRow<FQuest_Info>(FName(*TablePropName), "NoData_Questinfo_Table");
	Quest.QuestTable = CurrQuestTable.QuestTable;

	return Quest;
}

//���� ���൵�� ���� ���̺� �� ���ڻ̾Ƽ� �迭�������� // ������Ƽ Quest_Step �� ���൵�� Ž�� (FQuest_Info Quest; Quest.Quest_Step;)
TArray<int> UActorComponent_Playfab::GetQuestRowNames(const FString& QuestStepProp, UDataTable* QuestTable)
{
	TArray<FString> RowNames;
	TArray<int> QuestRowNames;
	RowNames = UDataTableFunctionLibrary::GetDataTableColumnAsString(QuestTable, FName("Quest_Step"));

	for (int i = 0; i < RowNames.Num(); i++)
	{
		if (RowNames[i] == QuestStepProp)
		{
			QuestRowNames.Add(i + 1);
		}
	}
	return QuestRowNames;
}
// ����Ʈ �� ���൵���ϱ�
int UActorComponent_Playfab::QuestTotalStepcount(const FString& QuestName)
{
	UDataTable* CheckTable = FindQuestTable(QuestName);
	TArray<FName> checkRows = CheckTable->GetRowNames(); // ������� ���ؼ�
	TArray<int> Totalindex;
	for (auto it : checkRows)
	{
		FQuest_Info* checkProp = CheckTable->FindRow<FQuest_Info>(it, "TotalStepCount, Nodata");

		if (checkProp) 	{	Totalindex.AddUnique(checkProp->Quest_Step); 	}	// ������Ƽ AddUnique�� �߰� (�ߺ����ֱ̳�)
	}
	
	return Totalindex.Num();
}

void UActorComponent_Playfab::Quest_Start(const FString& QuestName)
{
	Quest_Update_Statistic(QuestName, enum_Quest_Update::Start);
}

bool UActorComponent_Playfab::Quest_Finish(const FString& QuestName, int index)
{
	int CurrQuest = FindQuestInfo_Index(QuestName);
	if (CurrQuest == -1)												{	return false;	}	// �߸��� �̸��̰ų�
	if (!MyQuest_Info[CurrQuest].IsFinished.IsValidIndex(index))	{	return false; }	// �߸��� �ε��� �����̶��

	MyQuest_Info[CurrQuest].IsFinished[index] = true;
	Quest_Update_Title(QuestName);

	for (int i = 0; i < MyQuest_Info[CurrQuest].IsFinished.Num(); i++)
	{
		if (!MyQuest_Info[CurrQuest].IsFinished[i]) { return false; } // index�� ���� true�� �ƴ϶��
	}

	//Step�� index�� ��� true���
	return true;
}

void UActorComponent_Playfab::Quest_Next(const FString& QuestName)
{
	int CurrQuest = FindQuestInfo_Index(QuestName);
	if (CurrQuest == -1)	{  return; }	// �߸��� �̸��̶�� Ż��

	int NextStep = MyQuest_Info[CurrQuest].Quest_Step + 1;

	FQuest_Info Quest;
	Quest = SetQuestInfo(QuestName, NextStep);
	// ����Ʈ�� �����ٸ�						// ����Ʈ �Ϸ� ���� ������Ʈ
	if (!Quest.IsFinished.IsValidIndex(0))		{	Quest_Complete(QuestName);	return;	}

	MyQuest_Info[CurrQuest] = Quest; //�������
	PlayerOwner->Quest_Next(MyQuest_Info[CurrQuest]);
	Quest_Update_Title(QuestName);		// Ÿ��Ʋ �����Ϳ� ������Ʈ�ϱ�
}
//����Ʈ���� == ���������� 0�� �ٲ��ֱ�
void UActorComponent_Playfab::Quest_Drop(const FString& QuestName)
{
	Quest_Update_Statistic(QuestName, enum_Quest_Update::Drop);
}
//����Ʈ�Ϸ� == ���������� 2�� �ٲ��ֱ�
void UActorComponent_Playfab::Quest_Complete(const FString& QuestName)
{
	Quest_Update_Statistic(QuestName, enum_Quest_Update::Complete);
}

bool UActorComponent_Playfab::Quest_Check_isDoing(const FString& QuestName)
{
	for (auto it : MyQuest_Info)
	{
		if (it.Quest_Name == QuestName)	{	return true;	}
	}
	return false;
}

bool UActorComponent_Playfab::Quest_Check_IsComplete(const FString& QuestName)
{
	TArray<FString> QuestKey;
	PlayFab_Statistics.GetKeys(QuestKey);
	for (auto it : QuestKey)
	{
		if ((it == QuestName) && (2 == *PlayFab_Statistics.Find(it))) 	{	return true;}	// ������������ ����Ʈ�̸��� ã�Ƽ�  value�� 2�ΰ͵鸸
	}

	//��ã���� false
	return false;
}

bool UActorComponent_Playfab::Quest_Check_IsLastFinished(const FString& QuestName)
{
	int FindQuest = FindQuestInfo_Index(QuestName);
	if (FindQuest == -1) {  return  false; }
	
	int StepTotalCount = QuestTotalStepcount(QuestName);

	if (MyQuest_Info[FindQuest].Quest_Step >= StepTotalCount) // ���൵�� �ִ����̺��� �Ѿ�� ��� �Ϸ��ߴ��� �ľ��ϱ�
	{
		for (auto it : MyQuest_Info[FindQuest].IsFinished)
		{
			if (!it) {  return false; }// �ϳ��� ����Ʈ�� �Ϸ����� ���ѻ��¶�� falseŻ��
		}
		return true; //��οϷ��ߴٸ� true Ż��
	} 

	//���൵�� �����Ѱ��
	return false;
}

enum_Quest_Condition UActorComponent_Playfab::Quest_Check_Condition(const FString& QuestName)
{
	if (Quest_Check_isDoing(QuestName))
	{
		if (Quest_Check_IsLastFinished(QuestName))//������ ����ޱ� ���� �Դ�������
		{
			return enum_Quest_Condition::LastFinished;
		}
		return enum_Quest_Condition::IsDoing;//�׳� �������϶�
	}
	else if (Quest_Check_IsComplete(QuestName))	{	return	enum_Quest_Condition::IsComplete;	} // ������� �ް� ���������϶�

	return enum_Quest_Condition::NotStart;
}


//���� ������ ������ ������Ʈ�ϱ�
void UActorComponent_Playfab::UpdateAchievement(int AchieveNumber)
{
	FString Temp;
	FString AchieveCount = FString::FromInt(AchieveNumber); // AchieveNumber = 001~999 Naming ���߱��
	for (int i = 0; i < 3 - AchieveCount.Len(); i++)
	{
		Temp += "0";
	}
	AchieveCount = Temp + AchieveCount;
	
	FString FullName = AchieveName + AchieveCount;
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());
	JsonObj->SetStringField("name", FullName);
	PlayFab::ClientModels::FExecuteCloudScriptRequest request;
	request.FunctionName = "Upload_StatisticName";
	request.FunctionParameter = JsonObj;
	request.GeneratePlayStreamEvent = true;
	ClientAPI->ExecuteCloudScript(
		request,
		UPlayFabClientAPI::FExecuteCloudScriptDelegate::CreateLambda([&, FullName, AchieveCount](const PlayFab::ClientModels::FExecuteCloudScriptResult& result)
			{
				//����������Ʈ ������ ȣ��
				PlayFab_Statistics.Add(FullName, 0);
				PlayerOwner->Finished_UpdateAchieve(AchieveCount);
			}));
			
}
//���������͸� �߷��� ��ȯ
TArray<FString> UActorComponent_Playfab::GetAchievement()
{
	TArray<FString> AchieveNumber;
	TArray<FString> Keys;
	PlayFab_Statistics.GetKeys(Keys);
	for (auto it : Keys)
	{
		if (it.Find(AchieveName) == 0) 
		{
			AchieveNumber.Add(it.Right(it.Len() - AchieveName.Len()));
		}
	}
	return AchieveNumber;
}

void UActorComponent_Playfab::Debug_ClearAchievement()
{
	TArray<FString> Keys, RemoveKeys;
	PlayFab_Statistics.GetKeys(Keys);

	for (auto it : Keys)
	{
		if (it.Find(AchieveName) == 0)
		{
			RemoveKeys.Add(it);
			//PlayFab_Statistics.Remove(it);

		}
	}
	PlayFab_Statistics.GetKeys(Keys);
	for (auto it : Keys)
	{
		UE_LOG(LogTemp, Log, TEXT("// statistics Key :: %s "), *it);
		// ����������� Ŭ�����ϴ� ��� Ȯ���غ���
	}
}

////////////////////////////////////////////////
//Friend
//ģ�� ����Ʈ ������Ʈ
void UActorComponent_Playfab::getFriendListEvent()
{
	PlayFabClientPtr ClientAPI = IPlayFabModuleInterface::Get().GetClientAPI();
	
	//ģ�� �������� �������� �ޱ� ���ؼ� �̷��� ���� ��������� �Ѵ�.
	//PlayFab�� Ÿ��Ʋ ���������� ����������Ѵ�.
	TSharedPtr<PlayFab::ClientModels::FPlayerProfileViewConstraints> ProfileConstraints(new PlayFab::ClientModels::FPlayerProfileViewConstraints);
	ProfileConstraints->ShowCreated=true;
	ProfileConstraints->ShowDisplayName = true;
	ProfileConstraints->ShowLastLogin = true;
	ProfileConstraints->ShowLocations = true;

	PlayFab::ClientModels::FGetFriendsListRequest request;
	request.ProfileConstraints = ProfileConstraints;

	
	//PlayFab::UPlayFabClientAPI::FGetFriendsListDelegate::;
	ClientAPI->GetFriendsList(
		request,
		PlayFab::UPlayFabClientAPI::FGetFriendsListDelegate::CreateUObject(this,&UActorComponent_Playfab::UpdateFriendList),
		PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UActorComponent_Playfab::ErrorScript)
				);
}

//ģ��&���� ��� ����
void UActorComponent_Playfab::UpdateFriendList(const PlayFab::ClientModels::FGetFriendsListResult& result)
{
	FriendList.Empty();
	BlockList.Empty();

	for (auto it : result.Friends)
	{
		
		if (true /* ���⿡ �ѱ����� üũ*/)
		{
			UE_LOG(LogTemp, Log, TEXT("// UpdateFriendList :: city : %s , time %d : %d "), *it.Profile->Locations[0].City, it.Profile->LastLogin.mValue.UtcNow().GetHour()+9%24, it.Profile->LastLogin.mValue.UtcNow().GetMinute());
			
			FFriendStruct sTemp;
			//������ ���� �ð��� ���� �������� ��ü�Ǿ���.
			//sTemp.LastLogin= it.Profile->LastLogin.mValue;
			sTemp.PlayFabID= it.Profile->PlayerId;
			sTemp.TitleID= it.Profile->DisplayName;

			//Tag�� �������� ģ������ �����Ѵ�.
			if (it.Tags.Num() > 0)
			{
				BlockList.Add(sTemp.TitleID, sTemp);
			}
			else
			{
				FriendList.Add(sTemp.TitleID, sTemp);
				//�α׾ƿ� �ð��� ���ϴ� �Լ�
				Cast<AGameModeBase_Solaseado>(GetWorld()->GetAuthGameMode())->HTTPRequestActor->GetUserLogoutTime(sTemp.TitleID);
			}
		}
	}

	//ģ������ ������Ʈ�� ������� ������Ʈ
	if (FriendList.IsEmpty())
	{
		UpdateFriendSlot.Broadcast();
	}
	UpdateFriend.Broadcast();
}

//�ܺ� �������Ʈ���� ȣ�����ֱ� ���ؼ�
void UActorComponent_Playfab::CallUpdateFriend()
{
	UpdateFriendSlot.Broadcast();
}

//ã�� ������ ģ����Ͽ� �ִ��� üũ�Ѵ�.
bool UActorComponent_Playfab::UpdateFriendLoginTime(FString UserID, FDateTime LoginTime)
{
	if (FriendList.Find(UserID))
	{
		FriendList.Find(UserID)->LastLogin = LoginTime;
		return true;
	}
	
	return false;

}

////////////////////////////////////////////////