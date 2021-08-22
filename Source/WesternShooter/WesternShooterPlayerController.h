// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WesternShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WESTERNSHOOTER_API AWesternShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf <class UUserWidget> WinScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf <class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf <class UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf <class UUserWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;

	FTimerHandle RestartTimer;

	UUserWidget* HUD;
	UUserWidget* UserInventory;

	UPROPERTY(EditAnywhere)
	TSubclassOf <class UUserWidget> LootWidgetClass;

public:
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterEvents")
	void EnemyHitted();

	void EnemyHittedInfo();
	void HandleInentory();

	void ShowLootWidget();

	virtual void SetupInputComponent() override;
};
