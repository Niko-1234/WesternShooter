// Fill out your copyright notice in the Description page of Project Settings.


#include "WesternShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AWesternShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), PlayerPawn);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

bool AWesternShooterAIController::IsDead() const
{
	AShooterCharacter* ControlledChar = Cast<AShooterCharacter>(GetPawn());
	if (ControlledChar != nullptr)
	{
		return ControlledChar->IsDead();
	}
	return true;
}

void AWesternShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}