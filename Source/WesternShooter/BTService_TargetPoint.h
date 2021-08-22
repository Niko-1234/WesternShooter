// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_TargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNSHOOTER_API UBTService_TargetPoint : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_TargetPoint();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
