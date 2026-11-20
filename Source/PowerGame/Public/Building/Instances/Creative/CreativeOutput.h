#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/MeshBuildInstance.h"

#include "Logistics/ItemReceiver.h"

#include "CreativeOutput.generated.h"

class UItemData;

class AConveyor;

UCLASS()
class POWERGAME_API ACreativeOutput : public AMeshBuildInstance, public IItemReceiver {

	GENERATED_BODY()

public:
	ACreativeOutput();
	
	virtual void Tick(float deltaTime) override;

	virtual bool CanReceiveItem(UItemData*) override { return false; }
	virtual void ReceiveItem(UItemData*) override {}

	virtual TArray<FMachineItemSlot>& GetMachineSlots() override { return slots; }

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemData> item = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<FMachineItemSlot> slots;
	
};