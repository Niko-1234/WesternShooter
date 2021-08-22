// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Niagara/Public/NiagaraComponent.h"

#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class WESTERNSHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* MuzzleFlash;

	int BulletsInMagazine;

	//class constans
	UPROPERTY(EditDefaultsOnly, Category = "Gun Properties")
	int MagazieSize = 6;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	USoundBase* ShotSound;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Properties")
	float MaxRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Properties")
	float Damage = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactEffectEnviroment;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ImpactEffectBody;

// PUBLIC METHODS
public:
	void PullTrigger(float AditionalDamage);
	int GetBulletsLeft() const;
	void ReloadBullets();

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;

};
