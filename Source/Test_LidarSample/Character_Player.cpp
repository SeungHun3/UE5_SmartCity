// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Player.h"
#include "Widget/Widget_SpeechBubble.h"
#include "Widget/Widget_Main.h"

#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACharacter_Player::ACharacter_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 600.f;
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bInheritYaw = false;
	
	MessageBox = CreateDefaultSubobject<UWidgetComponent>(TEXT("MessageBox"));
	MessageBox->SetupAttachment(SpringArmComp);
	MessageBox->SetVisibility(false);
	MessageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACharacter_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ üũ updata Time 0.1sec
	fDalay += DeltaTime;
	if (fDalay >= 0.1f)
	{
		fDalay = 0.f;
		int fps = int(1.f / DeltaTime);
		if (MainWidget)
		{
			MainWidget->setFPS(fps);
		}
	}
}

// Called to bind functionality to input
void ACharacter_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// ���� ���� �߰�  (ä��, �ο���, ���� )
void ACharacter_Player::AddMainWidget()
{
	MainWidget = CreateWidget<UWidget_Main>(GetWorld(), MainWdigetClass);
	if (MainWidget)
	{
		MainWidget->AddToViewport();
	}
}
// ��ǳ�� (���� ä��)
void ACharacter_Player::OpenMessageBox(const FString& message)
{
	// UE_LOG(LogTemp, Log, TEXT("// Text Message Box :: %s "), *message);
	// ��ǳ�� ���� 
	if (MessageBox)
	{
		MessageBox->SetVisibility(true);
		UWidget_SpeechBubble* messageBox = Cast<UWidget_SpeechBubble>(MessageBox->GetUserWidgetObject());
		if (messageBox)
			messageBox->setSpeechBox(FText::FromString(message));

		FTimerHandle MessageTimer;
		GetWorld()->GetTimerManager().SetTimer(MessageTimer, this, &ACharacter_Player::CloseMessageBox, 0.01f, false, 2.5f);
	}
}
// ä�� ��� 
void ACharacter_Player::AddChatList(const FString& id, const FString& Message)
{
	if (MainWidget)
	{
		MainWidget->AddMessageSlot(id, Message);
	}
}

// OpenMessageBox Ÿ�̸� �Լ�
void ACharacter_Player::CloseMessageBox()
{
	if(MessageBox)
		MessageBox->SetVisibility(false);
}

// ��ũ�� �ؽ�Ʈ �ڽ�
void ACharacter_Player::OpenWorldMessageBox(const FString& message, bool bTimer)
{
	// ��ũ�� ���� 
	if (MainWidget)
		MainWidget->setMessageBox(FText::FromString(message), bTimer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���̴� �÷��̾� ���� ó��
// �ӵ� ũ�� , ĳ���� ����
void ACharacter_Player::setLiDARinformation(float vel ,float height)
{
	if (MainWidget)
		MainWidget->setLiDARPlayerInfo(vel, height);
}
// �ӵ�
void ACharacter_Player::setLiDARVelocity(float X, float Y)
{
	if (MainWidget)
		MainWidget->setLiDARVelocity(X, Y);
}
// (LiDAR ) Evnet
void ACharacter_Player::ChangeRoomEvent(uint8 ev)
{
	if (MainWidget)
	{
		MainWidget->getLiDARQuizEvent(FString::FromInt(ev));
	}
}
// (LiDAR ) Event Pause
//void ACharacter_Player::ChangeEventPause(bool ev)
//{
//	if (MainWidget)
//	{
//		MainWidget->PhotonPauseQuizEvent(ev);
//	}
//}

// ���� �ο��� üũ 
void ACharacter_Player::AddLiDARPlayerCount()
{
	ParticipantLiDAR++;
	if (MainWidget)
		MainWidget->setCurrentPlayer(ParticipantLiDAR, ParticipantClient);
}
void ACharacter_Player::AddClentPlayerCount()
{
	ParticipantClient++;
	if (MainWidget)
		MainWidget->setCurrentPlayer(ParticipantLiDAR, ParticipantClient);
}
void ACharacter_Player::RemoveLiDARPlayerCount()
{
	ParticipantLiDAR--;
	if (MainWidget)
		MainWidget->setCurrentPlayer(ParticipantLiDAR, ParticipantClient);
}
void ACharacter_Player::RemoveClentPlayerCount()
{
	ParticipantClient--;
	if (MainWidget)
		MainWidget->setCurrentPlayer(ParticipantLiDAR, ParticipantClient);
}