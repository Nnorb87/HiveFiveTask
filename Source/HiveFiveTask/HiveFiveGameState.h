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

	void StoreClientNames(FString PName, FString PScore);

	TArray<FString> GetPlayerNames();

	void CheckPlayerScores();

protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:

	UFUNCTION()
	void OnNameUpdate();

	void IterateOverConnectedPlayers();

	UPROPERTY(ReplicatedUsing = OnNameUpdate)
	TArray<FString> PlayerNamesArray;

	UPROPERTY(EditAnyWhere)
	int32 KillGoal = 10;

	void EndTheGame(const FString& PlayerName);

	UFUNCTION(NetMulticast, Reliable)
	void MultiEndTheGame(const FString& PlayerName);

};

