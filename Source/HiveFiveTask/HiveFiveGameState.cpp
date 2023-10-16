// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveGameState.h"
#include "HiveFiveGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "HiveFivePlayerState.h"
#include "Net/UnrealNetwork.h"


void AHiveFiveGameState::BeginPlay(){
	Super::BeginPlay(); 
}

void AHiveFiveGameState::StoreClientNames(FString PName, FString PScore){
	PlayerNamesArray.Add(PName);
	// for (const FString& Element : PlayerNamesArray){
	// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *Element);
	// }
	if (HasAuthority()){
		IterateOverConnectedPlayers();
	}
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