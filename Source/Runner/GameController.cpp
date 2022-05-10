// Fill out your copyright notice in the Description page of Project Settings.


#include "GameController.h"
#include "RunnerPlayerController.h"
#include "RunnerCharacter.h"
#include "GameFramework/Actor.h"
#include "InGameUserUI.h"

AGameController::AGameController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameController::BeginPlay()
{
	Super::BeginPlay();

	//FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	//Request->OnProcessRequestComplete().BindUObject(this, &AGameController::OnResponseReceived);
	//Request->SetURL("https://us-central1-endless-runner-rsn-1.cloudfunctions.net/roundStart");
	//Request->SetVerb("GET");
	//Request->ProcessRequest();

	//GameState = EGameState::VE_InitializeGame;
	GameState = EGameState::VE_PreparePlatform;

	PlayerController = Cast<ARunnerPlayerController>(GetWorld()->GetFirstPlayerController());

	//OpenGame();
	StartSession();
	
}

//void AGameController::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
//{
//	UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());
//	if (bConnectedSuccessfully)
//	{
//		GameState = EGameState::VE_PreparePlatform;
//
//		PlayerController = Cast<ARunnerPlayerController>(GetWorld()->GetFirstPlayerController());
//
//		StartSession();
//	}
//}

void AGameController::StartSession()
{
	if (PlayerController != nullptr)
	{
		RunnerCharacter = Cast<ARunnerCharacter>(PlayerController->GetCharacter());
	}

	// Initialize platforms
	InitializeGame();

	PlayerController->SetInitialLives(PlayerLives);

	GameState = EGameState::VE_PrepareGame;

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AGameController::OnRespawn, 0.1f, false);
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RunnerCharacter == nullptr) return;

	if (GameState != EGameState::VE_Running) return;

	// Move the floor with the Actor
	if (FloorPlane != nullptr)
	{
		FVector FloorPosition = FloorPlane->GetActorLocation();
		FloorPosition.Y = RunnerCharacter->GetActorLocation().Y;

		FloorPlane->SetActorLocation(FloorPosition);

		if (RunnerCharacter->GetActorLocation().Z <= FloorPosition.Z)
		{
			PlayerController->StopRunning();

			bool LastLive = false;
			PlayerController->TakeLive(LastLive);

			if (LastLive)
			{
				GameState = EGameState::VE_GameOver;

				if (PlayerController->GetUI() != nullptr)
				{
					PlayerController->GetUI()->OnGameOver();
				}
			}
			else 
			{
				GameState = EGameState::VE_RemovePlatforms;
				GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AGameController::OnRespawn, 1.0f, false);
			}
		}
	}

	if (PlatformPlayerToCheck != nullptr)
	{
		if (RunnerCharacter->GetActorLocation().Y > PlatformPlayerToCheck->GetActorLocation().Y)
		{

			// Next is the next one to check
			PlatformPlayerToCheck = PlatformPlayerToCheck->GetNext();

			if (FirstPlatform != nullptr)
			{
				AModule* TempFirstToRemove = FirstPlatform->GetNext();

				// Destroy first
				FirstPlatform->Destroy();

				FirstPlatform = TempFirstToRemove;
			}

			// Add new module
			SpawnNewPlatform();
		}
	}
}

