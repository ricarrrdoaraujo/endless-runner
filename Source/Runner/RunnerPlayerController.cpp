// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerPlayerController.h"
#include "RunnerCharacter.h" 
#include "GameFramework/SpringArmComponent.h"
#include "InGameUserUI.h"

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
    
    CoinsCollected = 0;
    CurrentDistance = 0;

    APawn* const pawn = GetPawn();
    if (pawn != nullptr)
    {
        MyCharacter = Cast<ARunnerCharacter>(pawn);
    }

    if (InGameUIClass != nullptr)
    {
        InGameUI = CreateWidget<UInGameUserUI>(this, InGameUIClass);

        if (InGameUI != nullptr)
        {
            InGameUI->AddToViewport();

            InGameUI->UpdateCoins(CoinsCollected);
            InGameUI->UpdateDistance(CurrentDistance);
            InGameUI->UpdateLives(CurrentLives);
        }
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

    if (bIsRunning)
    {
        UE_LOG(LogTemp, Warning, TEXT("bIsRunning"));
        CurrentDistance += DeltaTime;

        if (InGameUI != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Distance Up"));
            InGameUI->UpdateDistance(CurrentDistance);
        }
    }

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

        if (CurrentLane == 1)
        {
            MyCharacter->GetCameraBoom()->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
            
        }
        else if (CurrentLane == 2)
        {
            MyCharacter->GetCameraBoom()->SetRelativeRotation(FRotator(-20.f, -5.f, 0.f));
        }
        else if (CurrentLane == 0)
        {
            MyCharacter->GetCameraBoom()->SetRelativeRotation(FRotator(-20.f, 5.f, 0.f));
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
    CurrentDistance = 0.0f;

    if (InGameUI != nullptr)
    {
        InGameUI->UpdateCoins(CoinsCollected);
        InGameUI->UpdateDistance(CurrentDistance);
    }
}

void ARunnerPlayerController::TakeLive(bool& isLastLive)
{
    bool IsLastLive = false;
    if ((CurrentLives - 1) < 0)
    {
        CurrentLives = 0;
        IsLastLive = true;
    }
    else
    {
        CurrentLives -= 1;
    }

    if (InGameUI != nullptr)
    {
        InGameUI->UpdateLives(CurrentLives);
    }
}

void ARunnerPlayerController::SetInitialLives(int32 InitialLives)
{
    CurrentLives = InitialLives;
}

void ARunnerPlayerController::OnCollectCoin()
{
    CoinsCollected += 1;

    if (InGameUI != nullptr)
    {
        InGameUI->UpdateCoins(CoinsCollected);
    }
}