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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerRespawn();

	UFUNCTION(Blueprintcallable)
	void OpenLobby();

	UFUNCTION(Blueprintcallable)
	void CallOpenLevel(const FString& Address);

	UFUNCTION(Blueprintcallable)
	void CallClientTravel(const FString& Address);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
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


	void Move(float Value);

	UFUNCTION(Server, Reliable)
	void ServerMovement(float Value);

	void Turn(float Value);
	UFUNCTION(Server, Reliable)

	void ServerTurn(float Value);

	void Fire();

	void SpawnProjectile();

	UFUNCTION(Server, Reliable)
	void ServerFire();
};
