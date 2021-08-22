// Fill out your copyright notice in the Description page of Project Settings.


#include "ClothStaticItem.h"
#include "ShooterCharacter.h"


UClothStaticItem::UClothStaticItem()
{
	ComponentToChange = nullptr;
	bIsEquipped = false;
}

void UClothStaticItem::Use(AShooterCharacter* Character)
{
	Super::Use(Character);

	if (ComponentToChange == nullptr)
		return;

	//if (ComponentToChange->GetStaticMesh() == nullptr)
	//{
	//	ComponentToChange->SetStaticMesh(PickupMesh);
	//	Character->Armor += ItemArmor;
	//	Character->DamageBoost += ItemDamage;
	//	bIsEquipped = true;
	//}
	if (bIsEquipped)
	{
		ComponentToChange->SetStaticMesh(nullptr);
		TakeOffHandle(Character);
	}
	else
	{
		ComponentToChange->SetStaticMesh(PickupMesh);
		EquipHandle(Character);
	}
}

void UClothStaticItem::EquipHandle(AShooterCharacter* Character)
{
	Character->Armor += ItemArmor;
	Character->DamageBoost += ItemDamage;
	bIsEquipped = true;
}

void UClothStaticItem::TakeOffHandle(AShooterCharacter* Character)
{
	if (bIsEquipped)
	{
		Character->Armor -= ItemArmor;
		Character->DamageBoost -= ItemDamage;
		bIsEquipped = false;
	}
}
