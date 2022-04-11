// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Definitions.h"
#include "Module.generated.h"

UCLASS()
class RUNNER_API AModule : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModule();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Module")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Module")
	USceneComponent* StartModulePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Module")
	USceneComponent* EndModulePoint;

	UPROPERTY(EditDefaultsOnly, Category = "Module Settings")
	EPlatformType ModuleType;

	UPROPERTY(EditDefaultsOnly, Category = "Module Settings")
	float ModuleLength = 20.0f;

public:
	FORCEINLINE float GetModuleLength() {return ModuleLength;}

	FORCEINLINE EPlatformType GetModuleType() {return ModuleType;}

	FORCEINLINE AModule* GetNext() {return NextPlatform;}

	FORCEINLINE void SetNext(AModule* Value) {NextPlatform = Value;}

	FORCEINLINE AModule* GetPrevious() {return PreviousPlatform;}

	FORCEINLINE void SetPrevious(AModule* Value) {PreviousPlatform = Value;}

protected:

	AModule* PreviousPlatform = nullptr;

	AModule* NextPlatform = nullptr;

public:

	void DestroyModule();
};
