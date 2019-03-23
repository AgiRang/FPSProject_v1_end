// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "FPSProject.h"
#include "FPSProjectile.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Camera 1 / Attach CapsuleComponent in CameraComponent
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());

	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = 1;

	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First person Mesh"));
	FPSMesh->SetOnlyOwnerSee(1);
	FPSMesh->SetupAttachment(FPSCameraComponent);
	FPSMesh->bCastDynamicShadow = 0;
	FPSMesh->CastShadow = 0;

	GetMesh()->SetOwnerNoSee(1);

}

void AFPSCharacter:: SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("It is U"));
	
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSCharacter::MoveForward(float Value)
{
	//전방위치 확인
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}
void AFPSCharacter::MoveRight(float value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump=1;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = 0;
}

void AFPSCharacter::Fire()
{
	if (ProjectileClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotaition = CameraRotation;

		//aming
		MuzzleRotaition.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			//bullet spawn
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass,MuzzleLocation,MuzzleRotaition,SpawnParams);
			if (Projectile)
			{
				FVector LunchDirection = MuzzleRotaition.Vector();
					Projectile->FireInDirection(LunchDirection);
			}
			}
	}
}

