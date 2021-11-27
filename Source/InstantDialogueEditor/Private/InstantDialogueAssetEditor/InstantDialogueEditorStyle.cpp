#include "InstantDialogueAssetEditor/InstantDialogueEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Misc/Paths.h"

TSharedPtr<FSlateStyleSet> FInstantDialogueEditorStyle::StyleSet = nullptr;
//TSharedPtr< ISlateStyle > FInstantDialogueEditorStyle::Instance = NULL;

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( StyleSet->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( StyleSet->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

void FInstantDialogueEditorStyle::Initialize()
{
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet("InstantDialogueEditorStyle"));

	StyleSet->SetContentRoot(FPaths::ProjectPluginsDir() / TEXT("InstantDialogue/Resources"));

	StyleSet->Set("InstantDialogueEditor.AutoArrange", new IMAGE_BRUSH("AutoArrangeIcon", Icon40x40));
	StyleSet->Set("InstantDialogueEditor.AutoArrange.Small", new IMAGE_BRUSH( "AutoArrangeIcon", Icon20x20 ) );
	
	StyleSet->Set("InstantDialogueEditor.TestPic", new IMAGE_BRUSH("TestPic", Icon40x40));

	StyleSet->Set(
		"BigPic",
		new FSlateImageBrush("TestPic.png", Icon40x40)
	);

	//StyleSet->Set("InstantDialogueEditor.TestPic", new FSlateImageBrush(GetPluginContentPath("Icons/DlgDialogue_16x.png"), Icon40x40);

	/*TSharedPtr<IPlugin> CurrentPlugin = IPluginManager::Get().FindPlugin(DIALOGUE_SYSTEM_PLUGIN_NAME.ToString());
	if (CurrentPlugin.IsValid())
	{
		// Replaces the Engine Content Root (Engine/Editor/Slate) with the plugin content root
		StyleSet->SetContentRoot(CurrentPlugin->GetContentDir());
		PluginContentRoot = CurrentPlugin->GetContentDir();
	}*/

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FInstantDialogueEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

const FName& FInstantDialogueEditorStyle::GetStyleSetName()
{
	return StyleSet->GetStyleSetName();
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
