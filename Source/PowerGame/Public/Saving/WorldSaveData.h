#pragma once

#include <CoreMinimal.h>
#include <GameFramework/SaveGame.h>

#include "Core/Core.h"

#include "Saving/NetworkSaveData.h"

#include <InstancedStruct.h>

#include "WorldSaveData.generated.h"

UCLASS()
class POWERGAME_API UWorldSaveData : public USaveGame {

	GENERATED_BODY()
	
public:
	UWorldSaveData();
	
	// Slot data

	UPROPERTY(VisibleAnywhere, Category = "Slot data")
	FString slotName;
	UPROPERTY(VisibleAnywhere, Category = "Slot data")
	uint32 userIndex = 0;

	// Player data

	UPROPERTY(VisibleAnywhere, Category = "Player")
	FVector playerPosition;
	UPROPERTY(VisibleAnywhere, Category = "Player")
	FRotator playerRotation;

	// Building data

	UPROPERTY(VisibleAnywhere, Category = "Buildings")
	TArray<FInstancedStruct> buildings;

	// Power network data

	UPROPERTY(VisibleAnywhere, Category = "Network data")
	TArray<FNetworkSaveData> networks;

};