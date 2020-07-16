// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "ToonTanks/GameModes/TankGameModeBase.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;
	GameMode = Cast<ATankGameModeBase>(GetWorld()->GetAuthGameMode());

	// Adding dynamic function to owning actor
	Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Owner"));
	}
}

void UHealthComponent::TakeDamage(
	AActor *DamageActor,
	float Damage,
	const class UDamageType *DamageType,
	class AController *InstagatedBy,
	AActor *DamageCauser)
{
	if (Damage == 0 || Health == 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	
	if (Health <= 0)
	{
		if (GameMode)
		{
			GameMode->ActorDied(Owner);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No GameMode Present"));
		}
	}
}