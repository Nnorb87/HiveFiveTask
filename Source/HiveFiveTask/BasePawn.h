// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class HIVEFIVETASK_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void ServerMoveToLocation(AController* PlayerController, const FVector& TargetLocation);

	UFUNCTION(Server, Reliable)
	void ServerMove(float Value);

	UFUNCTION(Server, Reliable)
	void ServerTurn(float Value);

	UFUNCTION(Server, Reliable)
	void ServerRotate(FVector LookAtTarget);



protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, category = "Movement")
	float MovementSpeed = 500.f;

	UPROPERTY(EditAnywhere, category = "Movement")
	float TurnRate = 45.f;

	FVector TargetVector = FVector::ZeroVector;
	APlayerController* PlayerControllerRef;

	void Move(float Value);
	void Turn(float Value);
	void Fire();
	void MoveClick();
	
	void MoveToLocation(AController* PlayerController, const FVector& TargetLocation);
	FVector FindNearestNavMeshPoint(const FVector& Location);
	void Rotate(FVector LookAtTarget);
	bool AutoRotation = false;
};
