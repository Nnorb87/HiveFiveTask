// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBoardUserWidget.h"
#include "HiveFiveHUD.generated.h"



/**
 * 
 */
UCLASS()
class HIVEFIVETASK_API AHiveFiveHUD : public AHUD
{
	GENERATED_BODY()

public:
	UScoreBoardUserWidget* GetScoreBoard(){return ScoreBoardWidget;}

protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScoreBoardUserWidget> ScoreBoardWidgetClass;

	UScoreBoardUserWidget* ScoreBoardWidget;

};
