#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HiveFivePlayerState.generated.h"

UCLASS()
class HIVEFIVETASK_API AHiveFivePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
 	AHiveFivePlayerState();
	void IncreaseScore(){++PlayerScore;}
	int32 GetPlayerScore(){return PlayerScore;}
	void SetPlayerScore(int32 NewScore){ PlayerScore = NewScore;}
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void UpdateScore();

	void InitialUpdateHUD(TArray<FString> PlayerNamesArray);

	int32 GetDisplayIndex(FString PName, FString PScore);

	UPROPERTY(ReplicatedUsing = OnRepScore)
	int32 PlayerScore;

	UFUNCTION()
	void OnRepScore();

	void UpdateHUD();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};
