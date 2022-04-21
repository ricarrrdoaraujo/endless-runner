// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUserUI.h"
#include "Components/TextBlock.h"

void UInGameUserUI::UpdateCoins(int32 Coins)
{
	if (CoinsCounter == nullptr) return;
	CoinsCounter->SetText(FText::FromString(FString::FromInt(Coins)));
}

void UInGameUserUI::UpdateDistance(float Distance)
{ 
	if (DistanceCounter == nullptr) return;
	
	float Rounded = roundf(Distance);

	FNumberFormattingOptions NumberFormat;
	NumberFormat.MinimumFractionalDigits = 0;
	NumberFormat.MaximumIntegralDigits = 10;

	FText NewDistance = FText::AsNumber(Rounded, &NumberFormat);
	DistanceCounter->SetText(NewDistance);
}

void UInGameUserUI::UpdateLives(int32 Lives)
{
	if (LivesCounter == nullptr) return;
	LivesCounter->SetText(FText::FromString(FString::FromInt(Lives)));
}