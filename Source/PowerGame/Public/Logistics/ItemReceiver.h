#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>

#include "ItemReceiver.generated.h"

class UItemData;

UINTERFACE(MinimalAPI)
class UItemReceiver : public UInterface {

	GENERATED_BODY()

};

class POWERGAME_API IItemReceiver {

	GENERATED_BODY()

public:
	virtual bool CanReceiveItem(UItemData* item) = 0;
	virtual void ReceiveItem(UItemData* item) = 0;

};