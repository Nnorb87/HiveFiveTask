#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HiveFiveCharacter.generated.h"

UCLASS()
class HIVEFIVETASK_API AHiveFiveCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AHiveFiveCharacter();

	void PlayerRespawn();

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, category = "Movement")
	float MovementSpeed = 600.f;

	UPROPERTY(EditAnywhere, category = "Movement")
	float TurnRate = 120.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	class UCharacterMovementComponent* CharacterMovementComponent;

	FVector TargetVector = FVector::ZeroVector;
	
	APlayerController* PlayerControllerRef;
	
	void MoveClick();

	void MoveToLocation(AController* PlayerController, const FVector& TargetLocation);

	FVector FindNearestNavMeshPoint(const FVector& Location);

	bool CanPlayerMove();

	void Fire();

	UFUNCTION(Server, Reliable)
	void ServerFire();

	void SpawnProjectile();
};
