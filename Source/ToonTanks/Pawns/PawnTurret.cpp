// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTurret.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"

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
}

void APawnTurret::CheckFireCondition()
{
    if (!PlayerTank)
    {
        return;
    }

    if (ReturnDistanceToPlayer() <= FireRange)
    {
        UE_LOG(LogTemp, Warning, TEXT("Fire"));
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