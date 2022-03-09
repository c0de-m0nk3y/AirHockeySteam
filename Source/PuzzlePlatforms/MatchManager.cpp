// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchManager.h"

#include "UObject/ConstructorHelpers.h"
#include "Puck.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AMatchManager::AMatchManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates=true;

	ConstructorHelpers::FClassFinder<AActor> PuckBP(TEXT("/Game/BP/BP_Puck"));
	PuckBPClass=PuckBP.Class;

}

// Called when the game starts or when spawned
void AMatchManager::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* world=GetWorld();
	if(world!=nullptr)
	{
		if(world->IsServer())
		{
			SpawnPuck();
		}
	}
}



// Called every frame
void AMatchManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
}

void AMatchManager::SpawnPuck()
{
	UWorld* world=GetWorld();
	if(world!=nullptr)
	{
		Puck=(APuck*)world->SpawnActor<APuck>(PuckBPClass, PuckSpawnLocation, FRotator::ZeroRotator);
	}

}


