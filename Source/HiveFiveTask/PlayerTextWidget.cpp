// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTextWidget.h"
#include "Components/TextBlock.h"

void UPlayerTextWidget::SetPlayerName(FString NewText) {
    if (PlayerName){
        FText Text = FText::FromString(NewText);
        PlayerName->SetText(Text);
    }
}


void UPlayerTextWidget::SetPlayerScore(FString NewText) {
    if (PlayerScore){
        FText Text = FText::FromString(NewText);
        PlayerScore->SetText(Text);
    }
}
