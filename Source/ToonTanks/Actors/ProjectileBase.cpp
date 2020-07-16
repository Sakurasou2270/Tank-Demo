// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Dynamic Delegates
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

void AProjectileBase::OnHit(
	UPrimitiveComponent *HitComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	FVector NormalImpulse,
	const FHitResult &Hit)
{
	AActor * MyOwner = GetOwner();

	if (!MyOwner)
	{
		return;
	}

	if (OtherActor != NULL && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
	}
	Destroy();
}