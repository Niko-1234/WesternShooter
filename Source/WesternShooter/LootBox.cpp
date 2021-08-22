// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBox.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "ShooterCharacter.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WesternShooterPlayerController.h"
#include "Item.h"
#include "LootComponent.h"

// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxDimensions.X = 50.f;
	BoxDimensions.Y = 50.f;
	BoxDimensions.Z = 50.f;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	CollisionBox->InitBoxExtent(BoxDimensions);
	RootComponent = CollisionBox;

	BoxBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(CT_BoxBaseMesh);
	BoxBaseMesh->SetupAttachment(RootComponent);
	BoxTopMesh = CreateDefaultSubobject<UStaticMeshComponent>(CT_BoxTopMesh);
	BoxTopMesh->SetupAttachment(RootComponent);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALootBox::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ALootBox::OnOverlapEnd);

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->Capacity = 20;

	LootComponent = CreateDefaultSubobject<ULootComponent>(TEXT("LootComponent"));
}

// Called when the game starts or when spawned
void ALootBox::BeginPlay()
{
	Super::BeginPlay();
	bIsOverlapingPlayer = false;
	bOpenTop = false;
	
}

// Called every frame
void ALootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Inventory->Items.Num() < 1 || bOpenTop)
	{
		KeepOpen();
	}

}

void ALootBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
							  AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex,
							  bool bFromSweep,
							  const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Box Begin Overlap"));
	AShooterCharacter* OverlapedChar = Cast<AShooterCharacter>(OtherActor);
	if (OverlapedChar == nullptr) { return; }
	if (Inventory->Items.Num() < 1) { return; }
	if (OverlapedChar->GetController()->IsPlayerController())
	{
		bIsOverlapingPlayer = true;
		OverlapBegin();
		UE_LOG(LogTemp, Warning, TEXT("Box Begin Overlap Player"));
	}
}

void ALootBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Box End Overlap"));
	AShooterCharacter* OverlapedChar = Cast<AShooterCharacter>(OtherActor);
	if (OverlapedChar == nullptr) { return; }
	if (OverlapedChar->GetController()->IsPlayerController())
	{
		bIsOverlapingPlayer = false;
		OverlapEnd();
		UE_LOG(LogTemp, Warning, TEXT("Box End Overlap Player"));
	}
}

void ALootBox::Loot(AShooterCharacter* Looter)
{
	if (bIsOverlapingPlayer && Looter->IsPlayerControlled())
	{
		bOpenTop = true;
		LootComponent->OpenLootWidget();
	}
}

void ALootBox::KeepOpen()
{
	if(BoxTopMesh->GetRelativeRotation().Roll > -10.f)
		BoxTopMesh->AddLocalRotation(FRotator(0, 0, -0.5f));
}

