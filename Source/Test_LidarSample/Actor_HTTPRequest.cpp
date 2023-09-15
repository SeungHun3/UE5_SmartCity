// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_HTTPRequest.h"

// Sets default values
AActor_HTTPRequest::AActor_HTTPRequest(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HttpCallCount = 0;
	Http = &FHttpModule::Get();
}

//DB�� �ش� ������ �α׾ƿ� �ð��� ��û�Ѵ�.
void AActor_HTTPRequest::GetUserLogoutTime(FString UserID)
{
	FString HTTP=FString("https://www.m6sola.o-r.kr/Photon/GetLogoutTime.php?UserID=") + UserID;
	HttpCall(HTTP);
	++HttpCallCount;
}

//HTTP��ũ�� ��û
//���� GET��� -> �Ŀ� POST�� ���� ����
void AActor_HTTPRequest::HttpCall(const FString &HttpLink)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AActor_HTTPRequest::OnResponseReceived);
	Request->SetURL(HttpLink);
	Request->SetVerb("GET");
	//Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	//
	Request->SetHeader("Content-Type", TEXT("text/html"));
	Request->ProcessRequest();
}

// Called when the game starts or when spawned
void AActor_HTTPRequest::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AActor_HTTPRequest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//HTTP �ݹ� �Լ�
void AActor_HTTPRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString str = Response->GetContentAsString();
	str=str.TrimStartAndEnd();
	UE_LOG(LogTemp, Log, TEXT("// OnResponseReceived :: %s"), *str);

	//���⿡ ����ġ�� HTTP��û�� ��� ���� ���� �� ������ �ϴ� ģ��â���� ���̹Ƿ� �ϳ��� �д�.
	GetLastLogoutTime(str);
}

//���� �����͸� �Ľ��ؼ� ���� �α׾ƿ� �ð��� ���Ѵ�.
FString AActor_HTTPRequest::sParsingHTTP(FString ExtractedHttp, FString InputString)
{
	FString Substring = "";
	//ã�� ������ Index ��ġ�� ã�´�.
	int strIndex = ExtractedHttp.Find(InputString, ESearchCase::CaseSensitive, ESearchDir::FromStart, 0);
	if (strIndex >= 0)
	{
		// "LastLogoutTime:" ������ ���ڿ��� �����մϴ�.
		Substring = ExtractedHttp.RightChop(strIndex + InputString.Len());

		int BreakIndex = 0;
		if (Substring.FindChar(TEXT('<'), BreakIndex))
		{
			Substring = Substring.Left(BreakIndex);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("// error :: "));
			return "error";
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("// error :: "));
		return "error";
	}
	return Substring;
}

//�α׾ƿ� �ð� �Ľ��Լ�
bool AActor_HTTPRequest::GetLastLogoutTime(FString ExtractedHttp)
{
	
	FString strLastLoing = "LastLogoutTime : ";

	//////
	FString Substring="";
	Substring = sParsingHTTP(ExtractedHttp, "LastLogoutTime : ");
	if (Substring == "error")
	{
		return false;
	}
	Substring = Substring.Replace(TEXT(" "), TEXT("T"));


	FString UserID = "";
	UserID = sParsingHTTP(ExtractedHttp, "UserID : ");
	if (UserID == "error")
	{
		return false;
	}

	FDateTime DateTime;
	if (FDateTime::ParseIso8601(*Substring, DateTime))
	{
		//���⼭ ģ����Ͽ� �α׾ƿ� �ð��� ���Ž����ش�.
		SettingFriend(UserID, DateTime);
		return true;
	}
	
	return false;
}