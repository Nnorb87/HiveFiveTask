// Fill out your copyright notice in the Description page of Project Settings.


#include "HiveFiveHUD.h"
#include "Blueprint/UserWidget.h"
#include "HiveFivePlayerController.h"
#include "GameFramework/Pawn.h"
#include "ScoreBoardUserWidget.h"

void AHiveFiveHUD::BeginPlay(){
    Super::BeginPlay();
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