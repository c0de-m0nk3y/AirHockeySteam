// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PuzzPlatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API APuzzPlatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:


	UPROPERTY(replicated)
		int Team = 111;
};
