// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class RUNNER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	

	APickup();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class UStaticMeshComponent* Mesh;

};
