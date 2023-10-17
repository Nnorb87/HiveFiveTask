#include "HiveFivePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "HiveFivePlayerController.h"
#include "HiveFiveHUD.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "HiveFivePlayerState.h"
#include "HiveFiveGameState.h"
#include "DrawDebugHelpers.h"
#include "ScoreBoardUserWidget.h"

AHiveFivePlayerState::AHiveFivePlayerState(){
		SetPlayerScore(0);
 }

void AHiveFivePlayerState::BeginPlay(){
	AHiveFiveGameState* HFGameState = Cast<AHiveFiveGameState>(GetWorld()->GetGameState());
    if (HFGameState){
        InitialUpdateHUD(HFGameState->GetPlayerNames());
	}
}

void AHiveFivePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHiveFivePlayerState, PlayerScore);	
}

void AHiveFivePlayerState::OnRepScore(){
	// On Change of PlayerScore Client Updates thier HUD
	UpdateHUD();
}

void AHiveFivePlayerState::UpdateScore(){
		// Server Call from Projectile it increases PlayerScore by 1 and initiates OnRepScore so clients will syncronize thier HUD-s
		IncreaseScore();
		UpdateHUD();
}

void AHiveFivePlayerState::UpdateHUD(){
	//Updates the HUD if the player kills an enemy get called from Projectile/Onhit through UpdateScore
		if (AHiveFivePlayerController* HiveFivePlayerController =Cast<AHiveFivePlayerController>(GetWorld()->GetFirstPlayerController())){
			
			if (AHiveFiveHUD* HUD = Cast<AHiveFiveHUD>(HiveFivePlayerController->GetHUD())) {

				if(UScoreBoardUserWidget* ScoreBoard = HUD->GetScoreBoard()){
					ScoreBoard->SetPlayerData(GetPlayerName(), FString::Printf(TEXT("%d"), PlayerScore));
				}
			}
		}
}

void AHiveFivePlayerState::InitialUpdateHUD(TArray<FString> PlayerNamesArray){
	//Sets the HUD StartingState, fills up the HUD with all of the player's names based on the PlayerNamesArray lists and sets thier score to 0
	//Will be called From GameState/IterateOverConnectedPlayers() and from PlayerState/BeginPlay()
	if (AHiveFivePlayerController* HiveFivePlayerController =Cast<AHiveFivePlayerController>(GetWorld()->GetFirstPlayerController())){
		
		if (AHiveFiveHUD* HUD = Cast<AHiveFiveHUD>(HiveFivePlayerController->GetHUD())) {

			if(UScoreBoardUserWidget* ScoreBoard = HUD->GetScoreBoard()){
				for(FString PN : PlayerNamesArray){
					ScoreBoard->SetPlayerData(PN, FString::Printf(TEXT("%d"), 0));
				}
				
			}
		}
	}

}