// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseEnvironmentState.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UserControlledState.generated.h"

UCLASS()
class NEUROEVOLUTIONAGENTS_API AUserControlledState : public ABaseEnvironmentState
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUserControlledState();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void UpdateState() override;

protected:
	void PlayerForwardInput(float axis);

	void PlayerRightInput(float axis);

	void PlayerAttackPress();

	void PlayerAttackRelease();
};
