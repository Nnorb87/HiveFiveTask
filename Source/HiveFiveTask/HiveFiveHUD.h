#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "HiveFiveHUD.generated.h"

UCLASS()
class HIVEFIVETASK_API AHiveFiveHUD : public AHUD
{
	GENERATED_BODY()

public:
	class UScoreBoardUserWidget* GetScoreBoard(){return ScoreBoardWidget;}

protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UScoreBoardUserWidget> ScoreBoardWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMultiplayerMenuWidget> MultiPlayerMenuWidgetClass;

	class UScoreBoardUserWidget* ScoreBoardWidget;

	class UMultiplayerMenuWidget* MultiPlayerMenuWidget;

	void CreateScoreBoardWidget();

	void CreateMultiplayerMenuWidget();

};
