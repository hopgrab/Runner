// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordTrigger.h"

ASwordTrigger::ASwordTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &ASwordTrigger::Attack);

	attacking = false;
}

void ASwordTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void ASwordTrigger::Attack(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AAI_Character* enemy = Cast<AAI_Character>(otherActor);
		if (enemy && attacking)
		{
			enemy->TakeDamage();
		}
	}
}