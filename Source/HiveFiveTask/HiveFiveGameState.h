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
virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
void IncreaseScore(){Score += 1;}
void GetScore(){Score;}
void SetScore(int32 NewScore){ Score = NewScore;}
void UpdateScore();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(ReplicatedUsing = OnRepScore )
	int32 Score;

	UFUNCTION()
	void OnRepScore();
};

