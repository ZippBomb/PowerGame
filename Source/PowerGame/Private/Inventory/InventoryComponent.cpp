#include "Inventory/InventoryComponent.h"

UInventoryComponent::UInventoryComponent() {
	
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay() {

	Super::BeginPlay();
}

void UInventoryComponent::AddItem(const FPrimaryAssetId& itemID, uint32 quantity) {

	if (storedItems.Contains(itemID))
		storedItems[itemID] += quantity;
	else
		storedItems.Add(itemID, quantity);

}
bool UInventoryComponent::RemoveItem(const FPrimaryAssetId& itemID, uint32 quantity) {

	if (!storedItems.Contains(itemID)) return false;

	if (storedItems[itemID] > quantity)
		storedItems[itemID] -= quantity;
	else
		storedItems.Remove(itemID);

	return true;

}