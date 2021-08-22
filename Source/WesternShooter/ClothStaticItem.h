// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ClothStaticItem.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNSHOOTER_API UClothStaticItem : public UItem
{
	GENERATED_BODY()

public:

	UClothStaticItem();

	/**Value of Armomr given by item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float ItemArmor;

	/**Damage Boost given by item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0))
	float ItemDamage;

	/**Item equipo indicator*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bIsEquipped;

protected:
	UStaticMeshComponent* ComponentToChange;

protected:

	virtual void Use(class AShooterCharacter* Character) override;
	void EquipHandle(AShooterCharacter* Character);
	void TakeOffHandle(AShooterCharacter* Character);
	
};
