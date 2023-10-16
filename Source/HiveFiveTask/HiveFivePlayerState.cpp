#include "HiveFivePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "HiveFivePlayerController.h"
#include "HiveFiveHUD.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "DrawDebugHelpers.h"

AHiveFivePlayerState::AHiveFivePlayerState(){
		SetPlayerScore(0);
 }

void AHiveFivePlayerState::BeginPlay(){
}

void AHiveFivePlayerState::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AHiveFivePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHiveFivePlayerState, PlayerScore);
}

void AHiveFivePlayerState::OnRepScore(){
    // UE_LOG(LogTemp, Warning, TEXT("Client: %s %d"),*GetPlayerName(), PlayerScore);
	UpdateHUD();
}

void AHiveFivePlayerState::UpdateScore(){
		IncreaseScore();
		// UE_LOG(LogTemp, Warning, TEXT("Server: %s %d"),*GetPlayerName(), PlayerScore);
		UpdateHUD();
}

void AHiveFivePlayerState::UpdateHUD(){
		if (AHiveFivePlayerController* HiveFivePlayerController =Cast<AHiveFivePlayerController>(GetWorld()->GetFirstPlayerController())){
			
			if (AHiveFiveHUD* HUD = Cast<AHiveFiveHUD>(HiveFivePlayerController->GetHUD())) {

				if(UScoreBoardUserWidget* ScoreBoard = HUD->GetScoreBoard()){
					ScoreBoard->SetPlayerData(GetPlayerName(), FString::Printf(TEXT("%d"), PlayerScore));
				}
			}
		}
}

void AHiveFivePlayerState::InitialUpdateHUD(FString PName, FString PScore){
	if (!HasAuthority()){ return; }
	UE_LOG(LogTemp, Warning, TEXT("InitialUpdateHUD"));
	if (AHiveFivePlayerController* HiveFivePlayerController =Cast<AHiveFivePlayerController>(GetWorld()->GetFirstPlayerController())){	
	if (AHiveFiveHUD* HUD = Cast<AHiveFiveHUD>(HiveFivePlayerController->GetHUD())) {

		if(UScoreBoardUserWidget* ScoreBoard = HUD->GetScoreBoard()){
			ScoreBoard->SetPlayerData(PName, PScore);
		}
	}
}
}