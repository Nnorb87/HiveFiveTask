// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HiveFiveGameState.generated.h"

UCLASS()
class HIVEFIVETASK_API AHiveFiveGameState : public AGameState
{
	GENERATED_BODY()

public:

	UPROPERTY(ReplicatedUsing = OnNameUpdate)
	TArray<FString> PlayerNamesArray;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	UFUNCTION()
	void OnNameUpdate();

	void StoreClientNames(FString PName, FString PScore);

	void IterateOverConnectedPlayers();

	TArray<FString> GetPlayerNames();

protected:
	virtual void BeginPlay() override;


};

