// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchManager.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AMatchManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatchManager();

	UPROPERTY(EditAnywhere)
		FVector PuckSpawnLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<class AActor> PuckBPClass;
	class APuck* Puck = nullptr;

	void SpawnPuck();
	void GoalScored(int Team);

private:
	FTimerHandle PuckSpawnTimer;
	TArray<int> score;

	TArray<class AActor*> Sounds;
};
