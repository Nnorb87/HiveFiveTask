// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HiveFiveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HIVEFIVETASK_API AHiveFiveGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	void RequestRespawn(class ACharacter* ElimCharacter, AController* ElimmedController);
	bool IsThereAnyCollision(const FVector& SphereCenter, float SphereRadius);
    virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void OpenLobby();

	UFUNCTION()
	void CallOpenLevel(const FString& Address);

	UFUNCTION()
	void CallClientTravel(const FString& Address);

private:
	int32 Selection;



};
