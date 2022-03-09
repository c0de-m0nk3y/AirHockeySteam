// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PuzzPlatPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API APuzzPlatPlayerState : public APlayerState
{
	GENERATED_BODY()
	


public:

	UPROPERTY(replicated)
		int CurrentTeam = 111;
};
