//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
////#include "Http.h"
//#include "Module.h"
//#include "Definitions.h"
//#include "GameController.generated.h"
//
//// Struct containing the platforms to spawn
//USTRUCT(BlueprintType)
//struct FProbabilityTable
//{
//	GENERATED_USTRUCT_BODY()
//
//		FProbabilityTable()
//	{
//		PlatformType = EPlatformType::VE_Ground;
//
//		Probability = 20.0f;
//	}
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
//		EPlatformType PlatformType;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
//		TSubclassOf<class AModule> PlatformClass;
//
//
//	// NEW:  Add weighted  probability for each platform
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
//		float Probability;
//};
//
//UCLASS()
//class RUNNER_API AGameController : public AActor
//{
//	GENERATED_BODY()
//
//protected:
//
//	UPROPERTY(EditInstanceOnly, Category = "Game Controller Settings")
//	AActor* FloorPlane;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Game Controller Settings")
//	TArray<FProbabilityTable> PlatformTable;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Game Controller Settings")
//	int VisibleModuleNumber = 7;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Game Controller Settings")
//	int PlayerLives = 3;
//	
//public:	
//	// Sets default values for this actor's properties
//	AGameController();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//	void InitializeGame();
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//	EGameState GameState = EGameState::VE_None;
//
//private:
//
//	int PlatformCount = 0;
//
//	class AModule* PreviousPlatform;
//
//	class AModule* PlatformPlayerToCheck;
//
//	class AModule* FirstPlatform;
//
//	class ARunnerPlayerController* PlayerController;
//
//	class ARunnerCharacter* RunnerCharacter;
//
//	void SpawnNewPlatform();
//
//	UFUNCTION(BlueprintCallable)
//	void StartSession();
//
//	void OpenGame();
//
//	UFUNCTION()
//	void OnRespawn();
//
//	FTimerHandle RespawnTimerHandle;
//
//	// Gets a platform according to the previous one and weighthed platforms
//	EPlatformType GetPlatformTypeToSpawn(const EPlatformType& PreviousType);
//
//	TSubclassOf<class AModule> GetModuleByType(const EPlatformType& PreviousType);
//
////private:
////	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
//};
