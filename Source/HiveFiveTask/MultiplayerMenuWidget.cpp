#include "MultiplayerMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "GameFramework/PlayerController.h"
#include "HiveFiveGameMode.h"
#include "Components/Border.h"


void UMultiplayerMenuWidget::NativeConstruct(){
    Super::NativeConstruct();

    if (HostGameButton){
        HostGameButton->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnClickHost);
    }
     if (JoinGameButton){
        JoinGameButton->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnClickJoin);
    }
     if (ExitGameButton){
        ExitGameButton->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnClickExit);
    }

    if (GameOverText){
        GameOverText->SetVisibility(ESlateVisibility::Hidden);
    }

     if (GameOverBorder){
        GameOverBorder->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UMultiplayerMenuWidget::OnClickHost(){   
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)    {
        AHiveFiveGameMode* HFGameMode = Cast<AHiveFiveGameMode>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
        if (HFGameMode){
            HFGameMode->HostServer();
        }
    }
}

void UMultiplayerMenuWidget::OnClickJoin(){

    FString ServerIP = IpAddressInputBlock->GetText().ToString();
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)    {
        AHiveFiveGameMode* HFGameMode = Cast<AHiveFiveGameMode>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
        if (HFGameMode){
            HFGameMode->JoinToHost(ServerIP);
        }
    }
}

void UMultiplayerMenuWidget::OnClickExit(){
    APlayerController* PlayerController = GetOwningPlayer();

    if (PlayerController){
        PlayerController->ConsoleCommand("quit");
    }
}

void UMultiplayerMenuWidget::ShowGameOver(FString PlayerName){
    if (GameOverText){
        GameOverText->SetVisibility(ESlateVisibility::Visible);
        GameOverBorder->SetVisibility(ESlateVisibility::Visible);
        GameOverText->SetText(FText::FromString(PlayerName));
    }
}