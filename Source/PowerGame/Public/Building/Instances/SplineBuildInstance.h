#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/BuildInstance.h"

#include "Core/Core.h"

#include "Saving/BuildingSaveData.h"

#include "SplineBuildInstance.generated.h"

class USplineComponent;
class USplineMeshComponent;

USTRUCT()
struct FSplineSaveData : public FBuildingSaveData {

	GENERATED_BODY()

	UPROPERTY()
	FVector start;
	UPROPERTY()
	FVector end;

	UPROPERTY()
	float sagAmount;

};

UCLASS()
class POWERGAME_API ASplineBuildInstance : public ABuildInstance {

	GENERATED_BODY()

public:
	ASplineBuildInstance();

	UFUNCTION(BlueprintCallable)
	virtual void SetStartAndEnd(const FVector& startLocation, const FVector& endLocation);

	virtual void SerializeSaveData(FInstancedStruct* out) override;
	virtual void DeserializeSaveData(const FInstancedStruct& data) override;

protected:
	UPROPERTY(EditAnywhere)
	float m_sagAmount = 0.17f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> m_spline = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineMeshComponent> m_splineMesh = nullptr;

};