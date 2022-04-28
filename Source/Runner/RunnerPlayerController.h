// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RunnerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API ARunnerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARunnerPlayerController();

	virtual void BeginPlay() override;

protected:

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

protected:

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void Jump();

	class ARunnerCharacter* MyCharacter;

	bool bLockMovement;
	float CurrentLockMovementTime;
	bool bCanMove = false;

private:
	bool bIsRunning = false;
	int CurrentLane;
	float CurrentDistance;

	int CoinsCollected;
	int CurrentLives;

private:
	bool bIsJumping;
	float RemainingJumpTime;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Player UI")
	class UInGameUserUI* InGameUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player UI")
	TSubclassOf<class UUserWidget> InGameUIClass;

	UPROPERTY(BlueprintReadWrite, Category = "Player UI")
	class UInitialGameWidget* StartGameUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player UI")
	TSubclassOf<class UUserWidget> StartGameUIClass;

public:

	FORCEINLINE UInGameUserUI* GetUI() { return InGameUI; };

	UFUNCTION(BlueprintCallable, Category = "Player Events")
	void OnCollectCoin();

public:

	UFUNCTION(BlueprintCallable, Category = "Player Events")
	void StartRunning();

	UFUNCTION(BlueprintCallable, Category = "Player Events")
	void StopRunning();

	void Respawn();

	void TakeLive(bool& isLastLive);

	void SetInitialLives(int32 InitialLives);
};
