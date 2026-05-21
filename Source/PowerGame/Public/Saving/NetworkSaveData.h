#pragma once

#include <CoreMinimal.h>
#include "Core/Core.h"

#include "NetworkSaveData.generated.h"

USTRUCT()
struct FNetworkSaveData {

	GENERATED_BODY()

	UPROPERTY()
	TArray<FGuid> connections;
	UPROPERTY()
	TArray<FGuid> generators;
	UPROPERTY()
	TArray<FGuid> misc;

	UPROPERTY()
	TArray<FGuid> loads;

};
