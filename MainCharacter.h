// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "SwordTrigger.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime\UMG\Private\UserWidget.cpp>
#include "Runtime/UMG/Public/UMG.h" 
#include "Runtime/UMG/Public/UMGStyle.h" 
#include "Runtime/UMG/Public/Slate/SObjectWidget.h" 
#include "Runtime/UMG/Public/IUMGModule.h" 
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include <iostream>
#include "MainCharacter.generated.h"

UCLASS()
class THIRDPERSON_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	bool IsDead;
	

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
		float Health;

	UPROPERTY(EditAnywhere)
		float Health_Threshhold;

	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> Player_Health_Widget_Class;
		UUserWidget* Player_Health_Widget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> Escape_Widget_Class_XD;
		UUserWidget* Escape_Widget_XD;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UClass* AnimBp;

	

	void RestartGame();
	void Escape();
	
	
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class APlayerController* MyController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

private:
	void Attack();
	

	UPROPERTY(EditAnywhere, Category = "Animation")
		AActor* Melee_Weapon;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimSequence* slice;
	UPROPERTY(EditAnywhere, Category = "Animation")
		ASwordTrigger* swordBox;
	
};
