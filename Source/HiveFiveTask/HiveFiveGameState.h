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
	void IterateOverConnectedPlayers();

protected:
	virtual void BeginPlay() override;


};

