// Fill out your copyright notice in the Description page of Project Settings.


#include "WesternShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"


void AWesternShooterPlayerController::BeginPlay()
{
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

void AWesternShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
	
}


UUserWidget* AWesternShooterPlayerController::GetHUDWidget() const
{
	return HUD;
}

void AWesternShooterPlayerController::EnemyHittedInfo()
{
	EnemyHitted();
}

void AWesternShooterPlayerController::HandleInentory()
{
	UserInventory = CreateWidget(this, InventoryWidgetClass);
	if (UserInventory)
	{
		if (UserInventory->IsInViewport())
		{
			UserInventory->RemoveFromParent();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}
		else
		{
			UserInventory->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void AWesternShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AWesternShooterPlayerController::HandleInentory);
}

void AWesternShooterPlayerController::ShowLootWidget()
{
	UUserWidget* LootWidget = CreateWidget(this, LootWidgetClass);
	if (LootWidget)
	{
		LootWidget->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}


