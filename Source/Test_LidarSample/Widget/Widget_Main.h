// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Main.generated.h"

struct FLiDARQuizData;

UCLASS()
class TEST_LIDARSAMPLE_API UWidget_Main : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_Message;	// 36�� ����
	UPROPERTY(meta = (BindWidget))
		class UOverlay* Overlay_WorldMessage;
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* ScrollBox_MessageSlot;

	// �޴� ��ư �̺�Ʈ
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Camera;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Event_End;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_Event_Start;
	UPROPERTY(meta = (BindWidget))
		class UButton* Button_AutoMove;
	
	// �ο���
	UPROPERTY(meta = (BindWidget))
		class UOverlay* Overlay_Player;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* TextBlock_CurrentPlayer_LiDAR;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* TextBlock_CurrentPlayer_Client;
	//UPROPERTY(meta = (BindWidget))
	//	class UTextBlock* TextBlock_CurrentPlayer_Total;

	// �ο��� ���ε� 
	UPROPERTY(BlueprintReadOnly)
		float CurrentPlayer_LiDAR = 0.f;
	UPROPERTY(BlueprintReadOnly)
		float CurrentPlayer_Client = 0.f;
	UPROPERTY(BlueprintReadOnly)
		float CurrentPlayer_Total = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ProgressMaxCnt = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ProgressVelocityMaxCnt = 1.f;

	// �÷��̾� ������ ��� ��ġ ( �ӵ� , ���� ) // ����Ŭ ���α׷��� �� ����
	/*UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProgressBar_Velocity;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ProgressBar_Height;*/

	// Ÿ�̸� 
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock_Timer;
	UPROPERTY(meta = (BindWidget))
		class UHorizontalBox* HorizontalBox_Timer;
	// FPS 
	UPROPERTY(BlueprintReadWrite)
		float GameFPS = 0.f;

	// Ÿ�̸� ī��Ʈ
	UPROPERTY(EditAnywhere)
		int MessageTimer = 20;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UWidget_MessageSlot> SpawnSlot;

protected:
	virtual void NativeConstruct() override;

	// ����, ä�� ���� �޼��� Ŭ����
	UFUNCTION()
		void CloseMessageBox();
	UFUNCTION()
		void ClearTimer();

	FTimerHandle TextTimer;
	int TempMessageTimer;

	FLiDARQuizData* CurrentQuizData;

	// fps widget Checker 
	uint8 Checker_Fps = 0;
	
public:
	// �޼��� ���
	UFUNCTION(BlueprintCallable)
		void setMessageBox(const FText& message, bool bTimer = false);
	// �޼��� ��� Ÿ�̸� �Ͻ�����
	UFUNCTION(BlueprintCallable)
		void PauseMessageTimer(bool bPause);

	// �÷��̾� ������ ��� ��ġ ( �ӵ� , ���� ) ���ε� �Լ�
	UFUNCTION(BlueprintImplementableEvent)
		void setCircleProgress_Height(float height);
	UFUNCTION(BlueprintImplementableEvent)
		void setCircleProgress_VelocityX(float x);
	UFUNCTION(BlueprintImplementableEvent)
		void setCircleProgress_VelocityY(float y);

	// (LiDAR ) Quiz Event 
	UFUNCTION(BlueprintImplementableEvent)
		void PhotonEndQuizEvent();
	UFUNCTION(BlueprintImplementableEvent)
		void PhotonPauseQuizEvent(bool ev);
	UFUNCTION(BlueprintImplementableEvent)
		void PhotonSelectQuizEvent(const FString& k, const FString& v1, const FString& v2);

	// ä�� ����Ʈ �߰� 
	void AddMessageSlot(const FString& id, const FString& Message);

	// ä�� ����Ʈ Ŭ����
	UFUNCTION(BlueprintCallable)
		void ClearMessageSlot();

	// fps Checker widget Func
	UFUNCTION(BlueprintImplementableEvent)
		void ChangeFpsWidget(uint8 Fps);

	// Ÿ�̸� �޼��� ��� ���� �˸�
	UFUNCTION(BlueprintImplementableEvent)
		void OnClearTimerEvent();

	// Ÿ�̸� �޼��� ���
	void setMessageTimer();

	// ���� �ο�
	void setCurrentPlayer(uint8 lidar = 0, uint8 client = 0);

	// ( LiDAR ) �÷��̾� ���� ��� �� 
	void setLiDARPlayerInfo(float vel, float height);
	void setLiDARVelocity(float x, float y);

	// ���� ������ ǥ��
	void setFPS(int fps);

	// ( LiDAR ) ���� ���� ���� �ȳ� �޼���
	void startLiDARQuizEvent(int timer);

	// ( LiDAR ) ���� o,x ���� ��� // ���� ��� �� Ÿ�̸� ���
	void getLiDARQuizEvent(const FString& number);

	// ( LiDAR ) ���� ���� ���� �ȳ� �޼���
	void EndLiDARQuizEvent();

};
