// Fill out your copyright notice in the Description page of Project Settings.

#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

// TODO Remove Below
#include "Engine/World.h"

void ATankGameModeBase::BeginPlay()
{
	HandleGameStart();

	Super::BeginPlay();
}

int ATankGameModeBase::GetTargetTurretCount()
{
	TSubclassOf<APawnTurret> ClassToFind;
	ClassToFind = APawnTurret::StaticClass();
	TArray<AActor *> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, TurretActors);
	return TurretActors.Num();
}

void ATankGameModeBase::ActorDied(AActor *DeadActor)
{
	if (DeadActor == PlayerTank)
	{
		PlayerTank->PawnDestroyed();
		HandleGameOver(false);

		if (PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
		}
	}
	else if (APawnTurret *DestroyedTurret = Cast<APawnTurret>(DeadActor))
	{
		DestroyedTurret->PawnDestroyed();
		TargetTurret--;
		if (TargetTurret == 0)
		{
			HandleGameOver(true);
		}
	}
}

void ATankGameModeBase::HandleGameStart()
{
	TargetTurret = GetTargetTurretCount();
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	GameStart();
	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableHandle;
		// Passing in the called function argument for the final parameter
		// Look Up
		FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(
			PlayerController,
			&APlayerControllerBase::SetPlayerEnabledState,
			true);

		GetWorld()->GetTimerManager().SetTimer(
			PlayerEnableHandle,
			PlayerEnableDelegate,
			StartDelay,
			false);
	}
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	GameOver(PlayerWon);
}
