#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerMenuWidget.generated.h"

UCLASS()
class HIVEFIVETASK_API UMultiplayerMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGameButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* IpAddressShowBlock;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* IpAddressInputBlock;

	UPROPERTY(meta = (BindWidget))
	class UBorder* IpAddressShowBorder;

	UPROPERTY(meta = (BindWidget))
	class UBorder* IpAddressInputBorder;

	UFUNCTION()
	void OnClickHost();

	UFUNCTION()
	void OnClickJoin();

	UFUNCTION()
	void OnClickExit();

	void SetIpAddressShowBlock();

protected:
    virtual void NativeConstruct() override;

	
};
