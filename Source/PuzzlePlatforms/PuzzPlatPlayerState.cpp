// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzPlatPlayerState.h"

void APuzzPlatPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //DOREPLIFETIME(APuzzPlatPlayerState, CurrentTeam); //WHY DOES THIS GIVE ERROR?
}

