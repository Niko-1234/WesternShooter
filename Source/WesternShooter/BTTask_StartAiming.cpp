// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartAiming.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_StartAiming::UBTTask_StartAiming()
{
	NodeName = TEXT("Start Aiming");
}

EBTNodeResult::Type UBTTask_StartAiming::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	Character->AimingStart();
	return EBTNodeResult::Succeeded;
}
