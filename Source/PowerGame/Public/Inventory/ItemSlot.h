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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UItemData> item;
	UPROPERTY(VisibleAnywhere)
	uint32 quantity = 0;

	UPROPERTY(VisibleAnywhere)
	uint32 slotIndex = 0;

	UInventorySlot* GetSlot(UInventoryPanel* invPanel) const;

};