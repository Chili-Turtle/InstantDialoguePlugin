#include "InstantDialogueRuntimePCH.h"

DEFINE_LOG_CATEGORY(InstantDialogueRuntime)

class FInstantDialogueRuntime : public IInstantDialogueRuntime
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FInstantDialogueRuntime, InstantDialogueRuntime )



void FInstantDialogueRuntime::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FInstantDialogueRuntime::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



