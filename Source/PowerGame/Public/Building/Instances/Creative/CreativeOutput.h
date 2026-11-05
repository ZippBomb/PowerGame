#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/MeshBuildInstance.h"

#include "CreativeOutput.generated.h"

class UItemData;

class AConveyor;

UCLASS()
class POWERGAME_API ACreativeOutput : public AMeshBuildInstance {

	GENERATED_BODY()

public:
	ACreativeOutput();
	
	virtual void Tick(float deltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemData> item = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<AConveyor> conveyor = nullptr;
	
};