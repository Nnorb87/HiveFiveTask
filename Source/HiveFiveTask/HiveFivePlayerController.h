// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HiveFivePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HIVEFIVETASK_API AHiveFivePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void FixSimpleMoveToLocation();

	virtual void OnRep_Pawn() override;
	
};
