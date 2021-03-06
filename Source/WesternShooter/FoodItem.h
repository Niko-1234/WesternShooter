// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "FoodItem.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNSHOOTER_API UFoodItem : public UItem
{
	GENERATED_BODY()

public:

	/**Value of Health to heal*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float HealthToHeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0))
	int ItemAmount;

protected:

	virtual void Use(class AShooterCharacter* Character) override;
};
