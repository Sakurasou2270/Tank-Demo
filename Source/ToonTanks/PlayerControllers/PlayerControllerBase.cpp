// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"

void APlayerControllerBase::SetPlayerEnabledState(bool SetPlayerEnabled)
{
    if (SetPlayerEnabled)
    {
        GetWorld()->GetFirstPlayerController()->GetPawn()->EnableInput(this);
    }
    else
    {
        GetWorld()->GetFirstPlayerController()->GetPawn()->DisableInput(this);
    }
    // bShowMouseCursor = SetPlayerEnabled;
}