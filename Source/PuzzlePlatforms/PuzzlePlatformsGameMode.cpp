// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformsGameMode.h"
#include "PuzzlePlatformsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PuzzlePlatformsGameInstance.h"
#include "PuzzPlatPlayerState.h"



APuzzlePlatformsGameMode::APuzzlePlatformsGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("GMode: Game mode constructor called"));

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP/BP_PlayerMallet"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerStateClass=APuzzPlatPlayerState::StaticClass();

}

void APuzzlePlatformsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("GMode: Game mode POSTLOGIN called"));

	int team=(2-++NumberOfPlayers)%2;
	APuzzPlatPlayerState* pState=NewPlayer->GetPlayerState<APuzzPlatPlayerState>();

	if(pState!=nullptr)
	{
		pState->CurrentTeam=(2-NumberOfPlayers)%2;
	}


	if(NumberOfPlayers>=2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &APuzzlePlatformsGameMode::StartGame, 2);
	}
}

void APuzzlePlatformsGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void APuzzlePlatformsGameMode::StartGame()
{
	auto GameInstance=Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	if(GameInstance==nullptr) return;

	GameInstance->StartSession();

	UWorld* World=GetWorld();
	if(!ensure(World!=nullptr)) return;

	bUseSeamlessTravel=true;
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");


}
