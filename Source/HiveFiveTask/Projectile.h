#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class HIVEFIVETASK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectile();
	

protected:

	virtual void BeginPlay() override;

private:	

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION()
	void Onhit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
