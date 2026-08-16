#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/Machines/Generator.h"

#include "Inventory/ItemSlot.h"

#include "CoalBoilerInstance.generated.h"

class UItemData;

class UCoalBoilerUI;

UCLASS()
class POWERGAME_API ACoalBoilerInstance : public AGenerator {

	GENERATED_BODY()

	friend UCoalBoilerUI;
	
public:
	ACoalBoilerInstance();

	virtual void Tick(float deltaTime) override;

	virtual void Interact(AMainPlayerCharacter* character) override;

protected:
	UPROPERTY(VisibleAnywhere)
	bool burning = false;

	UPROPERTY(VisibleAnywhere)
	FItemSlot inputSlot;
	UPROPERTY(VisibleAnywhere)
	float steam = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Production")
	float fuelConsumption = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Production")
	float steamProduction = 2.0f;

	void OnInputAdded(UItemData* item, uint32 quantity);
	void OnInputRemoved(UItemData* item, uint32 quantity);

private:
	UPROPERTY()
	float consumeTimer = 0.0f;

	void StartBurning();
	void StopBurning();

};