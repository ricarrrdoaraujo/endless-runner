// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Enumerate for game states
UENUM(BlueprintType)
enum class EGameState : uint8
{
	VE_None 			UMETA(DisplayName = "None"),
	VE_InitializeGame	UMETA(DisplayName = "InitializeGame"),
	VE_PreparePlatform	UMETA(DisplayName = "PrepareGame"),
	VE_PrepareGame		UMETA(DisplayName = "PrepareGame"),
	VE_Running 		    UMETA(DisplayName = "Running"),
	VE_RemovePlatforms	UMETA(DisplayName = "RemovePlatforms"),
	VE_Respawn			UMETA(DisplayName = "Respawn"),
	VE_GameOver 		UMETA(DisplayName = "PrepareGame")
};

// Enumerate for platform types
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	VE_Ground 					UMETA(DisplayName = "Ground"),					// Normal Ground
	VE_Bridge 					UMETA(DisplayName = "Bridge"),					// Normal Bridge
	VE_SmallCenterBridge 		UMETA(DisplayName = "SmallCenterBridge"),		// Small bridge on the center
	VE_SmallLeftBridge 		    UMETA(DisplayName = "SmallLeftBridge"),			// Small bridge on the left side
	VE_SmallRightBridge 		UMETA(DisplayName = "SmallRightBridge"),		// Small bridge on the right side
	VE_TwoBridges				UMETA(DisplayName = "TwoBridges")				// Two Bridges

};

template<typename TEnum>
static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value)
{
	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
};

template <typename EnumType>
static FORCEINLINE EnumType GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return EnumType(0);
	}
	//return (EnumType)Enum->FindEnumIndex(FName(*String));
	return (EnumType)Enum->GetIndexByName(FName(*String));
};

/**
 * 
 */
class RUNNER_API Definitions
{
public:
	Definitions();
	~Definitions();
};
