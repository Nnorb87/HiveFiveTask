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

void AHiveFiveCharacter::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AHiveFiveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHiveFiveCharacter::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHiveFiveCharacter::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"),IE_Pressed, this, &AHiveFiveCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("MoveClick"),IE_Pressed, this, &AHiveFiveCharacter::MoveClick);

}

// Movement with Mouse Click
void AHiveFiveCharacter::MoveClick(){
		FHitResult HitResult;
		PlayerControllerRef = Cast<APlayerController>(GetController());
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

//WASD Movement Will be deleted
void AHiveFiveCharacter::Move(float Value){
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * MovementSpeed;
	AddActorLocalOffset(DeltaLocation, true);

	 if(!HasAuthority()) {
		ServerMovement(Value);
	}
}

void AHiveFiveCharacter::ServerMovement_Implementation(float Value){
	Move(Value);
}

void AHiveFiveCharacter::Turn(float Value){
	FRotator DetlaRotation = FRotator::ZeroRotator;
	DetlaRotation.Yaw = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
	AddActorLocalRotation(DetlaRotation, true);

	if(!HasAuthority()) {
		ServerTurn(Value);
	}

}

void AHiveFiveCharacter::ServerTurn_Implementation(float Value){
	Turn(Value);
}

// Multiplayer joining part for builds
void AHiveFiveCharacter::OpenLobby()
{
	UWorld* world = GetWorld();
	world->ServerTravel("/Game/Menu?listen");
	
		if(GEngine){
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Multiplayer level loaded"))
			);
		}

}

void AHiveFiveCharacter::CallOpenLevel(const FString& Address){
	UGameplayStatics::OpenLevel(this, *Address);
}

void AHiveFiveCharacter::CallClientTravel(const FString& Address){
	APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
	if(playerController){

		if(GEngine){
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Address: %s"),*Address)
			);
		}

		playerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}