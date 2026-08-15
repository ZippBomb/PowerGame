#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/Machines/MachineBuildInstance.h"

#include "Inventory/ItemSlot.h"

#include "SmelterInstance.generated.h"

class UItemData;
class URecipeData;

class USmelterUI;

UCLASS()
class POWERGAME_API ASmelterInstance : public AMachineBuildInstance {

	GENERATED_BODY()
	
	friend USmelterUI;

public:
	ASmelterInstance();

	virtual void Tick(float deltaTime) override;

	virtual void Interact(AMainPlayerCharacter* character) override;

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
	float smeltingSpeed = 1.0f;

	void OnInputAdded(UItemData* item, uint32 quantity);
	void OnInputRemoved(UItemData* item, uint32 quantity);

	void OnOutputRemoved(UItemData* item, uint32 quantity);

private:
	UPROPERTY()
	float smeltTimer = 0.0f;

	void ConsumeInput();

	void StartSmelting();
	void StopSmelting();

	float CalculateProgress();

};