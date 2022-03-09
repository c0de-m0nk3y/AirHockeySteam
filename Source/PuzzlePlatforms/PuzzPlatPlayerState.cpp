// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzPlatPlayerState.h"

void APuzzPlatPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APuzzPlatPlayerState, CurrentTeam); //WHY DOES THIS GIVE ERROR?
}

void APuzzPlatPlayerState::CopyProperties(APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    UE_LOG(LogTemp, Warning, TEXT("Copying Properties YEAHBOIIIII"));
    APuzzPlatPlayerState* pState=Cast<APuzzPlatPlayerState>(PlayerState);
    pState->CurrentTeam=CurrentTeam;
}
