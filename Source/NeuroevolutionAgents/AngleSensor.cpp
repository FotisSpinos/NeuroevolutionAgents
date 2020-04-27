// Fill out your copyright notice in the Description page of Project Settings.


#include "AngleSensor.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"


// Sets default values for this component's properties
UAngleSensor::UAngleSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAngleSensor::BeginPlay()
{
	Super::BeginPlay();

	// Initialize ray variables
	SetSensorAttributes(0.2f, 0.7f, 1000.0f);
}

void UAngleSensor::EmitSensorRays()
{
	float currentAngleOffset = 0.0f, sensorAngle = 0.0f;

	// Define forward and origin vectors
	FVector actorForwardVector = GetOwner()->GetActorForwardVector();
	FVector origin(1.0f, 0.0f, 0.0f);	// The origin

	// the angle formed between the origin and the forward vector
	float originForwardAngle = atan2(actorForwardVector.Y - origin.Y, actorForwardVector.X - origin.Y);

	// Emit a ray at the center
	EmitSingleRay(originForwardAngle, 0);

	// Emit rays 
	for (unsigned int rayCount = 1; rayCount < raysNumber / 2 + 1; rayCount++)
	{
		// increase the offset for the new rays
		currentAngleOffset = offsetAngle * rayCount;

		// Create a ray for each one of the two sides (left and right)
		for (unsigned int i = 0; i < 2; i++)
		{
			if (i == 0) // positive rays
				sensorAngle = currentAngleOffset + originForwardAngle;
			else		// negative rays
				sensorAngle = -currentAngleOffset + originForwardAngle;

			EmitSingleRay(sensorAngle, rayCount * 2 - 1 + i);	//rayCount
		}
	}
}

void UAngleSensor::EmitSingleRay(float angle, int index)
{
	// Define the ray direction
	FVector rayDirection = FVector(cos(angle), sin(angle), 0);

	// Culculate the start and end of the ray
	FVector start = GetOwner()->GetActorLocation();
	FVector end = start + rayDirection * sensorDistance;

	// Check for collisions
	FHitResult outHit;
	FCollisionQueryParams collisionParams;

	// Raycast 
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, collisionParams))
	{
		//record distances
		float distance = (outHit.ImpactPoint - GetOwner()->GetActorLocation()).Size();
		recordedDistances.at(index) = distance / sensorDistance;	//make sure that the value is between 0 and 1

		// record tags
		if(outHit.GetActor()->Tags.IsValidIndex(0))
			recordedTags.at(index) = outHit.GetActor()->Tags[0].ToString();
	}
	else
	{
		recordedDistances.at(index) = 1.0;
		recordedTags.at(index) = FString("NO_IMPACT");
	}

	//Draw sensor vector
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 1, 0, 1);
}


// Called every frame
void UAngleSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

std::vector<float>& UAngleSensor::GetRecordedDistances()
{
	return recordedDistances;
}

std::vector<FString>& UAngleSensor::GetRecordedTags()
{
	return recordedTags;
}

void UAngleSensor::SetSensorRange(const float sensorRange)
{
	this->sensorDistance = sensorRange;
}

void UAngleSensor::SetOffsetAngle(const float offsetAngle)
{
	this->offsetAngle = offsetAngle;

	//Edit later
	offsetAngleStore = offsetAngle;
}

void UAngleSensor::SetSensorAttributes(float offsetAngle, float visionAngle, float sensorDistance)
{
	// Set offset angle
	this->offsetAngle = offsetAngle;

	// set vision angle
	this->visionAngle = visionAngle;

	// The amount of offsets fitting into the set vision multiplied by two for the two sides, plus 1 for the center ray
	raysNumber = (visionAngle / offsetAngle) * 2 + 1;

	// set the sensorDistance
	this->sensorDistance = sensorDistance;

	// initialize distances vector
	recordedDistances = std::vector<float>(raysNumber);
	
	// initialize tags vector
	recordedTags = std::vector<FString>(raysNumber);
}
