#include "InstantDialogueFactory.h"

#include "ConditionObject.h"
#include "InstantDialogueGraph.h"
#include "ConditionObject.h"

#define LOCTEXT_NAMESPACE "InstantDialogueFactory"

UInstantDialogueFactory::UInstantDialogueFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UInstantDialogue::StaticClass();
}

UInstantDialogueFactory::~UInstantDialogueFactory()
{

}

UObject* UInstantDialogueFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UObject>(InParent, Class, Name, Flags | RF_Transactional);
}




/////////// UCondtionFactory
UConditionFactory::UConditionFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UInstantDialogueConditionObject::StaticClass();
}

UConditionFactory::~UConditionFactory()
{

}

UObject* UConditionFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UInstantDialogueConditionObject>(InParent, Class, Name, Flags | RF_Transactional);
}

#undef LOCTEXT_NAMESPACE