void AGameController::InitializeGame()
{
	PreviousPlatform = nullptr;

	PlatformCount = 0;

	FVector Position = GetActorLocation();

	for (int i = 0; i < VisibleModuleNumber; i++)
	{
		AModule* SpawnedPlatform = nullptr;

		if (PlatformCount <= 3) // First platforms we only spawn Ground or Bridge
		{
			float RandomPlatformToSpawn = FMath::RandRange(0.0f, 100.0f);

			EPlatformType PlatformType = EPlatformType::VE_Ground;
			// Select any of the normal ones
			if (RandomPlatformToSpawn > 50.0f)
			{
				PlatformType = EPlatformType::VE_Bridge;
			}

			TSubclassOf<AModule> PlatformToSpawn = GetModuleByType(PlatformType);
			SpawnedPlatform = GetWorld()->SpawnActor<AModule>(PlatformToSpawn, Position, FRotator::ZeroRotator);
		}
		else
		{
			// Next platforms, use the module platform
			if (PreviousPlatform != nullptr)
			{
				// Next type to spawn: Depending on previous platform spawned
				EPlatformType NewType = GetPlatformTypeToSpawn(PreviousPlatform->GetModuleType());
				TSubclassOf<AModule> PlatformToSpawn = GetModuleByType(NewType);

				if (PlatformToSpawn != nullptr)
				{
					SpawnedPlatform = GetWorld()->SpawnActor<AModule>(PlatformToSpawn, Position, FRotator::ZeroRotator);
				}

			}
		}

		// Spawn the selected module
		if (SpawnedPlatform != nullptr)
		{
			SpawnedPlatform->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			SpawnedPlatform->SetActorLocation(Position);
			UE_LOG(LogTemp, Warning, TEXT("Length: %f"), SpawnedPlatform->GetModuleLength());
			Position.Y += SpawnedPlatform->GetModuleLength();

			// Set previous and next modules	
			if (PreviousPlatform != nullptr)
			{
				SpawnedPlatform->SetPrevious(PreviousPlatform);

				PreviousPlatform->SetNext(SpawnedPlatform);

				PreviousPlatform->SpawnPickups(PlatformCount);
			}

		}

		// Keep reference to last platform spawned
		PreviousPlatform = SpawnedPlatform;

		// Player the player at the beginning of the second player
		if (PlatformCount == 1)
		{
			PlatformPlayerToCheck = SpawnedPlatform;

			if (RunnerCharacter != nullptr)
			{
				FVector PlayerLoc = PlatformPlayerToCheck->GetActorLocation();

				PlayerLoc.Z = 110.0f;

				RunnerCharacter->SetActorLocation(PlayerLoc);
			}
		}

		// Platform to remove
		if (PlatformCount == 0)
		{
			FirstPlatform = SpawnedPlatform;
		}

		PlatformCount++;
	}
}

EPlatformType AGameController::GetPlatformTypeToSpawn(const EPlatformType& PlatformType)
{

	EPlatformType SelectedPlatform = EPlatformType::VE_Ground;

	TArray<FProbabilityTable> TempTable;

	// We can add all of the platforms if previous is one of this
	if ((PlatformType == EPlatformType::VE_Ground) ||
		(PlatformType == EPlatformType::VE_Bridge) ||
		(PlatformType == EPlatformType::VE_TwoBridges))
	{

		// We add all platforms
		for (int i = 0; i < PlatformTable.Num(); i++)
		{
			TempTable.Add(PlatformTable[i]);
		}

	}
	else if ((PlatformType == EPlatformType::VE_SmallCenterBridge) ||
		(PlatformType == EPlatformType::VE_SmallLeftBridge) ||
		(PlatformType == EPlatformType::VE_SmallRightBridge))
	{
		// Only add VE_Ground, VE_Bridge, VE_TwoBridges
		for (int i = 0; i < PlatformTable.Num(); i++)
		{
			if ((PlatformTable[i].PlatformType == EPlatformType::VE_Ground) ||
				(PlatformTable[i].PlatformType == EPlatformType::VE_Bridge) ||
				(PlatformTable[i].PlatformType == EPlatformType::VE_TwoBridges))
			{
				TempTable.Add(PlatformTable[i]);
			}
		}
	}

	if (TempTable.Num() == 0)
	{
		return EPlatformType::VE_Ground;
	}


	// Go through all temp probabilities 
	float total = 0;
	for (int i = 0; i < TempTable.Num(); i++)
	{
		total += TempTable[i].Probability;
	}

	float RandomPoint = FMath::FRand() * total;

	int IndexPlatform = -1;
	for (int i = 0; i < TempTable.Num(); i++)
	{
		if (RandomPoint < TempTable[i].Probability)
		{
			IndexPlatform = i;
			break;
		}
		else
		{
			RandomPoint -= TempTable[i].Probability;
		}
	}

	if (IndexPlatform == -1)// No index was found, get the last one
	{
		IndexPlatform = TempTable.Num() - 1;
	}

	if (IndexPlatform < TempTable.Num())
	{
		SelectedPlatform = TempTable[IndexPlatform].PlatformType;
	}


	return SelectedPlatform;
}

