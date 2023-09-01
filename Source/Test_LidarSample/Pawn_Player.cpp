// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Player.h"
#include "GameInstance_Solaseado.h"
#include "GameModeBase_Solaseado.h"


#include "GenericPlatform/GenericPlatformMisc.h"
#include "GameFramework/Actor.h"

#include "GameFramework/SpringArmComponent.h"

#include "ActorComponent_PlayfabStore.h"
#include "Actor_SolaseadoPhoton.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
APawn_Player::APawn_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//�ִϸ��̼� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationBPClass(TEXT("/Game/Project/Skeleton/anim/anim_body/ABP_Player.ABP_Player_C"));//����
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationBPClass(TEXT("/Game/Project/Skeleton/anim/anim_body/ABP_Player.ABP_Player_C"));	// ����
	if (AnimationBPClass.Class)	// ���� ���н� beginPlay�� ���� üũ �� set����
	{
		// Set the Animation Blueprint
		Body->SetAnimInstanceClass(AnimationBPClass.Class);
	}


	//Body->SetVisibility(false, true);
	// child���� �Ǳ� ���� ȣ���ع����� ���������� visible���� ����� ��

	// ���̷��� �޽� ���� visible����ó��
	
	TArray<USkeletalMeshComponent*> PartsMeshes;
	GetComponents<USkeletalMeshComponent>(PartsMeshes);
	for (auto Mesh : PartsMeshes)
	{
		Mesh->SetVisibility(false);
	}

	//Pawn�� ȸ���� Yaw �� controller�� ������ ���� = false
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	
}

// Called when the game starts or when spawned
void APawn_Player::BeginPlay()
{
	Super::BeginPlay();

	// �α� ��� �׽�Ʈ
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT(" // PawnBeginPlay")), true, true, FColor::Green, 10.0f);


}

// Called every frame
void APawn_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APawn_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FString APawn_Player::getLoginDeviceID()
{
	return FGenericPlatformMisc::GetMacAddressString();
}


// ���濡�� AddPlayer�� �޾ƿ� Property���� ITemIDs�� �޾ƿ�
// =====>>>>>>ĳ���� Mesh���� ���ִ� �Լ�
void APawn_Player::SetCostumeArray(const TArray<FString>& ITemIDs)
{
	//Instance�� �ִ� FindITem �Լ� �̿��ϱ�
	UGameInstance_Solaseado* MyInstance = Cast<UGameInstance_Solaseado>(GetGameInstance());

	for (auto ItemID : ITemIDs)
	{
		FCustomizing_Struct CustomStruct = MyInstance->Find_ITem(ItemID);
		if (!CustomStruct.ItemID.IsEmpty())
		{
			ChangeMesh(CustomStruct.ClassName, CustomStruct.Mesh);
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("// SetCostumeArray : Error!!  Not Found Item!!!"));
		}
	}
	

}

// UserTitleData = InstanceID  =>> ItemID�迭�� ����ִ� �Լ�
TArray<FString> APawn_Player::UploadPlayer()
{
	TArray<FString> ItemIDs;
	if (BP_ActorComponent_Playfab)
	{
		TArray<FString> TitleKeys;
		TArray<FString> TitleValue;

		// TitleKeys�� ��� Ű ä����
		BP_ActorComponent_Playfab->UserTitleData.GetKeys(TitleKeys);

		// TitleValue �� �� ����
		for (auto keys : TitleKeys)
		{
			TitleValue.Push(*BP_ActorComponent_Playfab->UserTitleData.Find(keys));
			
		}
		// �ϴ� ��� value �̾Ƴ���
		// ������ ��
		for (auto propertys : BP_ActorComponent_Playfab->getInventoryItemList())
		{
			for (auto it : TitleValue)
			{
				if (it == propertys.ItemInstanceId)
				{
					// ���� instanceID���� üũ �� �迭�� ����
					//UE_LOG(LogTemp, Log, TEXT("// instanceID : %s , ItemID : %s "), *it, *propertys.ItemId);
					ItemIDs.Push(propertys.ItemId);
				}
			}

		}
	}

	return ItemIDs;
}

