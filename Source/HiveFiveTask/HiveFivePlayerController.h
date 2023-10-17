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

protected:

	virtual void OnRep_Pawn() override;

private:

	void FixSimpleMoveToLocation();

	
};
