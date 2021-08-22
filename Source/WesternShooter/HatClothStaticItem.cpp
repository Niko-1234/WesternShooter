// Fill out your copyright notice in the Description page of Project Settings.


#include "HatClothStaticItem.h"
#include "ShooterCharacter.h"
#include "InventoryComponent.h"

void UHatClothStaticItem::Use(AShooterCharacter* Character)
{
	ComponentToChange = Character->Hat;

	Super::Use(Character);

	//Equip off other weapons
	for (auto Item : OwningInventory->Items)
	{

		UHatClothStaticItem* Hat = Cast<UHatClothStaticItem>(Item);
		if (Hat && Item != this)
		{
			Hat->TakeOffHandle(Character);
		}
	}
}
