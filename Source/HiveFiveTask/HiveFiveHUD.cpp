// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveHUD.h"
#include "Blueprint/UserWidget.h"
#include "HiveFivePlayerController.h"
#include "GameFramework/Pawn.h"
#include "ScoreBoardUserWidget.h"
#include "MultiplayerMenuWidget.h"

void AHiveFiveHUD::BeginPlay(){
    Super::BeginPlay();
    CreateScoreBoardWidget();
    CreateMultiplayerMenuWidget();
}

void AHiveFiveHUD::CreateScoreBoardWidget(){
     if (ScoreBoardWidgetClass) {
        AHiveFivePlayerController* HFPC = Cast<AHiveFivePlayerController>(GetOwningPlayerController());
        if(HFPC){
            ScoreBoardWidget = CreateWidget<UScoreBoardUserWidget>(HFPC,ScoreBoardWidgetClass);
            if (ScoreBoardWidget){
                ScoreBoardWidget->AddToPlayerScreen();
            }
        }
    }
}

void AHiveFiveHUD::CreateMultiplayerMenuWidget(){
    if (MultiPlayerMenuWidgetClass) {
        AHiveFivePlayerController* HFPC = Cast<AHiveFivePlayerController>(GetOwningPlayerController());
        if(HFPC){
            MultiPlayerMenuWidget = CreateWidget<UMultiplayerMenuWidget>(HFPC,MultiPlayerMenuWidgetClass);
            if (MultiPlayerMenuWidget){
                MultiPlayerMenuWidget->AddToPlayerScreen();
            }
        }
    }
}
