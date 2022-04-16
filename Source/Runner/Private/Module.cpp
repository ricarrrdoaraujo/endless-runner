// Fill out your copyright notice in the Description page of Project Settings.


#include "Module.h"
#include "Pickup.h"

AModule::AModule()
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	StartModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartModulePoint"));
	StartModulePoint->SetupAttachment(Root);

	EndModulePoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndModulePoint"));
	EndModulePoint->SetupAttachment(Root);

	//Spawn pickups position
	Lane0 = CreateDefaultSubobject<USceneComponent>(TEXT("Lane0"));
	Lane0->SetupAttachment(Root);

	Lane1 = CreateDefaultSubobject<USceneComponent>(TEXT("Lane1"));
	Lane1->SetupAttachment(Root);

	Lane2 = CreateDefaultSubobject<USceneComponent>(TEXT("Lane2"));
	Lane2->SetupAttachment(Root);

	PrimaryActorTick.bCanEverTick = false;

}


void AModule::BeginPlay()
{
	Super::BeginPlay();
	
	ModuleLength = FMath::Abs(EndModulePoint->GetComponentLocation().Y - StartModulePoint->GetComponentLocation().Y);
	UE_LOG(LogTemp, Warning, TEXT("ModuleLength: %f"), ModuleLength);
}


void AModule::SpawnPickups(int Difficulty)
{
	if (PickupClass == nullptr) return;

	/*
		Rules to show pickups
		Rule 0 - No pickups for first difficulty
		Rule 1 - Not always showing pickups, use a random to select randoms or not
		Rule 2 - If type is VE_TwoBridges, no randoms
		Rule 3 - If type is VE_Ground or VE_Bridge
		check next type
			 Rule 3.1 If next module type is VE_SmallLeftBridge => Lane = 0;
			 Rule 3.2 If next module type is VE_SmallCenterBridge => Lane = 1;
			 Rule 3.3 If next module type is VE_SmallRightBridge => Lane = 2;
			 Rule 3.4 If next module type is VE_Bridge or VE_Ground => Lane = Random;
	**/

	// Rule 0 No pickups for first modules
	if (Difficulty < 5) return;

	int Lane = 1;
	bool bSpawnPickup = false;


	// Rule 2. If module type is VE_TwoBridges
	if (ModuleType == EPlatformType::VE_TwoBridges)
	{
		return;
	}

	bSpawnPickup = true;

	if (!bSpawnPickup) return;

	switch (ModuleType)
	{
	case EPlatformType::VE_Ground: // Depending on next module we spawn the pickups in a specific place (to follow the path)
	case EPlatformType::VE_Bridge:

		if (NextPlatform != nullptr)
		{
			if (NextPlatform->GetModuleType() == EPlatformType::VE_SmallLeftBridge)
			{
				bSpawnPickup = true;
				Lane = 2;
			}

			if (NextPlatform->GetModuleType() == EPlatformType::VE_SmallCenterBridge)
			{
				bSpawnPickup = true;
				Lane = 1;
			}

			if (NextPlatform->GetModuleType() == EPlatformType::VE_SmallRightBridge)
			{
				bSpawnPickup = true;
				Lane = 0;
			}
			else
			{
				bSpawnPickup = true;
				Lane = FMath::RandRange(0, 2);
			}

		}
		break;
	case EPlatformType::VE_SmallCenterBridge: // Always Spawn on middle lane (1)
		bSpawnPickup = true;
		Lane = 1;
		break;
	case EPlatformType::VE_SmallLeftBridge: // Always Spawn on left lane (2)
		bSpawnPickup = true;
		Lane = 2;
		break;
	case EPlatformType::VE_SmallRightBridge: // Always Spawn on right lane (2)
		bSpawnPickup = true;
		Lane = 0;
		break;
	}


	if (bSpawnPickup)
	{
		FVector Position = Lane0->GetComponentLocation();

		if (Lane == 1)
		{
			Position = Lane1->GetComponentLocation();
		}

		if (Lane == 2)
		{
			Position = Lane2->GetComponentLocation();
		}

		// Spawn 3 pickups
		for (int i = 0; i < 3; i++)
		{
			APickup* SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClass, Position, FRotator::ZeroRotator);

			if (SpawnedPickup != nullptr)
			{
				SpawnedPickup->SetActorLocation(Position);

				SpawnedPickups.Add(SpawnedPickup);

				Position.Y -= YDistanceBetweenPickups;
			}

		}
	}
}

void AModule::DestroyModule()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PreviousPlatform = nullptr;
	NextPlatform = nullptr;
	Destroy();

	for (int i = SpawnedPickups.Num()-1; i >= 0; i--)
	{
		SpawnedPickups[i]->Destroy();
	}
}