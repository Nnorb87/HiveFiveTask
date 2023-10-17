#include "ScoreBoardUserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Actor.h"
#include "Components/VerticalBox.h"
#include "Net/UnrealNetwork.h"

void UScoreBoardUserWidget::SetPlayerData(FString Name, FString Score) {
    if (PlayerNames.Contains(Name)){
        UpdateHUD(Name, Score);
    } else{
        PlayerNames.Add(Name);     
        UpdateHUD(Name, Score); 
    }
}

void UScoreBoardUserWidget::UpdateHUD(FString Name, FString Score){
    int32 Index = PlayerNames.Find(Name);
    if (Index != INDEX_NONE) {
        switch (Index){
            case 0:
                PlayerName->SetText(FText::FromString(Name));
                PlayerScore->SetText(FText::FromString(Score));
                break;
            case 1:
                PlayerName_1->SetText(FText::FromString(Name));
                PlayerScore_1->SetText(FText::FromString(Score));
                break;
            case 2:
                PlayerName_2->SetText(FText::FromString(Name));
                PlayerScore_2->SetText(FText::FromString(Score));
                break;
            case 3:
                PlayerName_3->SetText(FText::FromString(Name));
                PlayerScore_3->SetText(FText::FromString(Score));
                break;
            default:
                break;
        }
        
    }else { 
        UE_LOG(LogTemp, Warning, TEXT("Invalid Index")); 
    }
}