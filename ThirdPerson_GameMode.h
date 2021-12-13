// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ThirdPerson_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_API AThirdPerson_GameMode : public AGameMode
{
	GENERATED_BODY()

	AThirdPerson_GameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APawn> PlayableCharacter;
	float Spawn_Z = 500.0f;
	UPROPERTY(EditAnywhere)
		float Spawn_X_Min;
	UPROPERTY(EditAnywhere)
		float Spawn_X_Max;
	UPROPERTY(EditAnywhere)
		float Spawn_Y_Min;
	UPROPERTY(EditAnywhere)
		float Spawn_Y_Max;
	void SpawnPlayableCharacter();
};
