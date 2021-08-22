// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopAiming.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_StopAiming::UBTTask_StopAiming()
{
	NodeName = TEXT("Stop Aiming");
}

EBTNodeResult::Type UBTTask_StopAiming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->AimingEnd();
	return EBTNodeResult::Succeeded;
}
