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

protected:
	virtual void BeginPlay() override;

private:

	UCapsuleComponent* CapsuleComponent;

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

	FVector TargetVector = FVector::ZeroVector;
	APlayerController* PlayerControllerRef;
	bool AutoRotation = false;

	void Move(float Value);
	void Turn(float Value);
	void Fire();
	void MoveClick();
	void MoveToLocation(AController* PlayerController, const FVector& TargetLocation);
	void Rotate(FVector LookAtTarget);
	FVector FindNearestNavMeshPoint(const FVector& Location);
	
	UFUNCTION(Server, Reliable)
	void ServerRotate(FVector LookAtTarget);

	UFUNCTION(Server, Reliable)
	void ServerMovement(float Value);

	UFUNCTION(Server, Reliable)
	void ServerTurn(float Value);

	UFUNCTION(Server, Reliable)
	void ServerFire();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire();
};
