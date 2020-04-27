// Fill out your copyright notice in the Description page of Project Settings.


#include "UserControlledState.h"
#include "Components/InputComponent.h"
#include "Engine.h"

// Sets default values
AUserControlledState::AUserControlledState()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUserControlledState::PlayerForwardInput(float axis)
{
	inputs[0][0] = axis;
}

void AUserControlledState::PlayerRightInput(float axis)
{
	inputs[0][1] = axis;
}

void AUserControlledState::PlayerAttackPress()
{
	inputs[0][2] = 1.0;
}

void AUserControlledState::PlayerAttackRelease()
{
	inputs[0][2] = 0.0;
}

void AUserControlledState::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUserControlledState::PlayerForwardInput);
	PlayerInputComponent->BindAxis("TurnRight", this, &AUserControlledState::PlayerRightInput);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AUserControlledState::PlayerAttackPress);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AUserControlledState::PlayerAttackRelease);
}

void AUserControlledState::UpdateState()
{
	GEngine->AddOnScreenDebugMessage(4, 0.3f, FColor::Yellow, FString("fitness: ") + FString(std::to_string(fitnesses[0]).c_str()));
}
