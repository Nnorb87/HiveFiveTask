// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveGameMode.h"
#include "HiveFiveCharacter.h"
#include "HiveFivePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" 



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
    FColor DebugSphereColor = FColor::Green; 
    // DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, DebugSphereColor, false, 5.f, 0, 1);
    FCollisionQueryParams CollisionParams;
    TArray<FHitResult> OverlappingActors;
    UWorld* World = GetWorld();
    if (World->SweepMultiByChannel(OverlappingActors, SphereCenter, SphereCenter, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereRadius), CollisionParams)){
        return true;
    }
    return false;
}