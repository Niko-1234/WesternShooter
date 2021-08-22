// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameFramework/SpringArmComponent.h"

#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class WESTERNSHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// PRIVATE VARIABLES
private:
	class USpringArmComponent* BoomArm;
	float ZoomSpeed;
	float HorizontalCameraOffsetSpeed;
	float VerticalCameraOffsetSpeed;
	bool bIsAiming;
	UPROPERTY()
	AGun* LHGun;
	UPROPERTY()
	AGun* RHGun;
	bool NextShootByRightHand;
	UPROPERTY(EditDefaultsOnly, Category = "Charactrer Properties")
	float MaxHealth = 100.f;
	float Health;
	bool bIsCrouch;

// PUBLIC VARIABLES
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GunInBelt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Glasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Hat;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Beard;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float HorizontalCameraOffsetRange = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float VerticalCameraOffsetRange = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float ZoomInDistance = 100.f;

	UPROPERTY()
	float ZoomOutDistance = 200.f;//Is seting on the SpringArm length on the game beggining

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float ZoomTime = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	float AimingSeedInputMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Shooting Animations")
	UAnimMontage* AnimMontShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Shooting Animations")
	FName LeftAMSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Shooting Animations")
	FName RightAMSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Shooting Animations")
	UAnimMontage* AMReloading;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AGun> LHGunClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AGun> RHGunClass;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	USoundBase* SDrawingGun;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	USoundBase* SPutOutGun;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	USoundBase* SReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, Category = "Charactrer Properties", meta = (ClampMin = 0.0))
	float DamageBoost;

	UPROPERTY(EditAnywhere, Category = "Charactrer Properties", meta = (ClampMin = 0.0))
	float Armor;

	UPROPERTY(EditAnywhere, Category = "Charactrer Properties", meta = (ClampMin = 100.0))
	float InteractionRange;

	UPROPERTY(BlueprintReadOnly)
	AActor* InteractedActor;

	UPROPERTY()
	class APawn* Target;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<APawn> TargetClass;
// PRIVATE METHODS
private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	FRotationMatrix GetRotationMatrix();
	FRotator GetCameraYawRotation();
	//void Aiming(float AxisValue);
	void AimingHandle();
	void ZoomIn(USpringArmComponent* SpringArm);
	void ZoomOut(USpringArmComponent* SpringArm);
	void ShowGunInHand();
	void ShowGunInBelt();
	void Reload();
	AActor* LookForNesrestLootObject(FVector LookingForFromLocation, float InteractRange);
	void Interact();
	void CrouchHandle();

public:
	UFUNCTION(BlueprintCallable)
	bool IsAiming();
	UFUNCTION(BlueprintCallable)
	bool IsDead();
	UFUNCTION(BlueprintCallable)
	float GetHealthProcentage();
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo();
	UFUNCTION(BlueprintCallable)
	void UseItem(class UItem* Item);
	UFUNCTION(BlueprintCallable)
	bool IsCrouch();

	void AimingStart();
	void AimingEnd();
	void Shot();
	void IncreaseHealth(float IncreaseValue);
	void ChangeGun(TSubclassOf<AGun> GunClass);
};
