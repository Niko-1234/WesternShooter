// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WesternShooterGameModeBase.h"
#include "KillEmAllGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNSHOOTER_API AKillEmAllGameMode : public AWesternShooterGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* KilledPawn) override;
	
private:
	void EndGame(bool bIsPlayerWinner);
};
