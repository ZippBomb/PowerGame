#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/MeshBuildInstance.h"

#include "Inventory/ItemSlot.h"

#include "Core/Core.h"

#include "CoalBoilerInstance.generated.h"

class UItemData;

class UCoalBoilerUI;

DECLARE_LOG_CATEGORY_EXTERN(LogMachines, Log, All);

UCLASS()
class POWERGAME_API ACoalBoilerInstance : public AMeshBuildInstance {

	GENERATED_BODY()

	friend UCoalBoilerUI;
	
public:
	ACoalBoilerInstance();

	virtual void Tick(float deltaTime) override;

	virtual void Interact(AMainPlayerCharacter* character) override;

	void CloseUI() { uiOpen = false; }

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

	UPROPERTY(VisibleAnywhere, Category = "UI")
	bool uiOpen = false;

	void OnInputAdded(UItemData* item, uint32 quantity);
	void OnInputRemoved(UItemData* item, uint32 quantity);

private:
	UPROPERTY()
	TObjectPtr<UCoalBoilerUI> m_ui = nullptr;

	UPROPERTY()
	float consumeTimer = 0.0f;

	void StartBurning();
	void StopBurning();

};