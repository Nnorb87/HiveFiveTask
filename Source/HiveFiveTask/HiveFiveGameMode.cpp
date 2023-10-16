// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveGameMode.h"
#include "HiveFiveCharacter.h"
#include "HiveFivePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" 
#include "GameFramework/PlayerState.h"
#include "HiveFiveGameState.h"
#include "HiveFivePlayerState.h"
#include "HiveFiveHUD.h"
#include "ScoreBoardUserWidget.h"

void AHiveFiveGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if(!HasAuthority()){return;}

    UE_LOG(LogTemp, Warning, TEXT("PostLogin"));

    if ( AHiveFivePlayerState* HiveFivePlayerState = Cast<AHiveFivePlayerState>(NewPlayer->PlayerState)){
        FString PlayerName = HiveFivePlayerState->GetPlayerName();
        int32 PlayerScore = HiveFivePlayerState->GetPlayerScore();
        UE_LOG(LogTemp, Warning, TEXT("Player Name: %s, Player Score: %d"), *PlayerName, PlayerScore);
        if (AHiveFiveGameState* HFGameState = GetGameState<AHiveFiveGameState>()){
            HFGameState->StoreClientNames(PlayerName, FString::Printf(TEXT("%d"), PlayerScore));
        }
    }
}

void AHiveFiveGameMode::RequestRespawn(ACharacter *ElimmedCharacter, AController *ElimmedController){
    if (ElimmedCharacter){
        ElimmedCharacter->Reset();
        ElimmedCharacter->Destroy();
    }

    if (ElimmedController){
        TArray<AActor*> PlayerStarts;
        UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
        Selection = FMath::RandRange(0,PlayerStarts.Num()-1);
        while (true) {
            if (IsThereAnyCollision(PlayerStarts[Selection]->GetActorLocation(), 100.f)){
                Selection = FMath::RandRange(0,PlayerStarts.Num()-1);
            } else {
                break;
            }
        }    
        RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]); 
    }
}

bool AHiveFiveGameMode::IsThereAnyCollision(const FVector& SphereCenter, float SphereRadius)
{
    FCollisionQueryParams CollisionParams;
    TArray<FHitResult> OverlappingActors;
    UWorld* World = GetWorld();
    if (World->SweepMultiByChannel(OverlappingActors, SphereCenter, SphereCenter, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), CollisionParams)){
        DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, false, 5.f, 0, 1);
        return true;
    }
    DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Green, false, 5.f, 0, 1);
    return false;
}