// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class RUNNER_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

public:

	void MoveToRight();
	void MoveToLeft();

	private:
	bool bIsMoving = false;

	void Respawn(FVector Position);

protected:

	FVector StartLocation;

public:
	FORCEINLINE void SetIsMoving(bool Value) { bIsMoving = Value; };

	FORCEINLINE USpringArmComponent* GetCameraBoom() { return CameraBoom; };
};
