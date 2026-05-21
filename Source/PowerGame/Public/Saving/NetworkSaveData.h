#pragma once

#include <CoreMinimal.h>
#include "Core/Core.h"

#include "NetworkSaveData.generated.h"

USTRUCT()
struct FNetworkSaveData {

	GENERATED_BODY()

	UPROPERTY()
	float baseFrequency = 0.0f;
	UPROPERTY()
	float baseVoltage = 0.0f;
	UPROPERTY()
	float responseStrength = 1.0f;

	UPROPERTY()
	bool dead = true;
	UPROPERTY()
	float frequency = 0.0f;
	UPROPERTY()
	float voltage = 0.0f;

	UPROPERTY()
	TArray<FGuid> connections;
	UPROPERTY()
	TArray<FGuid> generators;
	UPROPERTY()
	TArray<FGuid> misc;

	UPROPERTY()
	TArray<FGuid> loads;

};
