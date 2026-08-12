#pragma once

#include <CoreMinimal.h>

#include "Core/Core.h"

#include "ItemSlot.generated.h"

class UItemData;

class UInventoryPanel;
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
	uint32 slotIndex = 0;

	UInventorySlot* GetSlot(UInventoryPanel* invPanel) const;

};