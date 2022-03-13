// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMallet.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "PuzzPlatPlayerState.h"

#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
APlayerMallet::APlayerMallet()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates=true;
	SetReplicateMovement(true);

	// set our turn rates for input
	BaseTurnRate=45.f;
	BaseLookUpRate=45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement=true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate=FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity=600.f;
	GetCharacterMovement()->AirControl=0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom=CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength=300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation=true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation=false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

void APlayerMallet::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("MALLET: begin play"));

	if(GetWorld()==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is NULL!"));
		return;
	}

	GetWorldTimerManager().SetTimer(MalletInitDelayTimer, this, &APlayerMallet::DelayedBeginPlay, 0.50f);

	

}

void APlayerMallet::DelayedBeginPlay()
{
	InitMallet();
}

void APlayerMallet::InitMallet()
{
	GetCharacterMovement()->Velocity=FVector::ZeroVector;

	TArray<AActor*> spawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), spawnPoints);
	

	TArray<ASpawnPoint*> spawnPoints_Sorted;
	for(int i=0; i<spawnPoints.Num(); i++)
	{
		for(AActor* actor:spawnPoints)
		{
			ASpawnPoint* sp=Cast<ASpawnPoint>(actor);
			if(sp->Team==i)
				spawnPoints_Sorted.Add(sp);
		}
	}

	AGameStateBase* gamestate=GetWorld()->GetGameState();
	if(gamestate!=nullptr)
	{

		for(int i=0; i<gamestate->PlayerArray.Num(); i++)
		{
			APuzzPlatPlayerState* pState=Cast<APuzzPlatPlayerState>(gamestate->PlayerArray[i]);
			if(pState!=nullptr)
			{
				APlayerMallet* mallet=Cast<APlayerMallet>(pState->GetPawn());
				if(mallet==nullptr) return;
				if(mallet->StaticMesh==nullptr) return;
				int team_value=pState->CurrentTeam;
				
				switch(team_value)
				{
					case 0:
						mallet->StaticMesh->SetMaterial(0, RedTeamMaterial);
						break;
					case 1:
						mallet->StaticMesh->SetMaterial(0, BlueTeamMaterial);
						break;
					default:
						UE_LOG(LogTemp, Warning, TEXT("Team is invalid when setting colours"));
				}

				if(GetWorld()->IsServer())
				{
					if(spawnPoints_Sorted.Num()!=0) mallet->SetActorLocation(spawnPoints_Sorted[team_value]->GetActorLocation());
				}
					
			}
		}
	}
}



void APlayerMallet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}



void APlayerMallet::OnResetVR()
{
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APlayerMallet::MoveForward(float Value)
{
	if((Controller!=NULL)&&(Value!=0.0f))
	{
		// find out which way is forward
		const FRotator Rotation=Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerMallet::MoveRight(float Value)
{
	if((Controller!=NULL)&&(Value!=0.0f))
	{
		// find out which way is right
		const FRotator Rotation=Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerMallet::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate*BaseTurnRate*GetWorld()->GetDeltaSeconds());
}

void APlayerMallet::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate*BaseLookUpRate*GetWorld()->GetDeltaSeconds());
}

void APlayerMallet::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void APlayerMallet::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

// Called to bind functionality to input
void APlayerMallet::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerMallet::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerMallet::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerMallet::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerMallet::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlayerMallet::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlayerMallet::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APlayerMallet::OnResetVR);
}



