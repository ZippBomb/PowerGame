#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/Machines/MachineBuildInstance.h"

#include "Core/Core.h"

#include "Saving/BuildingSaveData.h"

#include "Generator.generated.h"

USTRUCT()
struct FGeneratorSaveData : public FBuildingSaveData {

	GENERATED_BODY()

	UPROPERTY()
	float maxOutput = 0.0f;
	UPROPERTY()
	float responseStrength = 1.0f;

};

UCLASS()
class POWERGAME_API AGenerator : public AMachineBuildInstance {

	GENERATED_BODY()

public:
	virtual void SerializeSaveData(FInstancedStruct* out) override;
	virtual void DeserializeSaveData(const FInstancedStruct& data) override;

	UFUNCTION(BlueprintCallable)
	inline float GetMaxOutput() const { return maxOutput; }

	UFUNCTION(BlueprintCallable)
	inline float GetOutput() const { return maxOutput * outputLevel; }
	UFUNCTION(BlueprintCallable)
	inline float GetOutputLevel() const { return outputLevel; }

	UFUNCTION(BlueprintCallable)
	inline float GetInertia() const { return inertia; }

	UFUNCTION(BlueprintCallable)
	inline void SetOutputLevel(float value) { outputLevel = value; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	float maxOutput = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Production")
	float outputLevel = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Production")
	float inertia = 1.0f;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type reason) override;

};