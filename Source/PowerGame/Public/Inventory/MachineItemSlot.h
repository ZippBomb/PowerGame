#pragma once

#include <CoreMinimal.h>

#include "Core/Core.h"

#include "Inventory/ItemSlot.h"

#include "MachineItemSlot.generated.h"

class AConveyor;

USTRUCT(BlueprintType)
struct POWERGAME_API FMachineItemSlot : public FItemSlot {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool input = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AConveyor> attachedConveyor = nullptr;

};