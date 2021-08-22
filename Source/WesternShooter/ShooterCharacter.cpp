// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WesternShooterGameModeBase.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "WesternShooterPlayerController.h"
#include "LootBox.h"
#include "LootComponent.h"
#include "Components/ActorComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false; //Dont use camera rotation to character -> blueprint have to be revreated
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GunInBelt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LHBeltGun"));
	GunInBelt->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LHBeltGunSocket"));
	Glasses = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glasses"));
	Glasses->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadItemSocket"));
	Hat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
	Hat->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadItemSocket"));
	Beard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beard"));
	Beard->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadItemSocket"));

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->Capacity = 30;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	BoomArm = FindComponentByClass<USpringArmComponent>();
	if (BoomArm == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spring Arm Component"));
	}
	else
	{
		ZoomOutDistance = BoomArm->TargetArmLength;
		ZoomSpeed = (ZoomOutDistance - ZoomInDistance) / ZoomTime;
		HorizontalCameraOffsetSpeed = HorizontalCameraOffsetRange / ZoomTime;
		VerticalCameraOffsetSpeed = VerticalCameraOffsetRange / ZoomTime;
	}

	bIsAiming = false;
	NextShootByRightHand = true;
	bIsCrouch = false;
	//WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	LHGun = GetWorld()->SpawnActor<AGun>(LHGunClass);
	LHGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LHGunSocket"));
	LHGun->SetOwner(this);

	RHGun = GetWorld()->SpawnActor<AGun>(RHGunClass);
	RHGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RHGunSocket"));
	RHGun->SetOwner(this);

	Target = GetWorld()->SpawnActor<APawn>(TargetClass);
	Target->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HeadItemSocket"));
	Target->SetOwner(this);
	Target->AddActorLocalOffset(FVector(-20, 0, 0));

	ShowGunInBelt();

	Health = MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimingHandle();

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis(TEXT("Aiming"), this, &AShooterCharacter::Aiming);
	PlayerInputComponent->BindAction(TEXT("AimingStart"), EInputEvent::IE_Pressed, this, &AShooterCharacter::AimingStart);
	PlayerInputComponent->BindAction(TEXT("AimingEnd"), EInputEvent::IE_Released, this, &AShooterCharacter::AimingEnd);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AShooterCharacter::CrouchHandle);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageApplied = FMath::Max(0.f, DamageApplied - Armor);
	DamageApplied = FMath::Min(Health, DamageApplied);

	Health -= DamageApplied;
	UE_LOG(LogTemp, Warning, TEXT("%s Health Left: %f"),*GetName(), Health);

	if (IsDead())
	{
		AWesternShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AWesternShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	return DamageApplied;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	if (bIsAiming || bIsCrouch)
	{
		AxisValue *= AimingSeedInputMultiplier;
	}
	AddMovementInput(GetRotationMatrix().GetUnitAxis(EAxis::X), AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	if (bIsAiming || bIsCrouch)
	{
		AxisValue *= AimingSeedInputMultiplier;
	}
	AddMovementInput(GetRotationMatrix().GetUnitAxis(EAxis::Y), AxisValue);
}

FRotationMatrix AShooterCharacter::GetRotationMatrix()
{
	return FRotationMatrix(GetCameraYawRotation());
}

FRotator AShooterCharacter::GetCameraYawRotation()
{
	return FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
}

//void AShooterCharacter::Aiming(float AxisValue)
//{
//	if (AxisValue > 0.f)
//	{
//		//Rotate to Camera Direction
//		FRotator DeltaRotation = FRotator(GetCameraYawRotation() - FRotator(0, GetActorRotation().Yaw, 0));
//		AddActorLocalRotation(DeltaRotation, false);
//
//		ZoomIn(BoomArm);
//	}
//	else
//	{
//		ZoomOut(BoomArm);
//	}
//}

void AShooterCharacter::AimingHandle()
{
	if (bIsAiming && !IsDead())
	{
		//Rotate to Camera Direction
		FRotator DeltaRotation = FRotator(GetCameraYawRotation() - FRotator(0, GetActorRotation().Yaw, 0));
		AddActorLocalRotation(DeltaRotation, false);

		ZoomIn(BoomArm);
	}
	else
	{
		ZoomOut(BoomArm);
	}
}

void AShooterCharacter::ZoomIn(USpringArmComponent* SpringArm)
{
	if (SpringArm == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spring Arm Component"));
		return;
	}
	if (SpringArm->GetRelativeLocation().Y < HorizontalCameraOffsetRange)
	{
		FVector DeltaCameraOffset = FVector(0, HorizontalCameraOffsetSpeed * GetWorld()->DeltaTimeSeconds, VerticalCameraOffsetSpeed * GetWorld()->DeltaTimeSeconds);
		SpringArm->AddRelativeLocation(DeltaCameraOffset, false);
	}

	if (SpringArm->TargetArmLength > ZoomInDistance)
	{
		SpringArm->TargetArmLength -= ZoomSpeed*GetWorld()->DeltaTimeSeconds;
	}
}

void AShooterCharacter::ZoomOut(USpringArmComponent* SpringArm)
{
	if (SpringArm == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spring Arm Component"));
		return;
	}
	if (SpringArm->GetRelativeLocation().Y > 0)
	{
		FVector DeltaCameraOffset = FVector(0, HorizontalCameraOffsetSpeed * GetWorld()->DeltaTimeSeconds, VerticalCameraOffsetSpeed * GetWorld()->DeltaTimeSeconds);
		SpringArm->AddRelativeLocation(-1 * DeltaCameraOffset, false);
	}
	if (SpringArm->TargetArmLength < ZoomOutDistance)
	{
		SpringArm->TargetArmLength += ZoomSpeed*GetWorld()->DeltaTimeSeconds;
	}
}

void AShooterCharacter::AimingStart()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	ShowGunInHand();
	UGameplayStatics::SpawnSoundAttached(SDrawingGun, GetMesh(), TEXT("Draw Gun Sound"));
	bIsAiming = true;
}

