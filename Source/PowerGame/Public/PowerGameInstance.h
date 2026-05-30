#pragma once

#include <CoreMinimal.h>
#include <Engine/GameInstance.h>

#include "PowerGameInstance.generated.h"

UCLASS()
class POWERGAME_API UPowerGameInstance : public UGameInstance {

	GENERATED_BODY()
	
public:
	FString pendingSaveSlot;
	bool pendingSaveLoad = false;

};