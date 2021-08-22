// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponStaticItem.h"
#include "ShooterCharacter.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void UWeaponStaticItem::Use(AShooterCharacter* Character)
{
	ComponentToChange = Character->GunInBelt;

	if (ComponentToChange->GetStaticMesh() != PickupMesh)
	{
		//Equip off other weapons
		for (auto Item : OwningInventory->Items)
		{
			UWeaponStaticItem* Weapon = Cast<UWeaponStaticItem>(Item);
			if (Weapon)
			{
				Weapon->bIsEquipped = false;
			}
		}

		//Equip This weapon
		ComponentToChange->SetStaticMesh(PickupMesh);
		EquipHandle(Character);
		Character->ChangeGun(GunClass);
		UGameplayStatics::SpawnSoundAttached(SUseSound, Character->GetMesh(), TEXT("Equip Weapon Sound"));
	}
}
