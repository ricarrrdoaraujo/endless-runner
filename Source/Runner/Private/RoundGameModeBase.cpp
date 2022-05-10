// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundGameModeBase.h"
#include "Json.h"

void ARoundGameModeBase::StartPlay()
{
	Super::StartPlay();

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ARoundGameModeBase::OnResponseReceived);
	Request->SetURL("https://us-central1-endless-runner-rsn-1.cloudfunctions.net/roundStart");
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

void ARoundGameModeBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);
	
	if (bConnectedSuccessfully)
	{
		//UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());
		UE_LOG(LogTemp, Display, TEXT("Session ID: %s"), *ResponseObj->GetStringField("roundSessionId"));
		SetRoundSessionId(*ResponseObj->GetStringField("roundSessionId"));
	}
}
