// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClothStaticItem.h"
#include "GlassesClothStaticItem.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNSHOOTER_API UGlassesClothStaticItem : public UClothStaticItem
{
	GENERATED_BODY()

protected:
	virtual void Use(class AShooterCharacter* Character) override;
	
};
