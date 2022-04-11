// Fill out your copyright notice in the Description page of Project Settings.


#include "Module.h"

AModule::AModule()
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	StartModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartModulePoint"));
	StartModulePoint->SetupAttachment(Root);

	EndModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndModulePoint"));
	EndModulePoint->SetupAttachment(Root);

	PrimaryActorTick.bCanEverTick = false;

}


void AModule::BeginPlay()
{
	Super::BeginPlay();
	
	ModuleLength = FMath::Abs(EndModulePoint->GetComponentLocation().Y - StartModulePoint->GetComponentLocation().Y);
	UE_LOG(LogTemp, Warning, TEXT("ModuleLength: %f"), ModuleLength);
}


void AModule::DestroyModule()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PreviousPlatform = nullptr;
	NextPlatform = nullptr;
	Destroy();

}