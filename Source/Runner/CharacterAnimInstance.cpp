// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "RunnerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
    if (RunnerCharacter == nullptr)
    {
        RunnerCharacter = Cast<ARunnerCharacter>(TryGetPawnOwner());
    }
    if (RunnerCharacter)
    {
        //RunnerCharacter->MoveForward();
        // Get the lateral(X,Y) speed from velocity
        FVector Velocity{ RunnerCharacter->GetVelocity() };
        Velocity.Z = 0;
        Speed = Velocity.Size();

        bIsInAir = RunnerCharacter->GetCharacterMovement()->IsFalling();

        if (RunnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }
    }
}


void UCharacterAnimInstance::NativeInitializeAnimation()
{
    RunnerCharacter = Cast<ARunnerCharacter>(TryGetPawnOwner());
}
