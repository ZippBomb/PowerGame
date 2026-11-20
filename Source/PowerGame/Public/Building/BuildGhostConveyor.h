#pragma once

#include <CoreMinimal.h>
#include "Building/BuildGhost.h"

#include "BuildGhostConveyor.generated.h"

struct FMachineItemSlot;

UCLASS()
class POWERGAME_API ABuildGhostConveyor : public ABuildGhost {

	GENERATED_BODY()

public:
	virtual void Update(const FVector& hitLocation, ABuildInstance* hitBuildInstance) override;

	virtual ABuildInstance* Confirm(const FVector& location, ABuildInstance* targetBuildInstance) override;

protected:
	UPROPERTY(EditAnywhere)
	float maxSnapDistance = 100.0f;

private:
	FMachineItemSlot* machineSlot = nullptr;

};