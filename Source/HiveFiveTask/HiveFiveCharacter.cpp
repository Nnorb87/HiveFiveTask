#include "HiveFiveCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "GameFramework/PlayerStart.h"
#include "HiveFiveGameMode.h"
#include "HiveFivePlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "HiveFiveHUD.h"
#include "ScoreBoardUserWidget.h"
#include "HiveFivePlayerState.h"

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

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	CharacterMovementComponent = GetCharacterMovement();
	FRotator RotationSpeed = FRotator(0.f,0.f,360.f);
	CharacterMovementComponent->RotationRate = RotationSpeed;

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency= 33.f;
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

void AHiveFiveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed, this, &AHiveFiveCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("MoveClick"),IE_Pressed, this, &AHiveFiveCharacter::MoveClick);
}

// Movement with MouseClick
void AHiveFiveCharacter::MoveClick(){
	if (!CanPlayerMove()){return;}
	FHitResult HitResult;
	PlayerControllerRef = Cast<APlayerController>(GetController());
	if(PlayerControllerRef){
		PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult);

		DrawDebugSphere(
		GetWorld(),
		HitResult.ImpactPoint,
		10.f,
		12,
		FColor::Green,
		false,
		1.f
		);
	}

	TargetVector = HitResult.ImpactPoint;
	MoveToLocation(GetController(),TargetVector);
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

// Attack
void AHiveFiveCharacter::Fire(){
	if (!CanPlayerMove()){return;}
	if(HasAuthority()){
		SpawnProjectile();
	} else {
		ServerFire();
	}
	
}

void AHiveFiveCharacter::ServerFire_Implementation(){
	SpawnProjectile();
}

void AHiveFiveCharacter::SpawnProjectile(){
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

// Respawn
void AHiveFiveCharacter::PlayerRespawn(){
	AHiveFiveGameMode* HiveFiveGameMode = GetWorld()->GetAuthGameMode<AHiveFiveGameMode>();
	if (HiveFiveGameMode){
		HiveFiveGameMode->RequestRespawn(this, Controller);
	}
}

bool AHiveFiveCharacter::CanPlayerMove(){
	if (AHiveFivePlayerController* HFPlayerController = Cast<AHiveFivePlayerController>(GetController())){
		if (AHiveFivePlayerState* HFPlayerState = Cast<AHiveFivePlayerState>(HFPlayerController->PlayerState)){
			return HFPlayerState->GetPlayerCanMove();
		}
	}
	return false;
}