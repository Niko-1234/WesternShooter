// Fill out your copyright notice in the Description page of Project Settings.


#include "GlassesClothStaticItem.h"
#include "ShooterCharacter.h"

void UGlassesClothStaticItem::Use(AShooterCharacter* Character)
{
	ComponentToChange = Character->Glasses;

	Super::Use(Character);
}