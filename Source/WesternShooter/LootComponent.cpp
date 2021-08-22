// Fill out your copyright notice in the Description page of Project Settings.


#include "LootComponent.h"
#include "WesternShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "InventoryComponent.h"

// Sets default values for this component's properties
ULootComponent::ULootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULootComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULootComponent::PassItemToActor(UItem* Item, AShooterCharacter* Looter, UInventoryComponent* LootedInventory)
{
	LootedInventory->RemoveItem(Item);
	Looter->Inventory->AddItem(Item);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld() ,PickUpSound, GetOwner()->GetActorLocation());
}

void ULootComponent::OpenLootWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("Loot"));
	AWesternShooterPlayerController* PlayerController = Cast<AWesternShooterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->ShowLootWidget();
	}
}

