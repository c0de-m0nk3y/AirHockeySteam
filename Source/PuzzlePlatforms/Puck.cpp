// Fill out your copyright notice in the Description page of Project Settings.


#include "Puck.h"

#include "Components/StaticMeshComponent.h"

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

	
}

void APuck::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
}

