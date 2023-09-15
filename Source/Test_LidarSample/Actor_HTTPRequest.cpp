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

//DB에 해당 유저의 로그아웃 시간을 요청한다.
void AActor_HTTPRequest::GetUserLogoutTime(FString UserID)
{
	FString HTTP=FString("https://www.m6sola.o-r.kr/Photon/GetLogoutTime.php?UserID=") + UserID;
	HttpCall(HTTP);
	++HttpCallCount;
}

//HTTP링크에 요청
//현재 GET사용 -> 후에 POST로 변경 예정
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

//HTTP 콜백 함수
void AActor_HTTPRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString str = Response->GetContentAsString();
	str=str.TrimStartAndEnd();
	UE_LOG(LogTemp, Log, TEXT("// OnResponseReceived :: %s"), *str);

	//여기에 스위치로 HTTP요청을 어떻게 할지 정할 수 있지만 일단 친구창에만 쓰이므로 하나만 둔다.
	GetLastLogoutTime(str);
}

//받은 데이터를 파싱해서 남은 로그아웃 시간을 구한다.
FString AActor_HTTPRequest::sParsingHTTP(FString ExtractedHttp, FString InputString)
{
	FString Substring = "";
	//찾는 문장의 Index 위치를 찾는다.
	int strIndex = ExtractedHttp.Find(InputString, ESearchCase::CaseSensitive, ESearchDir::FromStart, 0);
	if (strIndex >= 0)
	{
		// "LastLogoutTime:" 다음의 문자열을 추출합니다.
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

//로그아웃 시간 파싱함수
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
		//여기서 친구목록에 로그아웃 시간을 갱신시켜준다.
		SettingFriend(UserID, DateTime);
		return true;
	}
	
	return false;
}