void AShooterCharacter::AimingEnd()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	ShowGunInBelt();
	UGameplayStatics::SpawnSoundAttached(SPutOutGun, GetMesh(), TEXT("Draw Gun Sound"));
	bIsAiming = false;
}

void AShooterCharacter::Shot()
{
	if (!bIsAiming) { return; }
	if (GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() == AMReloading) { return; }

	if (LHGun->GetBulletsLeft() < 1 && LHGun->GetBulletsLeft() < 1)
	{
		Reload();
		return;
	}

	if (NextShootByRightHand) 
	{
		RHGun->PullTrigger(DamageBoost);
		PlayAnimMontage(AnimMontShooting, 1.0, RightAMSection);
		NextShootByRightHand = false;
	}
	else
	{
		LHGun->PullTrigger(DamageBoost);
		PlayAnimMontage(AnimMontShooting, 1.0, LeftAMSection);
		NextShootByRightHand = true;
	}
}

void AShooterCharacter::IncreaseHealth(float IncreaseValue)
{
	Health = FMath::Min(MaxHealth, (Health + IncreaseValue));
}

void AShooterCharacter::ChangeGun(TSubclassOf<AGun> GunClass)
{
	if (LHGun != nullptr && RHGun != nullptr)
	{
		LHGun->Destroy();
		RHGun->Destroy();
	}

	LHGun = GetWorld()->SpawnActor<AGun>(GunClass);
	LHGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LHGunSocket"));
	LHGun->SetOwner(this);

	RHGun = GetWorld()->SpawnActor<AGun>(GunClass);
	RHGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RHGunSocket"));
	RHGun->SetOwner(this);

	ShowGunInBelt();
}

void AShooterCharacter::UseItem(UItem* Item)
{
	Item->Use(this);
	Item->OnUse(this);
}

bool AShooterCharacter::IsCrouch()
{
	return bIsCrouch;
}

void AShooterCharacter::ShowGunInHand()
{
	LHGun->SetActorHiddenInGame(false);
	GunInBelt->SetVisibility(false);
}

void AShooterCharacter::ShowGunInBelt()
{
	LHGun->SetActorHiddenInGame(true);
	GunInBelt->SetVisibility(true);
}

void AShooterCharacter::Reload()
{
	PlayAnimMontage(AMReloading, 1.0, NAME_None);
	UGameplayStatics::SpawnSoundAttached(SReloadSound, GetMesh(), TEXT("Reload Sound"));
	LHGun->ReloadBullets();
	RHGun->ReloadBullets();
}

AActor* AShooterCharacter::LookForNesrestLootObject(FVector LookingForFromLocation, float InteractRange)
{
	AActor* NearestActor = nullptr;
	TArray <AActor*> OverlappedActors;
	TArray <AActor*> ActorsToIgnore;
	TArray <TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(LHGun);
	ActorsToIgnore.Add(RHGun);
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		LookingForFromLocation,
		InteractRange,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OverlappedActors);

	float NearestDistance = InteractRange;
	for (auto& actor : OverlappedActors) {
		float Distance = (actor->GetActorLocation() - GetActorLocation()).Size();
		ULootComponent* LootComp = actor->FindComponentByClass<ULootComponent>();
		if (Distance < NearestDistance && LootComp) 
		{
			NearestActor = actor;
			NearestDistance = Distance;
		}
	}
	return NearestActor;
}

void AShooterCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Char try interact"));
	InteractedActor = LookForNesrestLootObject(this->GetActorLocation(), InteractionRange);
	ALootBox* InteractedBox = Cast<ALootBox>(InteractedActor);
	if (InteractedBox)
	{
		InteractedBox->Loot(this);
	}
}

void AShooterCharacter::CrouchHandle()
{
	if (bIsCrouch)
	{
		bIsCrouch = false;
	}
	else
	{
		bIsCrouch = true;
	}
}

bool AShooterCharacter::IsAiming()
{
	return bIsAiming;
}

bool AShooterCharacter::IsDead()
{
	return (Health <= 0.f);
}

float AShooterCharacter::GetHealthProcentage()
{
	if (MaxHealth > 0)
		return Health / MaxHealth;
	else
		return 0.f;
}

float AShooterCharacter::GetMaxHealth()
{
	return MaxHealth;
}

int AShooterCharacter::GetCurrentAmmo()
{
	int CurrentAmmo = 0;
	if (LHGun != nullptr)
		CurrentAmmo += LHGun->GetBulletsLeft();
	if (RHGun != nullptr)
		CurrentAmmo += RHGun->GetBulletsLeft();

	return CurrentAmmo;
}

