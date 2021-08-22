// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootBox.generated.h"

UCLASS()
class WESTERNSHOOTER_API ALootBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALootBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// CLASS CONSTANTS
private:
	const FName CT_BoxBaseMesh = (TEXT("BoxBase"));
	const FName CT_BoxTopMesh = (TEXT("BoxTop"));

	bool bOpenTop;

	// PROPERTIES
public:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BoxBaseMesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BoxTopMesh;

	FVector BoxDimensions;

	bool bIsOverlapingPlayer;

	UPROPERTY()
	class AShooterCharacter* OverlapedActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class ULootComponent* LootComponent;

	// METHODS
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	//Overlap Begin Event for Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "LootBox Events")
	void OverlapBegin();

	//Overlap End Event for Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "LootBox Events")
	void OverlapEnd();

	void Loot(AShooterCharacter* Looter);

private:
	void KeepOpen();

};
