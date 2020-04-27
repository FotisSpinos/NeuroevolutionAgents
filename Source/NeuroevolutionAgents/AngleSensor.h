// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AngleSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEUROEVOLUTIONAGENTS_API UAngleSensor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAngleSensor();

	UAngleSensor(const float sensorRange, const float angleOffset, const float sensorAngle);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	std::vector<float>& GetRecordedDistances();
	std::vector<FString>& GetRecordedTags();

	void SetSensorRange(const float sensorRange);
	void SetOffsetAngle(const float offsetAngle);

	void SetSensorAttributes(float offsetAngle, float visionAngle, float visionRange);
	void EmitSensorRays();

	float sensorDistance;
	float offsetAngle;
	float visionAngle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float offsetAngleStore;

	std::vector<float> recordedDistances;
	std::vector<FString> recordedTags;

	void EmitSingleRay(float angle, int index);

	unsigned int raysNumber;
};
