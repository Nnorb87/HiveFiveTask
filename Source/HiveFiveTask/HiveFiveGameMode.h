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

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinToHost(const FString& Address);

protected:

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:

	int32 Selection;

	bool IsThereAnyCollision(const FVector& SphereCenter, float SphereRadius);

};
