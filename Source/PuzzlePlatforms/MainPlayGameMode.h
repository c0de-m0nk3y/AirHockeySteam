// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsGameMode.h"
#include "MainPlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMainPlayGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostSeamlessTravel() override;


};
