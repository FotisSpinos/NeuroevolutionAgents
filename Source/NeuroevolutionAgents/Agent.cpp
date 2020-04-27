// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"

// Add default functionality here for any IAgent functions that are not pure virtual.

double IAgent::GetInput(const int index)
{
	return 0.0f;
}

void IAgent::AddObservation(std::vector<float>& observations)
{
}

void IAgent::AddObservation(float observation)
{
}
