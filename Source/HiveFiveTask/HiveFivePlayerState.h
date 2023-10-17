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

	int32 GetPlayerScore(){return PlayerScore;}

	void SetPlayerScore(int32 NewScore){ PlayerScore = NewScore;}

	void UpdateScore();

	void InitialUpdateHUD(TArray<FString> PlayerNamesArray);

	void SetPlayerCanMove(bool Value){ bPlayerCanMove = Value;}

	bool GetPlayerCanMove(){return bPlayerCanMove;}

protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:

	void IncreaseScore(){++PlayerScore;}

	void UpdateHUD();

	UPROPERTY(ReplicatedUsing = OnRepScore)
	int32 PlayerScore;

	UFUNCTION()
	void OnRepScore();
	
	bool bPlayerCanMove = true;

};
