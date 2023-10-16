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

    SetIpAddressShowBlock();
}

void UMultiplayerMenuWidget::OnClickHost(){   
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)    {
        AHiveFiveGameMode* HFGameMode = Cast<AHiveFiveGameMode>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
        if (HFGameMode){
            HFGameMode->OpenLobby();
        }
    }
}

void UMultiplayerMenuWidget::OnClickJoin(){

    FString ServerIP = IpAddressInputBlock->GetText().ToString();
    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)    {
        AHiveFiveGameMode* HFGameMode = Cast<AHiveFiveGameMode>(UGameplayStatics::GetGameMode(PlayerController->GetWorld()));
        if (HFGameMode){
            HFGameMode->CallClientTravel(ServerIP);
        }
    }
}

void UMultiplayerMenuWidget::OnClickExit(){
    APlayerController* PlayerController = GetOwningPlayer();

    if (PlayerController){
        PlayerController->ConsoleCommand("quit");
    }
}

void UMultiplayerMenuWidget::SetIpAddressShowBlock(){
}
