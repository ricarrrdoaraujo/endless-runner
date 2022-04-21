// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUserUI.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UInGameUserUI : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoinsCounter;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DistanceCounter;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LivesCounter;

public: 

	void UpdateCoins(int32 Coins);

	void UpdateDistance(float Distance);

	void UpdateLives(int32 Lives);

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
	void OnInitializeGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
	void OnStartGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
	void OnGameOver();
	
};
