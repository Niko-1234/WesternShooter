// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClothStaticItem.h"
#include "WeaponStaticItem.generated.h"

/**
 * 
 */

class AGun;

UCLASS()
class WESTERNSHOOTER_API UWeaponStaticItem : public UClothStaticItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AGun> GunClass;
	
protected:
	virtual void Use(class AShooterCharacter* Character) override;
};
