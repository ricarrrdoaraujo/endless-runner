// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "RoundGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API ARoundGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	//ARoundGameModeBase();

	virtual void StartPlay() override;

private:

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

protected:

	FString RoundSessionId;

public:

	FORCEINLINE void SetRoundSessionId(FString SessionId) { RoundSessionId = SessionId; };
	FORCEINLINE FString GetRoundSessionId() { return RoundSessionId; };
};
