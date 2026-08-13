#pragma once

#include <CoreMinimal.h>
#include "UI/Inventory/InventorySlot.h"

#include "MachineSlot.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, UItemData*, uint32);

UCLASS(Abstract)
class POWERGAME_API UMachineSlot : public UInventorySlot {

	GENERATED_BODY()

public:
	FOnItemAdded onItemAdded;

	
protected:
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UItemData>> allowedItems;

	virtual bool CanAcceptDrop(UItemData* droppedItem) const override;
	virtual bool NativeOnDrop(const FGeometry& geometry, const FDragDropEvent& event, UDragDropOperation* operation) override;

};