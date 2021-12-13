// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "AI_Character.h"
#include "Waypoint.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AAI_Controller::AAI_Controller()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception_Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldofView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

}

void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();
	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Good Bruh"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not All Good Bruh"));
	}
}

void AAI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AAI_Controller::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AAI_Character* character = Cast<AAI_Character>(GetPawn());
	if (DistanceFromPlayer > AISightRadius)
	{
		IsPlayerDetected = false;
	}
	if (character->NextWaypoint != nullptr && IsPlayerDetected == false)
	{
		MoveToActor(character->NextWaypoint, 5.0f);
	}
	else if (IsPlayerDetected == true)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(Player, 5.0f);
	}
}

FRotator AAI_Controller::GetControlRotation() const
{
	if (GetPawn() == nullptr) 
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAI_Controller::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceFromPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
	}

	IsPlayerDetected = true;
}
