#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/MeshBuildInstance.h"

#include "Core/Core.h"

#include "Logistics/ItemReceiver.h"

#include <Logging/LogMacros.h>
#include <InstanceDataTypes.h>

#include "Conveyor.generated.h"

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
	float progress = 0.0f;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> ism = nullptr;
	FPrimitiveInstanceId ismID;

};

UCLASS()
class POWERGAME_API AConveyor : public AMeshBuildInstance, public IItemReceiver {

	GENERATED_BODY()
	
public:
	AConveyor();

	virtual void Tick(float deltaTime) override;

	void InsertItem(UItemData* item);

	virtual bool CanReceiveItem(UItemData* item) override { return insertTimer <= 0.0f; }
	virtual void ReceiveItem(UItemData* item) override { InsertItem(item); }

protected:
	UPROPERTY(EditAnywhere, Category = "Conveyor")
	float speed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Conveyor")
	float insertSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Conveyor")
	FVector start;
	UPROPERTY(EditAnywhere, Category = "Conveyor")
	FVector end;
	UPROPERTY(EditAnywhere, Category = "Conveyor")
	float length = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Conveyor")
	TScriptInterface<IItemReceiver> connectedReceiver = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category = "Conveyor | Data")
	TArray<FConveyorItem> items;

	UPROPERTY(VisibleAnywhere)
	TMap<UItemData*, TObjectPtr<UInstancedStaticMeshComponent>> itemISMMap;
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UInstancedStaticMeshComponent>> ismPool;

	UPROPERTY()
	float insertTimer = 0.0f;
	
};