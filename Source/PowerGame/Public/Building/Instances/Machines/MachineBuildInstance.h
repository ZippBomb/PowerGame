#pragma once

#include <CoreMinimal.h>
#include "Building/Instances/MeshBuildInstance.h"

#include "Core/Core.h"

#include "MachineBuildInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMachines, Log, All);

class UMachineUI;

UCLASS()
class POWERGAME_API AMachineBuildInstance : public AMeshBuildInstance {

	GENERATED_BODY()
	
public:
	AMachineBuildInstance();

	virtual void Interact(AMainPlayerCharacter* character) override;

	void CloseUI() { uiOpen = false; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	bool uiOpen = false;

	UPROPERTY()
	TObjectPtr<UMachineUI> ui = nullptr;

	template<typename T> T* GetUI() const {

		T* ret = Cast<T>(ui);
		PW_ASSERT(ret != nullptr, LogMachines, TEXT("Could not cast UI."));

		return ret;

	}

};