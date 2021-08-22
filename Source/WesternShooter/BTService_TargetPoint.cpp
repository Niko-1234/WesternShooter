// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TargetPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"

UBTService_TargetPoint::UBTService_TargetPoint()
{
	NodeName = TEXT("Update Target Point Location");
}

void UBTService_TargetPoint::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr) { return; }
	if (OwnerComp.GetAIOwner() == nullptr) { return; }

	APawn* TargetPoint = Cast<AShooterCharacter>(PlayerPawn)->Target;
	if (TargetPoint)
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), TargetPoint);
}