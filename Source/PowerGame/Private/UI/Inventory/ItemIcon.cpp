#include "UI/Inventory/ItemIcon.h"
#include "UI/Inventory/InventorySlot.h"
#include "UI/Inventory/InventoryPanel.h"

#include "UI/MainHUD.h"

#include "Inventory/InventoryComponent.h"
#include "Inventory/ItemData.h"

#include "Player/MainPlayerCharacter.h"

#include <Components/Image.h>

#include <Blueprint/WidgetBlueprintLibrary.h>

void UItemIcon::SetItem(UItemData* item) {

	if (item == nullptr) {

		icon->SetBrushFromTexture(nullptr);
		icon->SetOpacity(0.0f);

	} else {

		icon->SetBrushFromTexture(item->icon);
		icon->SetOpacity(1.0f);

	}

}

FReply UItemIcon::NativeOnMouseButtonDown(const FGeometry& geometry, const FPointerEvent& mouseEvent) {

	if (!mouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) return Super::NativeOnMouseButtonDown(geometry, mouseEvent);
	return UWidgetBlueprintLibrary::DetectDragIfPressed(mouseEvent, this, EKeys::LeftMouseButton).NativeReply;

}

void UItemIcon::NativeOnDragDetected(const FGeometry& geometry, const FPointerEvent& mouseEvent, UDragDropOperation*& outOperation) {

	if (m_slot->GetItem() == nullptr) return;

	TObjectPtr<UDragDropOperation> operation = NewObject<UDragDropOperation>();
	TObjectPtr<UItemIcon> visual = CreateWidget<UItemIcon>(GetOwningPlayer(), GetClass());

	if (visual == nullptr) return;
	visual->SetItem(m_slot->GetItem());

	TObjectPtr<UItemDragPayload> payload = NewObject<UItemDragPayload>();
	payload->item = m_slot->GetItem();
	payload->quantity = m_slot->GetQuantity();
	payload->slot = m_slot;

	operation->Payload = Cast<UObject>(payload);
	operation->DefaultDragVisual = visual;
	operation->OnDragCancelled.AddDynamic(this, &UItemIcon::HandleDragCancelled);
	outOperation = operation;

	m_slot->Clear();
	m_slot->OnDragStarted(payload);

	UInventoryPanel* invPanel = m_slot->GetInventoryPanel();
	if (invPanel == nullptr || invPanel->inventory == nullptr) return;

	invPanel->inventory->RemoveFromSlot(m_slot->GetSlotIndex());

}

void UItemIcon::HandleDragCancelled(UDragDropOperation* operation) {

	TObjectPtr<UItemDragPayload> payload = Cast<UItemDragPayload>(operation->Payload);
	if (payload == nullptr) return;

	UInventorySlot* slot = payload->slot;
	PW_ASSERT(slot != nullptr, LogUI, TEXT("UItemDragPayload::slot is invalid."));

	slot->OnDragCancelled(payload);

	UInventoryPanel* invPanel = slot->GetInventoryPanel();
	if (invPanel == nullptr || invPanel->inventory == nullptr) return;

	invPanel->inventory->SetAtSlot(slot->GetSlotIndex(), payload->item, payload->quantity);

}