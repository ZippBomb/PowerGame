#pragma once

#include <CoreMinimal.h>
#include "Building/BuildGhost.h"

#include "BuildGhostConveyor.generated.h"

struct FMachineItemSlot;

class USplineComponent;
class USplineMeshComponent;

UCLASS()
class POWERGAME_API ABuildGhostConveyor : public ABuildGhost {

	GENERATED_BODY()

public:
	ABuildGhostConveyor();

	virtual void Update(const FVector& hitLocation, ABuildInstance* hitBuildInstance) override;

	virtual ABuildInstance* Confirm(const FVector& location, ABuildInstance* targetBuildInstance) override;

	virtual void Reset() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Conveyor Build Ghost|Spline")
	TObjectPtr<USplineComponent> m_spline = nullptr;
	UPROPERTY(EditAnywhere, Category = "Conveyor Build Ghost|Spline")
	TObjectPtr<USplineMeshComponent> m_splineMesh = nullptr;

	UPROPERTY(EditAnywhere)
	float maxSnapDistance = 100.0f;
	UPROPERTY(EditAnywhere)
	float tangentLength = 100.0f;

private:
	FMachineItemSlot* machineSlot = nullptr;

	UPROPERTY()
	ABuildInstance* m_startBuildInstance = nullptr;
	FMachineItemSlot* m_startSlot = nullptr;

	FMachineItemSlot* GetMachineSlot(const FVector& location, ABuildInstance* buildInstance) const;

	void ResetSpline();

};