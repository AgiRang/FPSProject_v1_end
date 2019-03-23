// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProjectile.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"



// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//구체를 단순 콜리전 표현
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(15.0f);

	

	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	
	RootComponent = CollisionComponent;

	

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = 1;
	ProjectileMovementComponent->bShouldBounce = 1;
	ProjectileMovementComponent->Bounciness = 0.3f;

	//Bullet Life Time
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}




//Function that appear when something hits a projecttile
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponenet, FVector NormalInpulse, const FHitResult&Hit)
{
	if (OtherActor != this && OtherComponenet->IsSimulatingPhysics())
	{
		OtherComponenet->AddImpulseAtLocation(ProjectileMovementComponent->Velocity *100.0f, Hit.ImpactPoint);
	}
}