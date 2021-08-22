// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"
#include "ShooterCharacter.h"
#include "InventoryComponent.h"

void UFoodItem::Use(class AShooterCharacter* Character)
{
	if (Character)
	{
		Super::Use(Character);

		Character->IncreaseHealth(HealthToHeal);
		if (ItemAmount > 1)
		{
			ItemAmount--;
		}
		else
		{
			OwningInventory->RemoveItem(this);
		}
	}
}