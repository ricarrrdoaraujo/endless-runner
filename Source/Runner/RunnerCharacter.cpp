// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; //do not pull camera when collides with level

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	bIsMoving = false;

	GetCharacterMovement()->MaxWalkSpeed = 400;
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	// if (true)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// UE_LOG(LogTemp, Warning, TEXT("AddMovementInput"));
		AddMovementInput(Direction, 1.0f);
	}
}

void ARunnerCharacter::MoveToRight()
{
	if (!bIsMoving) return;
	FVector CurrentAct = GetActorLocation();
	CurrentAct.X = CurrentAct.X - 180.0;

	SetActorLocation(CurrentAct);
}

void ARunnerCharacter::MoveToLeft()
{
	if (!bIsMoving) return;
	FVector CurrentAct = GetActorLocation();
	CurrentAct.X = CurrentAct.X + 180.0;

	SetActorLocation(CurrentAct);
}

void ARunnerCharacter::Respawn(FVector Position)
{
	SetActorLocation(Position);

	bIsMoving = false;
}