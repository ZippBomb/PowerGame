#include "UI/Inventory/InventorySlot.h"
#include "UI/Inventory/InventoryPanel.h"
#include "UI/Inventory/ItemIcon.h"

#include "UI/MainHUD.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemSlot.h"
#include "Inventory/ItemData.h"

#include <Components/TextBlock.h>

#include <Blueprint/DragDropOperation.h>

#include <Engine/AssetManager.h>

void UInventorySlot::NativeConstruct() {

	Super::NativeConstruct();

	icon->SetSlot(this);

	icon->SetItem(nullptr);
	quantityText->SetVisibility(ESlateVisibility::Hidden);

}

void UInventorySlot::SetData(const FItemSlot& data) {

	SetItem(data.item);
	SetQuantity(data.quantity);

}

void UInventorySlot::SetItem(UItemData* value) {

	item = value;
	icon->SetItem(value);

}
void UInventorySlot::SetQuantity(uint32 value) {

	quantity = value;

	if (quantity != 0) {

		if (quantityText->GetVisibility() == ESlateVisibility::Hidden)
			quantityText->SetVisibility(ESlateVisibility::Visible);

		quantityText->SetText(FText::FromString(FString::FromInt(value)));

	}
	else 
		quantityText->SetVisibility(ESlateVisibility::Hidden);

}

void UInventorySlot::Clear() {

	SetItem(nullptr);
	SetQuantity(0);

}

bool UInventorySlot::NativeOnDrop(const FGeometry& geometry, const FDragDropEvent& event, UDragDropOperation* operation) {

	TObjectPtr<UItemDragPayload> payload = Cast<UItemDragPayload>(operation->Payload);
	if (payload == nullptr) return false;
	if (!CanAcceptDrop(payload->item)) return false;

	if (item == payload->item) {

		if (quantity + payload->quantity > item->stackSize) {

			uint32 left = payload->quantity - item->stackSize + quantity;

			SetQuantity(item->stackSize);

			if (inventoryPanel != nullptr && inventoryPanel->inventory != nullptr)
				inventoryPanel->inventory->SetAtSlot(slotIndex, item, quantity);

			UInventorySlot* other = payload->slot;
			PW_ASSERT(other != nullptr, LogUI, TEXT("UItemDragPayload::slot is invalid."));

			other->SetItem(payload->item);
			other->SetQuantity(left);

			if (other->inventoryPanel == nullptr) return true;
			if (other->inventoryPanel->inventory == nullptr) return true;

			other->inventoryPanel->inventory->SetAtSlot(other->slotIndex, item, left);

			return true;

		} else {

			SetQuantity(quantity + payload->quantity);

			if (inventoryPanel == nullptr) return true;
			if (inventoryPanel->inventory == nullptr) return true;

			inventoryPanel->inventory->SetAtSlot(slotIndex, item, quantity);

			return true;

		}

	} else if (item != nullptr) return false;

	SetItem(payload->item);
	SetQuantity(payload->quantity);

	if (inventoryPanel == nullptr) return true;
	if (inventoryPanel->inventory == nullptr) return true;

	inventoryPanel->inventory->SetAtSlot(slotIndex, item, quantity);

	return true;

}