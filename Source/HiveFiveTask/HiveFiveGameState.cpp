// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveGameState.h"
#include "HiveFiveGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "HiveFivePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "HiveFivePlayerController.h"
#include "HiveFiveHUD.h"
#include "MultiPlayerMenuWidget.h"
#include "HiveFiveCharacter.h"
#include "Kismet/GameplayStatics.h"

void AHiveFiveGameState::BeginPlay(){
	Super::BeginPlay(); 
}

void AHiveFiveGameState::StoreClientNames(FString PName, FString PScore){
	// Server calls it from GameMode
	PlayerNamesArray.Add(PName);
	IterateOverConnectedPlayers();
}

void AHiveFiveGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AHiveFiveGameState, PlayerNamesArray);
}

TArray<FString> AHiveFiveGameState::GetPlayerNames(){
	return PlayerNamesArray;
}

void AHiveFiveGameState::OnNameUpdate(){
	IterateOverConnectedPlayers();
}

void AHiveFiveGameState::IterateOverConnectedPlayers(){
    for (APlayerState* PlayerState : PlayerArray){   
        AHiveFivePlayerState* HiveFivePlayerState = Cast<AHiveFivePlayerState>(PlayerState);
		HiveFivePlayerState->InitialUpdateHUD(PlayerNamesArray);
    }
}

void AHiveFiveGameState::CheckPlayerScores(){
	// check if any of the player reached the KillGoal, if yes shows the game over widget and blocks player inputs
	if (!HasAuthority()) {return;}
	    for (APlayerState* PlayerState : PlayerArray){   
			AHiveFivePlayerState* HiveFivePlayerState = Cast<AHiveFivePlayerState>(PlayerState);
			if (HiveFivePlayerState->GetPlayerScore() >= KillGoal){
				FString PlayerName = HiveFivePlayerState->GetPlayerName();
				EndTheGame(PlayerName);
				MultiEndTheGame(PlayerName);			
			}
		}


}

void AHiveFiveGameState::EndTheGame(const FString& PlayerName){
	if (AHiveFivePlayerController* HiveFivePlayerController =Cast<AHiveFivePlayerController>(GetWorld()->GetFirstPlayerController())){
		if (AHiveFiveHUD* HUD = Cast<AHiveFiveHUD>(HiveFivePlayerController->GetHUD())) {
				if(UMultiplayerMenuWidget* MultiPlayerMenu = HUD->GetMultiPlayerMenu()){
					FString GameOverString = FString::Printf(TEXT("Game Over. %s won the game. Congratulations."), *PlayerName);
					MultiPlayerMenu->ShowGameOver(GameOverString);
					for (APlayerState* PlayerState : PlayerArray){   
						AHiveFivePlayerState* HFPlayerState = Cast<AHiveFivePlayerState>(PlayerState);
						HFPlayerState->SetPlayerCanMove(false);
					}
				}
								
			}
	}

}

void AHiveFiveGameState::MultiEndTheGame_Implementation(const FString& PlayerName){
	EndTheGame(PlayerName);
}