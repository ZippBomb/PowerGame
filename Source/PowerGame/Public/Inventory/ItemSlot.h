#pragma once

#include <CoreMinimal.h>

#include "ItemSlot.generated.h"

class UItemData;

class UInventorySlot;

USTRUCT()
struct POWERGAME_API FItemSlot {

	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UItemData> item;
	UPROPERTY()
	uint32 quantity = 0;

	UPROPERTY()
	TObjectPtr<UInventorySlot> uiSlot;

};