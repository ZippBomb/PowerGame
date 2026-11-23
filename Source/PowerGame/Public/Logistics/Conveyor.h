#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/SplineBuildInstance.h"

#include "Core/Core.h"

#include "Logistics/ItemReceiver.h"

#include <Logging/LogMacros.h>
#include <InstanceDataTypes.h>

#include "Conveyor.generated.h"

class ABuildGhostConveyor;

class UItemData;
class IItemReceiver;

class UInstancedStaticMeshComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogLogistics, Log, All);

USTRUCT()
struct FConveyorItem {

	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UItemData> item = nullptr;

	UPROPERTY()
	float distance = 0.0f;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> ism = nullptr;
	FPrimitiveInstanceId ismID;

};

UCLASS()
class POWERGAME_API AConveyor : public ASplineBuildInstance, public IItemReceiver {

	GENERATED_BODY()

	friend ABuildGhostConveyor;
	
public:
	AConveyor();

	virtual void Tick(float deltaTime) override;

	void InsertItem(UItemData* item);

	void SetSpline(USplineComponent* spline);

	virtual bool CanReceiveItem(UItemData* item) override { return insertTimer <= 0.0f; }
	virtual void ReceiveItem(UItemData* item) override { InsertItem(item); }

	virtual TArray<FMachineItemSlot>& GetMachineSlots() override { return slots; }

protected:
	UPROPERTY(EditAnywhere, Category = "Conveyor")
	float speed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Conveyor")
	float insertSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Conveyor")
	float conveyorHeight = 65.0f;

	UPROPERTY(EditAnywhere, Category = "Conveyor")
	TScriptInterface<IItemReceiver> connectedReceiver = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category = "Conveyor | Data")
	TArray<FConveyorItem> items;

	UPROPERTY(EditAnywhere, Category = "Conveyor")
	TArray<FMachineItemSlot> slots;

	UPROPERTY(VisibleAnywhere)
	TMap<UItemData*, TObjectPtr<UInstancedStaticMeshComponent>> itemISMMap;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UInstancedStaticMeshComponent>> ismPool;

	UPROPERTY(VisibleAnywhere)
	float m_length = 100.0f;

	UPROPERTY()
	float insertTimer = 0.0f;
	
};