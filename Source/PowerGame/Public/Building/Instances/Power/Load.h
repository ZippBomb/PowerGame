#pragma once

#include <CoreMinimal.h>
#include <Building/Instances/MeshBuildInstance.h>

#include "Core/Core.h"

#include "Saving/BuildingSaveData.h"

#include "Load.generated.h"

USTRUCT()
struct FLoadSaveData : public FBuildingSaveData {

	GENERATED_BODY()

	UPROPERTY()
	float demand = 0.0f;

};

UCLASS()
class POWERGAME_API ALoad : public AMeshBuildInstance {

	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Update(float supplyRatio);

	virtual void SerializeSaveData(FInstancedStruct* out) override;
	virtual void DeserializeSaveData(const FInstancedStruct& data) override;

	UFUNCTION(BlueprintCallable)
	inline float GetDemand() const { return demand; }

protected:
	virtual void EndPlay(const EEndPlayReason::Type reason) override;

private:
	UPROPERTY(EditAnywhere, Category = "Properties")
	float demand = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float m_suppliedPower = 0.0f;

};