#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(InstantDialogueRuntime, Log, All);

/**
 * The public interface to this module
 */
class IInstantDialogueRuntime : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static IInstantDialogueRuntime& Get()
	{
		UE_LOG(LogTemp, Warning, TEXT("IInstance Runtime"));
		return FModuleManager::LoadModuleChecked< IInstantDialogueRuntime >("InstantDialogueRuntime");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static bool IsAvailable()
	{
		UE_LOG(LogTemp, Warning, TEXT("IInstance Runtime avaialable"));
		return FModuleManager::Get().IsModuleLoaded( "InstantDialogueRuntime" );
	}
};

