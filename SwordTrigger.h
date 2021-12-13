// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "AI_Character.h"
#include "SwordTrigger.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_API ASwordTrigger : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ASwordTrigger();

	UFUNCTION()
		void Attack(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY()
		bool attacking;
};
