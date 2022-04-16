#include "Pickup.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

APickup::APickup()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	PrimaryActorTick.bCanEverTick = false;

}