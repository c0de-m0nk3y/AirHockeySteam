// Fill out your copyright notice in the Description page of Project Settings.


#include "Puck.h"

#include "Components/StaticMeshComponent.h"
#include "Goal.h"
#include "MatchManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APuck::APuck()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates=true;
	SetReplicateMovement(true);

	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent=StaticMesh;
	StaticMesh->OnComponentHit.AddDynamic(this, &APuck::OnHit);
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &APuck::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APuck::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void APuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GoalScored)
	{
		RootComponent->ComponentVelocity=FVector::ZeroVector;
	}
}

void APuck::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{

}

void APuck::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GoalScored) return;

	AGoal* goal=Cast<AGoal>(OtherActor);
	if(goal!=nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GOAL!!! Team Scoring is:%d"), goal->AttackingTeam);
		AMatchManager* matchManager=Cast<AMatchManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMatchManager::StaticClass()));
		if(matchManager==nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Match Manager is NULL"));
			return;
		}
		else
		{
			matchManager->GoalScored(goal->AttackingTeam);
			GoalScored=true;
		}
	}
}

