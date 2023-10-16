// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBoardUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIVEFIVETASK_API UScoreBoardUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerScore;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerName_1;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerScore_1;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerName_2;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerScore_2;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerName_3;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerScore_3;

	TArray<UTextBlock*> TextBlocks = {PlayerName,PlayerName_1,PlayerName_2,PlayerName_3};

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UVerticalBox * VerticalBox;

	void SetPlayerName(FString NewText);

	void SetPlayerScore(FString NewText);

	void SetPlayerData(FString Name, FString Score);

	void UpdateHUD(FString Name, FString Score);

	TArray<FString> PlayerNames;
};
