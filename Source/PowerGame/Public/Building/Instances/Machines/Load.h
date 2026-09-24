#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/Machines/MachineBuildInstance.h"

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
class POWERGAME_API ALoad : public AMachineBuildInstance {

	GENERATED_BODY()

public:
	virtual void SerializeSaveData(FInstancedStruct* out) override;
	virtual void DeserializeSaveData(const FInstancedStruct& data) override;

	UFUNCTION(BlueprintCallable)
	inline float GetDemand() const { return demand; }

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	float demand = 0.0f;
	
	virtual void EndPlay(const EEndPlayReason::Type reason) override;

};