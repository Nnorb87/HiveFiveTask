// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveGameState.h"
#include "HiveFiveGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "HiveFivePlayerState.h"


void AHiveFiveGameState::BeginPlay(){
	Super::BeginPlay(); 
}

void AHiveFiveGameState::IterateOverConnectedPlayers(){

    FDateTime CurrentTime = FDateTime::Now();
    FString CurrentTimeString = CurrentTime.ToString();
    UE_LOG(LogTemp, Warning, TEXT("Current Time: %s"), *CurrentTimeString);

    UE_LOG(LogTemp, Warning, TEXT("Running On Server"))
    for (APlayerState* PlayerState : PlayerArray){   
        AHiveFivePlayerState* HiveFivePlayerState = Cast<AHiveFivePlayerState>(PlayerState);

        FString PlayerName = HiveFivePlayerState->GetPlayerName();
        int32 PlayerScore = HiveFivePlayerState->GetPlayerScore();

        UE_LOG(LogTemp, Warning, TEXT("%s %d"), *PlayerName, PlayerScore);
    }
}