TSubclassOf<class AModule> AGameController::GetModuleByType(const EPlatformType& PreviousType)
{
	for (int i = 0; i < PlatformTable.Num(); i++)
	{
		if (PreviousType == PlatformTable[i].PlatformType)
		{
			return PlatformTable[i].PlatformClass;
		}
	}

	return nullptr;
}


void AGameController::SpawnNewPlatform()
{
	if (PreviousPlatform == nullptr) return;

	FVector Position = PreviousPlatform->GetActorLocation();
	/*UE_LOG(LogTemp, Warning, TEXT("Length: %f"), SpawnedPlatform->GetModuleLength());*/
	Position.Y += PreviousPlatform->GetModuleLength();

	//TSubclassOf<AModule> PlatformToSpawn = nullptr;

	//int32 RandomIndex = FMath::RandRange(0, 2);
	//// Find the platform by type
	//for (int i = 0; i < PlatformTable.Num(); i++)
	//{
	//	if ((EPlatformType)RandomIndex == PlatformTable[i].PlatformType)
	//	{
	//		PlatformToSpawn = PlatformTable[i].PlatformClass;
	//		break;
	//	}
	//}

	EPlatformType NewType = GetPlatformTypeToSpawn(PreviousPlatform->GetModuleType());
	TSubclassOf<AModule> PlatformToSpawn = GetModuleByType(NewType);


	if (PlatformToSpawn != nullptr)
	{
		AModule* SpawnedPlatform = GetWorld()->SpawnActor<AModule>(PlatformToSpawn, Position, FRotator::ZeroRotator);

		// Spawn the module
		if (SpawnedPlatform != nullptr)
		{
			SpawnedPlatform->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			SpawnedPlatform->SetActorLocation(Position);
			//UE_LOG(LogTemp, Warning, TEXT("Length: %f"), SpawnedPlatform->GetModuleLength());
			Position.Y += SpawnedPlatform->GetModuleLength();

			// Set previous and next modules	
			if (PreviousPlatform != nullptr)
			{
				SpawnedPlatform->SetPrevious(PreviousPlatform);

				PreviousPlatform->SetNext(SpawnedPlatform);
			}

		}

		// Keep reference to last module spawned
		PreviousPlatform = SpawnedPlatform;

		PlatformCount++;
	}

}

void AGameController::OnRespawn()
{
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

	if (GameState == EGameState::VE_PrepareGame)
	{
		if (PlayerController->GetUI() != nullptr)
		{
			PlayerController->GetUI()->OnStartGame();

			GameState = EGameState::VE_Respawn;

			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AGameController::OnRespawn, 3.0f, false);
		}
	}
	else if (GameState == EGameState::VE_RemovePlatforms)
	{
		//Destroy all platforms
		AModule* First = FirstPlatform;
		while (First->GetNext() != nullptr)
		{
			AModule* Temp = First->GetNext();
			First->DestroyModule();
			First = Temp;
		}

		if (First != nullptr)
		{
			First->DestroyModule();
		}

		FirstPlatform = nullptr;

		InitializeGame();

		GameState = EGameState::VE_PrepareGame;

		GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AGameController::OnRespawn, 3.0f, false);

	}
	else if (GameState == EGameState::VE_Respawn)
	{

		if (PlayerController != nullptr)
		{
			PlayerController->Respawn();

			PlayerController->StartRunning();

			GameState = EGameState::VE_Running;
		}
	}
	
}