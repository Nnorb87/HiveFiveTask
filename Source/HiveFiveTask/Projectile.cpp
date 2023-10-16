#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HiveFiveCharacter.h"
#include "GameFramework/PlayerController.h"
#include "HiveFivePlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "HiveFiveGameState.h"
#include "HiveFivePlayerState.h"

AProjectile::AProjectile(){
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 5000.f;
	ProjectileMovementComponent->MaxSpeed = 5000.f;
}

void AProjectile::BeginPlay(){
	Super::BeginPlay();
	if(HasAuthority()){
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::Onhit);
	}
	
}

void AProjectile::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AProjectile::Onhit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
	if(AHiveFiveCharacter* HiveFiveCharacter = Cast<AHiveFiveCharacter>(OtherActor)){
		AHiveFiveCharacter* ProjectileOwnerCharacter = Cast<AHiveFiveCharacter>(GetOwner());

            if (APlayerController* PlayerController = Cast<APlayerController>(ProjectileOwnerCharacter->GetController())){
				
				if(AHiveFivePlayerState* HiveFivePlayerState = Cast<AHiveFivePlayerState>(PlayerController->PlayerState)){
					HiveFivePlayerState->UpdateScore();
				}
            }
		HiveFiveCharacter->PlayerRespawn();
		}
	Destroy();
}
