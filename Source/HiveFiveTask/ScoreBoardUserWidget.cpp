// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreBoardUserWidget.h"
#include "Components/TextBlock.h"



void UScoreBoardUserWidget::SetPlayerName(FString NewText) {
    if (PlayerName){
        FText Text = FText::FromString(NewText);
        PlayerName->SetText(Text);
    }
}

void UScoreBoardUserWidget::SetPlayerScore(FString NewText) {
    if (PlayerScore){
        FText Text = FText::FromString(NewText);
        PlayerScore->SetText(Text);
        UE_LOG(LogTemp, Warning, TEXT("%s"), *NewText);
    }
}
