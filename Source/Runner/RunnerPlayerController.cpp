// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPlayerController.h"
#include "RunnerCharacter.h" 

ARunnerPlayerController::ARunnerPlayerController()
{
}

void ARunnerPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bCanMove = false;
    bIsRunning = false;
    bLockMovement = true;
    CurrentLane = 1; //start in middle lane

    APawn* const pawn = GetPawn();
    if (pawn != nullptr)
    {
        MyCharacter = Cast<ARunnerCharacter>(pawn);
    }

    StartRunning();
}

void ARunnerPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bLockMovement)
    {
        CurrentLockMovementTime += DeltaTime;

        if (CurrentLockMovementTime >= 0.2f)
        {
            CurrentLockMovementTime = 0.0f;
            bLockMovement = false;
        }
    }

    if (!bCanMove) return;

    if (bIsJumping)
    {
        RemainingJumpTime -= DeltaTime;
        if (RemainingJumpTime <= 0.0f)
        {
            bIsJumping = false;

            if (MyCharacter != nullptr)
            {
                MyCharacter->StopJumping();
            }
        }
    }
}

void ARunnerPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAxis("MoveRight", this, &ARunnerPlayerController::MoveRight);

    InputComponent->BindAction("Jump", IE_Pressed, this, &ARunnerPlayerController::Jump);
}

void ARunnerPlayerController::MoveRight(float Value)
{
    if ((bLockMovement) || (!bCanMove)) return;

    if (MyCharacter != nullptr)
    {
        if (Value < 0.0f)
        {
            if ((CurrentLane + 1) < 3)
            {
                
                bLockMovement = true;
                CurrentLockMovementTime = 0.0f;

                CurrentLane += 1;
                MyCharacter->MoveToRight();

                CurrentLane = FMath::Clamp(CurrentLane, 0, 2);
            }
        }

        if (Value > 0.0f)
        {
            if ((CurrentLane - 1) >= 0)
            {
                bLockMovement = true;
                CurrentLockMovementTime = 0.0f;

                MyCharacter->MoveToLeft();

                CurrentLane -= 1;
                CurrentLane = FMath::Clamp(CurrentLane, 0, 2);
            }
        }
    }
}

void ARunnerPlayerController::Jump()
{
    if (!bCanMove) return;
    if (bIsJumping) return;

    if (MyCharacter != nullptr)
    {
        bIsJumping = true;
        RemainingJumpTime = 1.2f;
        MyCharacter->Jump();
    }
}

void ARunnerPlayerController::StartRunning()
{
    bCanMove = true;
    bIsRunning = true;
    if (MyCharacter != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartRunning"));
        MyCharacter->SetIsMoving(true);
    }
}

void ARunnerPlayerController::StopRunning()
{
    MyCharacter->SetIsMoving(false);
    bCanMove = false;
    bIsRunning = false;
    bLockMovement = true;
}

void ARunnerPlayerController::Respawn()
{
    CurrentLane = 1; // Set current lane to the middle one
    bIsRunning = false;
}