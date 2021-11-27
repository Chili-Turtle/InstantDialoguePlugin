#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "InstantDialogueFactory.generated.h"

UCLASS()
class INSTANTDIALOGUEEDITOR_API UInstantDialogueFactory : public UFactory
{
	GENERATED_BODY()

public:
	UInstantDialogueFactory();
	virtual ~UInstantDialogueFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};


UCLASS()
class INSTANTDIALOGUEEDITOR_API UConditionFactory : public UFactory
{
	GENERATED_BODY()

public:
	UConditionFactory();
	virtual ~UConditionFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};

