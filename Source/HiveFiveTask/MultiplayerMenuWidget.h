#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerMenuWidget.generated.h"

UCLASS()
class HIVEFIVETASK_API UMultiplayerMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowGameOver(FString PlayerName);

protected:

    virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGameButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* IpAddressInputBlock;

	UPROPERTY(meta = (BindWidget))
	class UBorder* IpAddressInputBorder;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameOverText;

	UPROPERTY(meta = (BindWidget))
	class UBorder* GameOverBorder;


	UFUNCTION()
	void OnClickHost();

	UFUNCTION()
	void OnClickJoin();

	UFUNCTION()
	void OnClickExit();
};
