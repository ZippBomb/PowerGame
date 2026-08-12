#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemData.h"

#include "UI/Inventory/InventoryMenu.h"
#include "UI/Inventory/InventoryPanel.h"
#include "UI/Inventory/InventorySlot.h"

DEFINE_LOG_CATEGORY(LogInventory);

UInventoryComponent::UInventoryComponent() {

	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay() {

	Super::BeginPlay();

	if (inventoryPanel != nullptr)
		inventoryPanel->inventory = this;

}

void UInventoryComponent::AddItem(UItemData* item, uint32 quantity) {

	uint32 remaining = quantity;
	if (storedItems.Contains(item)) {

		storedItems[item] += quantity;

		for (FItemSlot& slot : slots) {

			if (remaining == 0) return;
			if (slot.item != item || slot.quantity == item->stackSize) continue;

			if (slot.quantity + remaining > item->stackSize) {

				// If we can't fit all the items into this slot, fill it up to the max and
				// subtract the number of inserted items.

				remaining -= item->stackSize - slot.quantity;
				slot.quantity = item->stackSize;

			} else {

				// Otherwise just add the amount of items and set remaining to 0

				slot.quantity += remaining;
				remaining = 0;

			}

			if (inventoryPanel != nullptr)
				slot.GetSlot(inventoryPanel)->SetQuantity(slot.quantity);

		}

	} else
		storedItems.Add(item, quantity);

	// Any remaining items that could not fit into already present slots will be added into
	// empty slots, until we either run out of slots or inserted the entire amount of items

	for (int32 i = 0; i < slots.Num() && remaining > 0; i++) {

		FItemSlot& slot = slots[i];
		if (slot.item != nullptr) continue;

		PW_ASSERT(slot.quantity == 0, LogInventory, TEXT("Empty slot has a non zero quantity."));

		slot.item = item;

		if (remaining > item->stackSize) {

			// If we are going to insert more than the stack size, we can subtract exactly
			// the stack size since the slot is guaranteed to be empty.

			remaining -= item->stackSize;
			slot.quantity = item->stackSize;

		} else {

			// Otherwise we just set it to the remaining amount and finsih up.

			slot.quantity = remaining;
			remaining = 0;

		}

		if (inventoryPanel != nullptr)
			slot.GetSlot(inventoryPanel)->SetData(slot);

	}

}
bool UInventoryComponent::RemoveItem(UItemData* item, uint32 quantity) {

	/*if (!storedItems.Contains(item)) return false;

	if (storedItems[item] > quantity)
		storedItems[item] -= quantity;
	else
		storedItems.Remove(item);

	uint32 remaining = quantity;
	for (int32 i = slots.Num() - 1; i > -1 && quantity > 0; i--) {

		FItemSlot& slot = slots[i];
		if (slot.item == nullptr) continue;

		if (slot.quantity <= remaining) {

			remaining -= slot.quantity;

			m_ui->GetInventoryPanel()->RemoveSlot(i);
			slots.RemoveAt(i);

		} else {

			slot.quantity -= remaining;
			slot.uiSlot->UpdateQuantity(slot.quantity);

		}

	}*/

	return true;

}

void UInventoryComponent::SetAtSlot(uint32 index, UItemData* item, uint32 quantity) {

	PW_ASSERT(index < (uint32)slots.Num(), LogInventory, TEXT("Invalid slot index."));
	FItemSlot& slot = slots[index];

	uint32 amount = FMath::Min(quantity, item->stackSize);
	if (slot.item == nullptr)
		slot.item = item;
	else if (slot.item != item) return;
	
	uint32 added = amount - slot.quantity;
	slot.quantity = amount;

	if (storedItems.Contains(item))
		storedItems[item] += added;
	else
		storedItems.Add(item, added);

	if (inventoryPanel == nullptr) return;
	slot.GetSlot(inventoryPanel)->SetData(slot);

}
bool UInventoryComponent::RemoveFromSlot(uint32 index, uint32 quantity) {

	PW_ASSERT(index < (uint32) slots.Num(), LogInventory, TEXT("Invalid slot index."));
	FItemSlot& slot = slots[index];

	if (slot.item == nullptr) return false;

	if (quantity == 0 || slot.quantity < quantity) {

		storedItems[slot.item] -= slot.quantity;

		slot.item = nullptr;
		slot.quantity = 0;

		if (inventoryPanel != nullptr)
			slot.GetSlot(inventoryPanel)->SetData(slot);

		return true;

	}

	storedItems[slot.item] -= quantity;
	slot.quantity -= quantity;

	if (inventoryPanel != nullptr)
		slot.GetSlot(inventoryPanel)->SetData(slot);

	return true;

}

void UInventoryComponent::DisplayInventory(UInventoryPanel* invPanel) {

	invPanel->LoadSlots(slots);
	invPanel->inventory = this;

}