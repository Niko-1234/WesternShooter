// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
#include "WesternShooterPlayerController.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	MuzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlash"));
	MuzzleFlash->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	BulletsInMagazine = MagazieSize;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger(float AditionalDamage)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleFlash->GetAsset(), MuzzleFlash->GetComponentLocation(), MuzzleFlash->GetComponentRotation());
	UGameplayStatics::SpawnSoundAttached(ShotSound, Mesh, TEXT("Shot Sound"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		AActor* HitedActor = Hit.GetActor();
		if (HitedActor != nullptr && Cast<AShooterCharacter>(HitedActor))
		{
			float CurrentDamage = Damage + AditionalDamage;
			if (Hit.BoneName == TEXT("head"))
			{
				CurrentDamage *= 2;
			}
			UE_LOG(LogTemp, Warning, TEXT("Hitted Bone: %s"), *Hit.BoneName.ToString());
			FPointDamageEvent DamageEvent(CurrentDamage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();

			if (OwnerController == nullptr) { return; }
			HitedActor->TakeDamage(CurrentDamage, DamageEvent, OwnerController, this);

			// Tell Controler that enemy was hitted to show hitmark 
			if (OwnerController->IsPlayerController())
			{
				Cast<AWesternShooterPlayerController>(OwnerController)->EnemyHittedInfo();
			}
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffectBody, Hit.Location, ShotDirection.Rotation());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffectEnviroment, Hit.Location, ShotDirection.Rotation());
		}
	}

	BulletsInMagazine--;
}

int AGun::GetBulletsLeft() const
{	
	return BulletsInMagazine;
}

void AGun::ReloadBullets()
{
	BulletsInMagazine = MagazieSize;
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) { return false; }

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	OwnerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	ShotDirection = PlayerViewPointRotation.Vector() * (-1);

	FVector End = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, PlayerViewPointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) { return nullptr; }
	return OwnerPawn->GetController();
}

