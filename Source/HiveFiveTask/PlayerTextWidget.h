// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIVEFIVETASK_API UPlayerTextWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
	void SetPlayerName(FString NewText);

	void SetPlayerScore(FString NewText);

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UTextBlock* PlayerScore;
};
