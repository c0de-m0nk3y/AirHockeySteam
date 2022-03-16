// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "PuzzlePlatformsGameMode.h"
#include "PuzzlePlatformsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "PuzzlePlatformsGameInstance.h"
#include "PuzzPlatPlayerState.h"



APuzzlePlatformsGameMode::APuzzlePlatformsGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("GMode:constructor invoked"));

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
	UE_LOG(LogTemp, Warning, TEXT("GMode:POSTLOGIN invoked"));

	int team=(NumberOfPlayers++)%2;
	APuzzPlatPlayerState* pState=NewPlayer->GetPlayerState<APuzzPlatPlayerState>();

	UE_LOG(LogTemp, Warning, TEXT("GMode:NUMPLAYERS=%d"), NumberOfPlayers);

	if(pState!=nullptr)
	{
		pState->CurrentTeam=team;
		UE_LOG(LogTemp, Warning, TEXT("GMode:POSTLOGIN pState-CurrentTeam=%d"), pState->CurrentTeam);
	}

	UPuzzlePlatformsGameInstance* gameinstance=Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if(gameinstance!=nullptr)
	{
		if(NumberOfPlayers>=gameinstance->NumPlayersPerTeam*2)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameInstance.Numplayers=%d"), gameinstance->NumPlayersPerTeam);
			GetWorldTimerManager().SetTimer(GameStartTimer, this, &APuzzlePlatformsGameMode::StartGame, 5);
		}
	}	
}

void APuzzlePlatformsGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void APuzzlePlatformsGameMode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("GMode:StartGame invoked"));
	auto GameInstance=Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	if(GameInstance==nullptr) return;

	GameInstance->StartSession();

	UWorld* World=GetWorld();
	if(!ensure(World!=nullptr)) return;

	bUseSeamlessTravel=true;
	World->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");


}
