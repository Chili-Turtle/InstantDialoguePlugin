#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FInstantDialogueEditorStyle
{
public:
	static void Initialize();
	static void Shutdown();

	static const FName& GetStyleSetName();

	static TSharedPtr<ISlateStyle> Get() { return StyleSet; }

	/*static const FSlateBrush* GetBrush( FName PropertyName, const ANSICHAR* Specifier = NULL )
	{
		return Instance->GetBrush( PropertyName, Specifier );
	}*/

private:
	static TSharedPtr<FSlateStyleSet> StyleSet;
	//static TSharedPtr< class ISlateStyle > Instance;
};
