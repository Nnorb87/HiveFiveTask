#include "HiveFiveCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"

AHiveFiveCharacter::AHiveFiveCharacter(){
	PrimaryActorTick.bCanEverTick = true;

	bReplicates =true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

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

	// CapsuleComponent = FindComponentByClass<UCapsuleComponent>();

}

void AHiveFiveCharacter::BeginPlay(){
	Super::BeginPlay();
	PlayerControllerRef = Cast<APlayerController>(GetController());

	if (PlayerControllerRef)
    {
        PlayerControllerRef->bShowMouseCursor = true;
        PlayerControllerRef->bEnableClickEvents = true;
        PlayerControllerRef->bEnableMouseOverEvents = true;
    }
}

void AHiveFiveCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("%d"),AutoRotation);
	if (AutoRotation){
		Rotate(TargetVector);
	}
}

void AHiveFiveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHiveFiveCharacter::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHiveFiveCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed, this, &AHiveFiveCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("MoveClick"),IE_Pressed, this, &AHiveFiveCharacter::MoveClick);

}

void AHiveFiveCharacter::Rotate(FVector LookAtTarget){
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
	if(!HasAuthority()) {
		ServerRotate(LookAtTarget);
	}
}

void AHiveFiveCharacter::MoveClick(){
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

	MoveToLocation(GetController(),TargetVector);
	
	//  if (!HasAuthority()) {     
    //         ServerMoveToLocation(GetController(), TargetVector); 
    //     }
}

void AHiveFiveCharacter::MoveToLocation(AController* PlayerController, const FVector& TargetLocation){
	FVector NearestNavMeshPoint = FindNearestNavMeshPoint(TargetVector);
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, NearestNavMeshPoint);
}

FVector AHiveFiveCharacter::FindNearestNavMeshPoint(const FVector& Location){
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSystem){
        FNavLocation NearestNavLocation;
        if (NavSystem->ProjectPointToNavigation(Location, NearestNavLocation)){
            return NearestNavLocation.Location;
        }
    }
    return Location;
}

void AHiveFiveCharacter::Move(float Value){
	if (FMath::Abs(Value)>0.f){
		AutoRotation = false;
	}

	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * MovementSpeed;
	AddActorLocalOffset(DeltaLocation, true);

	 if(!HasAuthority()) {
		ServerMovement(Value);
	}
}

void AHiveFiveCharacter::Turn(float Value){
	if (FMath::Abs(Value)>0.f){
		AutoRotation = false;
	}
	FRotator DetlaRotation = FRotator::ZeroRotator;
	DetlaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
	AddActorLocalRotation(DetlaRotation, true);

	if(!HasAuthority()) {
		ServerTurn(Value);
	}

}

void AHiveFiveCharacter::Fire(){
	ServerFire();	
}

void AHiveFiveCharacter::ServerRotate_Implementation(FVector LookAtTarget){
	Rotate(LookAtTarget);
}

void AHiveFiveCharacter::ServerMovement_Implementation(float Value){
	Move(Value);
}

void AHiveFiveCharacter::ServerTurn_Implementation(float Value){
	Turn(Value);
}

void AHiveFiveCharacter::ServerFire_Implementation(){
	MulticastFire();
}

void AHiveFiveCharacter::MulticastFire_Implementation(){
	if(ProjectileClass && ProjectileSpawnPoint){
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		if(World){
			World->SpawnActor<AProjectile>(
				ProjectileClass,
				ProjectileSpawnPoint->GetComponentTransform(),
				SpawnParams
			);
		}
	}
}