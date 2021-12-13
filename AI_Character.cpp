// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AAI_Character::AAI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxAcceleration = 420.0f;

	HitPoints = 100;
	IsDead = false;
}

// Called when the game starts or when spawned
void AAI_Character::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	
}

// Called every frame
void AAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HitPoints <= 0)
	{
		if (!IsDead)
		{
			IsDead = true;

			GetMesh()->SetSimulatePhysics(true);
		}
	}

	
}

// Called to bind functionality to input
void AAI_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AAI_Character::TakeDamage()
{
	HitPoints -= 10;
	if (HitPoints <= 0)
	{
		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::FromInt(HitPoints));
}

