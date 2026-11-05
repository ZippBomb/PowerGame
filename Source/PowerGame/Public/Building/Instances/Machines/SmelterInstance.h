#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/Machines/Load.h"

#include "Logistics/ItemReceiver.h"

#include "Inventory/ItemSlot.h"

#include "SmelterInstance.generated.h"

class UItemData;
class URecipeData;

class AConveyor;

class USmelterUI;

UCLASS()
class POWERGAME_API ASmelterInstance : public ALoad, public IItemReceiver {

	GENERATED_BODY()
	
	friend USmelterUI;

public:
	ASmelterInstance();

	virtual void Tick(float deltaTime) override;

	virtual void Interact(AMainPlayerCharacter* character) override;

	virtual void ReceiveItem(UItemData* item) override { OnInputAdded(item); }
	virtual bool CanReceiveItem(UItemData* item) override { return inputSlot.item == nullptr || inputSlot.item == item; }

	UFUNCTION(BlueprintCallable)
	inline void SetRecipe(URecipeData* value) { recipe = value; }

protected:
	UPROPERTY(VisibleAnywhere)
	bool smelting = false;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URecipeData> recipe = nullptr;

	UPROPERTY(VisibleAnywhere)
	FItemSlot inputSlot;
	UPROPERTY(VisibleAnywhere)
	FItemSlot outputSlot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AConveyor> outputConveyor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float smeltingSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Properties")
	float powerDemand = 50.0f;

	void OnInputAdded(UItemData* item, uint32 quantity = 1);
	void OnInputRemoved(UItemData* item, uint32 quantity = 1);

	void OnOutputRemoved(UItemData* item, uint32 quantity = 1);

private:
	UPROPERTY()
	float smeltTimer = 0.0f;

	void ConsumeInput();

	void AttemptOutput();

	void StartSmelting();
	void StopSmelting();
	bool CanStartSmelting() const;

	float CalculateProgress();

};