// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

UItem::UItem()
{
	Weight = 0.f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

void UItem::Use(AShooterCharacter* Character)
{
	UGameplayStatics::SpawnSoundAttached(SUseSound, Character->GetMesh(), TEXT("Use Item Sound"));
}
