// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	//virtual void BeginPlay() override;

protected:
	/*UPROPERTY(BlueprintReadWrite, Category = "Player UI")
	class UInGameUserUI* InGameUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player UI")
	TSubclassOf<class UUserWidget> InGameUIClass;*/

	UPROPERTY(BlueprintReadWrite, Category = "Player UI")
	class UInitialGameWidget* StartGameUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player UI")
	TSubclassOf<class UUserWidget> StartGameUIClass;

public:
	//UFUNCTION(BlueprintCallable)
	//void RemoveInitialWidget();

};
