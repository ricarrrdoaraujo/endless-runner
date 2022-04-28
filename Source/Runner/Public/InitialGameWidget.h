// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InitialGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class RUNNER_API UInitialGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Events")
	void OnInitializeGame();

	/*UFUNCTION(BlueprintCallable)
	void RemoveInitialWidget();*/
	
};
