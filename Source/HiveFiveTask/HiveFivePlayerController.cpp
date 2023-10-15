// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFivePlayerController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/PlayerController.h"

void AHiveFivePlayerController::OnRep_Pawn(){
    Super::OnRep_Pawn();
    FixSimpleMoveToLocation();
}

void AHiveFivePlayerController::FixSimpleMoveToLocation(){
     if (!HasAuthority()){
        // UPathFollowingComponent registers only to server-only delegates for detect pawn changes.
        //Invoke the same functionality on the client-side.
        UPathFollowingComponent* PathFollowingComp = FindComponentByClass<UPathFollowingComponent>();
        if (PathFollowingComp){
            PathFollowingComp->UpdateCachedComponents();
        }
    }

}

