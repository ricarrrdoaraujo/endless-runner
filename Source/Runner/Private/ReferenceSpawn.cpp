// Fill out your copyright notice in the Description page of Project Settings.


#include "ReferenceSpawn.h"

// Sets default values
AReferenceSpawn::AReferenceSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AReferenceSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReferenceSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

