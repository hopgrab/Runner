 // Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include <Runtime\UMG\Private\UserWidget.cpp>




// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	MyController = CreateDefaultSubobject<APlayerController>(TEXT("MyController"));
	MyController->bShowMouseCursor = false;
	MyController->bEnableClickEvents = false;
	MyController->bEnableMouseOverEvents = false;

	
	
	IsDead = false;
	Health = 100.0f;
	
	
}




// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnBeginOverlap);
	

	if (Player_Health_Widget_Class != nullptr)
	{
		Player_Health_Widget = CreateWidget(GetWorld(), Player_Health_Widget_Class);
		Player_Health_Widget->AddToViewport(1);

	}
	
	if (Escape_Widget_Class_XD != nullptr)
	{
		Escape_Widget_XD = CreateWidget(GetWorld(), Escape_Widget_Class_XD);
	}

	if (Melee_Weapon)
	{
		Melee_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Health -= DeltaTime * Health_Threshhold;

	if (Health <= 0)
	{

		if (!IsDead)
		{
			IsDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainCharacter::RestartGame, 3.0f, false);

		}
	}

	if (swordBox && swordBox->attacking)
	{
		if (GetMesh()->IsPlaying() == false)
		{
			swordBox->attacking = false;
			GetMesh()->SetAnimClass(AnimBp);
		}
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &AMainCharacter::Attack);
	
	FInputActionBinding& toggle = PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &AMainCharacter::Escape);
	toggle.bExecuteWhenPaused = true;
}



void AMainCharacter::MoveForward(float Axis)
{
	if (!IsDead) 
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void AMainCharacter::MoveRight(float Axis)
{
	if (!IsDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void AMainCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AMainCharacter::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Heal"))
	{
		Health += 10.0f;

		if (Health > 100.0f)
			Health = 100.0f;

		OtherActor->Destroy();
	}
	if (OtherActor->ActorHasTag("Enemy"))
	{
		Health -= 10.0f;
	}
}



void AMainCharacter::Attack()
{
	if (Melee_Weapon)
	{
		
		GetMesh()->SetAnimation(slice);
		GetMesh()->PlayAnimation(slice, false);
		if (swordBox)
		{
			swordBox->attacking = true;
		}
		
	}

}

void AMainCharacter::Escape()
{
	Escape_Widget_XD->AddToViewport(2);
	if (Escape_Widget_XD->IsInViewport())
	{
		MyController->bShowMouseCursor = true;
		MyController->bEnableClickEvents = true;
		MyController->bEnableMouseOverEvents = true;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

	if (!Escape_Widget_XD->IsInViewport())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		MyController->bShowMouseCursor = false;
		MyController->bEnableClickEvents = false;
		MyController->bEnableMouseOverEvents = false;
	}
}

