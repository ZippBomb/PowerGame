#include "UI/Inventory/InventoryPanel.h"
#include "UI/Inventory/InventorySlot.h"

#include "UI/MainHUD.h"

#include "Inventory/ItemSlot.h"

#include "Player/MainPlayerController.h"

#include <Components/WrapBox.h>

inline UInventorySlot* UInventoryPanel::AddSlot() {

	PW_ASSERT(inventorySlotClass != nullptr, LogUI, TEXT("Inventory slot class must be assigned."));

	UInventorySlot* slot = CreateWidget<UInventorySlot>(GetOwningPlayer(), inventorySlotClass);
	slot->slotIndex = slots.Num();
	slot->inventoryPanel = this;

	slotContainer->AddChild(slot);
	slots.Add(slot);

	return slot;

}
void UInventoryPanel::RemoveSlot(int32 index) {

	PW_ASSERT(index < slots.Num(), LogUI, TEXT("Can't remove an invalid slot index."));

	UInventorySlot* slot = slots[index];

	slot->RemoveFromViewport();
	slotContainer->RemoveChild(slot);

	slots.RemoveAt(index);

}

void UInventoryPanel::LoadSlots(const TArray<FItemSlot>& itemSlots) {

	// Creathe slots if necessary.

	if (slots.Num() < itemSlots.Num()) {

		int32 diff = itemSlots.Num() - slots.Num();
		for (int32 i = 0; i < diff; i++) {

			UInventorySlot* slot = AddSlot();
			PW_ASSERT(slot->slotIndex == itemSlots[slot->slotIndex].slotIndex, LogUI, TEXT("Non matching slot indexes."));

		}

	}

	// Match the slots

	for (int32 i = 0; i < itemSlots.Num(); i++) {

		PW_ASSERT(i < slots.Num(), LogUI, TEXT("Invalid inventory slot index."));

		UInventorySlot* invSlot = slots[i];
		const FItemSlot& itemSlot = itemSlots[i];
		
		PW_ASSERT(invSlot != nullptr, LogUI, TEXT("Invalid inventory slot."));

		invSlot->SetData(itemSlot);

	}

}