// �޽� ���� ��Ű�� �Լ� 
void APawn_Player::ChangeMesh(const FString& ClassName, USkeletalMesh* Mesh)
{
	TArray<USkeletalMeshComponent*> MeshComponents;
	GetComponents<USkeletalMeshComponent>(MeshComponents);
	for (auto Parts : MeshComponents)
	{
		FString PartsName;
		PartsName = Parts->GetName();
		//UE_LOG(LogTemp, Log, TEXT("// Name :  %s "), *PartsName);
		if (PartsName == ClassName)
		{
			Parts->SetSkeletalMesh(Mesh);
		}
	}
}

// ó�����۽� PlayFab�����Ͱ� ���� ��� default�� �޽� ä���ִ� �Լ�
void APawn_Player::BeginDefalutMesh()
{

	//Instance�� �ִ� ������ ���̺� ���������
	UGameInstance_Solaseado* MyInstance = Cast<UGameInstance_Solaseado>(GetGameInstance());
	TArray<UDataTable*>InstanceDataTables = MyInstance->GetDataTables();


	if (!InstanceDataTables.IsValidIndex(0)) // ������ ���̺��� ���ٸ� Ż��
	{
		//UE_LOG(LogTemp, Warning, TEXT("// Nodata !!!!"));
		return;
	}
	// ������ ���̺��� �ִٸ�
	for (auto Table : InstanceDataTables)
	{
		//�޾ƿ� ������ ���̺� �迭���� �� ���̸� String "1" �� �����͸� �޾ƿ���
		FCustomizing_Struct CustomStruct = *Table->FindRow<FCustomizing_Struct>("1", "");
		if (!CustomStruct.ItemID.IsEmpty())
		{
			//�����Ͱ� ����� ������ Ȯ�� �� changeMesh �Լ� ����
			ChangeMesh(CustomStruct.ClassName, CustomStruct.Mesh);
		}
	}
	// ������ ���� ����� �������ѱ�
	Body->SetVisibility(true, true);
	
}

void APawn_Player::ChangeProperty(const FString& ITemID)
{
	AGameModeBase_Solaseado* GM_Solaseado = Cast<AGameModeBase_Solaseado>(GetWorld()->GetAuthGameMode());
	//GM_Solaseado->PhotonCloud->InputCharacterInfo("Pawn", ITemID); 
	GM_Solaseado->PhotonCloud->SendCostumeParts(ITemID);
	 
}

float APawn_Player::ZoomFunction(bool isZoomIn, float zoomAxis)
{
	if (!BP_SpringArm)
	{
		return BP_SpringArm->TargetArmLength;
	}


	float zoomValue = BP_SpringArm->TargetArmLength;
	// ���� : -Axis , �ܾƿ�  + Axis; 
	isZoomIn ? zoomValue -= zoomAxis : zoomValue += zoomAxis;
	

	// ����� ���� SpringArm�� ����
	BP_SpringArm->TargetArmLength = FMath::Clamp(zoomValue, 50.0f, 500.0f);


	return BP_SpringArm->TargetArmLength;
}



///  CostumePawn    //////////////////////////
///////////////////////////////////////////
bool APawn_Player::Bind_Init_Implementation(UWidget_CustomizingTab* WB_Customizing_Tab)
{
	return false;
}

bool APawn_Player::Select_Position_Implementation(int ClassName)
{
	return false;
}

void APawn_Player::Change_Scene_Implementation(int TabNumber)
{
	
}

//ģ�� ��û ������Ʈ
void APawn_Player::UpdateFriendRequests(FString PlayfabID, FString PlayName)
{
	FFriendStruct FriendRequest;
	FriendRequest.PlayFabID = PlayfabID;
	FriendRequest.TitleID = PlayName;

	if (BP_ActorComponent_Playfab->FriendList.Find(PlayName)==nullptr)
	{
		BP_ActorComponent_Playfab->FriendRequestList.Add(PlayName, FriendRequest);
		//�÷��̾� �˶� �߰� ���� �� ���� ����
	}
}
