// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchManager.h"

#include "UObject/ConstructorHelpers.h"
#include "Puck.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "PlayerMallet.h"
#include "GameFramework/PlayerState.h"
#include "Sound/AmbientSound.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMatchManager::AMatchManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates=true;

	ConstructorHelpers::FClassFinder<AActor> PuckBP(TEXT("/Game/BP/BP_Puck"));
	PuckBPClass=PuckBP.Class;

	score.Init(0, 2);

	

}

// Called when the game starts or when spawned
void AMatchManager::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmbientSound::StaticClass(), Sounds);
	UE_LOG(LogTemp, Warning, TEXT("Sounds array size=%d"), Sounds.Num());


	SpawnPuck();
	
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
		if(world->IsServer())
		{
			if(Puck!=nullptr) Puck->Destroy();
			Puck=(APuck*)world->SpawnActor<APuck>(PuckBPClass, PuckSpawnLocation, FRotator::ZeroRotator);
		}
	}
	
	AGameStateBase* gamestate=world->GetGameState();
	for(int i=0; i<gamestate->PlayerArray.Num(); i++)
	{
		APlayerMallet* mallet = Cast<APlayerMallet>(gamestate->PlayerArray[i]->GetPawn());
		if(mallet==nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("MALLET IS NULL"));
			return;
		}
		else
		{
			mallet->InitMallet();
		}
	}
	
}

void AMatchManager::GoalScored(int Team)
{
	GetWorldTimerManager().SetTimer(PuckSpawnTimer, this, &AMatchManager::SpawnPuck, 3.0f);
	int i=score[Team];
	score[Team]=++i;
	UE_LOG(LogTemp, Warning, TEXT("SCORE:%d:%d"), score[0], score[1]);
	
	AAmbientSound* goalsound=Cast<AAmbientSound>(Sounds[0]);
	goalsound->Play();
	AAmbientSound* goalsound_voice=Cast<AAmbientSound>(Sounds[1]);
	goalsound_voice->Play();
}


