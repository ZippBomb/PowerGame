#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "Inventory/ItemSlot.h"

#include "InventoryComponent.generated.h"

class UItemData;

class UInventoryMenu;

class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POWERGAME_API UInventoryComponent : public UActorComponent {

	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void Open();
	UFUNCTION(BlueprintCallable)
	void Close();

	void AddItem(UItemData* item, uint32 quantity = 1);
	bool RemoveItem(UItemData* item, uint32 quantity = 1);

protected:
	UPROPERTY(VisibleAnywhere)
	TMap<TObjectPtr<UItemData>, uint32> storedItems;

	UPROPERTY(VisibleAnywhere)
	TArray<FItemSlot> slots;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> closeAction = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UInventoryMenu> m_ui = nullptr;

	virtual void BeginPlay() override;
	
};