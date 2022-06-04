// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "Module.h"
#include "Definitions.h"
#include "RoundGameMode.generated.h"

// Struct containing the platforms to spawn
USTRUCT(BlueprintType)
struct FProbabilityTable
{
	GENERATED_USTRUCT_BODY()

		FProbabilityTable()
	{
		PlatformType = EPlatformType::VE_Ground;

		Probability = 20.0f;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	EPlatformType PlatformType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	TSubclassOf<class AModule> PlatformClass;


	//Add weighted  probability for each platform
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	float Probability;
};
/**
 * 
 */
UCLASS()
class RUNNER_API ARoundGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ARoundGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Game Controller Settings")
	TArray<FProbabilityTable> PlatformTable;

	UPROPERTY(EditDefaultsOnly, Category = "Game Controller Settings")
	int VisibleModuleNumber = 7;

	UPROPERTY(EditDefaultsOnly, Category = "Game Controller Settings")
	int PlayerLives = 3;

private:

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

protected:

	virtual void StartPlay() override;

	void InitializeGame();

	FString RoundSessionId;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EGameState GameState = EGameState::VE_None;

	/*UPROPERTY(EditInstanceOnly, Category = "Game Controller Settings")
	AActor* FloorPlane;*/

	UPROPERTY(EditAnywhere, Category = "Spawn Actor")
	class AReferenceSpawn* ReferenceToSpawn;

private:

	int PlatformCount = 0;

	class AModule* PreviousPlatform;

	class AModule* PlatformPlayerToCheck;

	class AModule* FirstPlatform;

	class ARunnerPlayerController* PlayerController;

	class ARunnerCharacter* RunnerCharacter;

	void SpawnNewPlatform();

	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION()
	void OnRespawn();

	FTimerHandle RespawnTimerHandle;

	// Gets a platform according to the previous one and weighthed platforms
	EPlatformType GetPlatformTypeToSpawn(const EPlatformType& PlatformType);

	TSubclassOf<class AModule> GetModuleByType(const EPlatformType& PreviousType);

public:

	FORCEINLINE void SetRoundSessionId(FString SessionId) { RoundSessionId = SessionId; };
	FORCEINLINE FString GetRoundSessionId() { return RoundSessionId; };
};
