// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReferenceSpawn.generated.h"

UCLASS()
class RUNNER_API AReferenceSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReferenceSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "Game Controller Settings")
	AActor* FloorPlane;

	UPROPERTY(EditInstanceOnly, Category = "Game Controller Settings")
	AActor* SpawnOrigin;

	FORCEINLINE AActor* GetFloorPlane() { return FloorPlane; };
	FORCEINLINE AActor* GetSpawnOrigin() { return SpawnOrigin; };

};
