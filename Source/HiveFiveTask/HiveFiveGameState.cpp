// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveGameState.h"
#include "Net/UnrealNetwork.h"
#include "HiveFivePlayerController.h"
#include "HiveFiveHUD.h"
#include "GameFramework/Pawn.h"


void AHiveFiveGameState::BeginPlay(){
	Super::BeginPlay();

    SetScore(0);
}

void AHiveFiveGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHiveFiveGameState, Score);
}

void AHiveFiveGameState::OnRepScore(){
   UpdateScore();
}

void AHiveFiveGameState::UpdateScore(){
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	AHiveFivePlayerController* HiveFivePlayerController = Cast<AHiveFivePlayerController>(PlayerController);
	if (PlayerController) {
		AHiveFiveHUD* HUD = Cast<AHiveFiveHUD>(HiveFivePlayerController->GetHUD());
		if (HUD) {
			HUD->GetScoreBoard()->SetPlayerScore(FString::Printf(TEXT("%d"), Score));
			UE_LOG(LogTemp, Warning, TEXT("%d"), Score);
		}
	}
}