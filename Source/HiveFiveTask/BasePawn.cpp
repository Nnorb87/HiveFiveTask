#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "NavigationSystem.h"

ABasePawn::ABasePawn(){
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(BaseMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm -> SetupAttachment(RootComponent);
	SpringArm -> TargetArmLength = 2000.0f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm);
}

void ABasePawn::BeginPlay(){
	Super::BeginPlay();
	PlayerControllerRef = Cast<APlayerController>(GetController());

	if (PlayerControllerRef)
    {
        PlayerControllerRef->bShowMouseCursor = true;
        PlayerControllerRef->bEnableClickEvents = true;
        PlayerControllerRef->bEnableMouseOverEvents = true;
    }
}

void ABasePawn::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	if(AutoRotation){
		Rotate(TargetVector);
	}

}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABasePawn::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABasePawn::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed, this, &ABasePawn::Fire);
	PlayerInputComponent->BindAction(TEXT("MoveClick"),IE_Pressed, this, &ABasePawn::MoveClick);
}

void ABasePawn::Move(float Value){
	if (FMath::Abs(Value)>0.f){ 
		AutoRotation = false;
	}
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * MovementSpeed;
	AddActorLocalOffset(DeltaLocation, true);
}

void ABasePawn::Turn(float Value){
	if (FMath::Abs(Value)>0.f){ 
		AutoRotation = false;
	}
	FRotator DetlaRotation = FRotator::ZeroRotator;
	DetlaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
	AddActorLocalRotation(DetlaRotation, true);

}

void ABasePawn::MoveClick(){
	AutoRotation = true;
	FHitResult HitResult;

	if(PlayerControllerRef){
		PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);

		DrawDebugSphere(
		GetWorld(),
		HitResult.ImpactPoint,
		10.f,
		12,
		FColor::Red,
		false,
		5.f
		);
	}

	TargetVector = HitResult.ImpactPoint;

	FVector NearestNavMeshPoint = FindNearestNavMeshPoint(TargetVector);
	MoveToLocation(NearestNavMeshPoint);
}

void ABasePawn::MoveToLocation(const FVector& TargetLocation){
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), TargetLocation);
}

FVector ABasePawn::FindNearestNavMeshPoint(const FVector& Location){
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSystem){
        FNavLocation NearestNavLocation;
        if (NavSystem->ProjectPointToNavigation(Location, NearestNavLocation)){
            return NearestNavLocation.Location;
        }
    }
    return Location;
}

void ABasePawn::Rotate(FVector LookAtTarget){
	FVector ToTarget = LookAtTarget - BaseMesh->GetComponentLocation();
	FRotator LookAtRotation = ToTarget.Rotation();
	LookAtRotation.Roll = 0.f;
	LookAtRotation.Pitch = 0.f;
	RootComponent->SetWorldRotation(
		FMath::RInterpTo(
			BaseMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			5.f
		)
	);

}

void ABasePawn::Fire(){
	if (GEngine){
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Blue,
			FString::Printf(TEXT("Fire"))
		);
	}

}
