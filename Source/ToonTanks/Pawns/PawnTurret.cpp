// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTurret.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

APawnTurret::APawnTurret()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(
        FireRateTimerHandle,
        this,
        &APawnTurret::CheckFireCondition,
        FireRate,
        true,
        false);

    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerTank || ReturnDistanceToPlayer() > FireRange)
    {
        return;
        UE_LOG(LogTemp, Warning, TEXT("NO Tank"));
    }
    RotateTurret(PlayerTank->GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
    if (!PlayerTank || !PlayerTank->GetIsPlayerAlive())
    {
        return;
    }

    if (ReturnDistanceToPlayer() <= FireRange)
    {
        Fire();
    }
}

float APawnTurret::ReturnDistanceToPlayer()
{
    if (!PlayerTank)
    {
        return 0.f;
    }

    float Distance = (PlayerTank->GetActorLocation() - GetActorLocation()).Size();
    return Distance;
}

void APawnTurret::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}