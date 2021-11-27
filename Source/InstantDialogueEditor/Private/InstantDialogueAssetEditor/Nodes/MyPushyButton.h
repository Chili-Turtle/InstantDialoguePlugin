#pragma once

#include "CoreMinimal.h"
#include "ClassViewerModule.h"
#include "SedNode_InstantDialogueCondition.h"
#include "Widgets/Input/SComboButton.h"

DECLARE_DELEGATE_OneParam(FOnPickerClosed, int) //UClass*
DECLARE_MULTICAST_DELEGATE(FTest)
DECLARE_DELEGATE_OneParam(FStinky, UClass*)

class SMyPushyButton : public SComboButton
{

public:
	SLATE_BEGIN_ARGS(SMyPushyButton){}

	SLATE_EVENT(FStinky, OnMenuClosed)
	
	SLATE_EVENT(FStinky, myStinky)

	// old stuff
	SLATE_EVENT( FOnGetContent, OnGetMenuContent )
	SLATE_EVENT( FOnIsOpenChanged, OnMenuOpenChanged )
	
	SLATE_EVENT( FOnComboBoxOpened, OnComboBoxOpened )
	SLATE_ARGUMENT( bool, IsFocusable )
	SLATE_ARGUMENT( bool, HasDownArrow )
	SLATE_ATTRIBUTE( FSlateColor, ForegroundColor )
	SLATE_ATTRIBUTE( FSlateColor, ButtonColorAndOpacity )
	SLATE_ATTRIBUTE( FMargin, ContentPadding )
	SLATE_ATTRIBUTE( EMenuPlacement, MenuPlacement )
	SLATE_ARGUMENT( EHorizontalAlignment, HAlign )
	SLATE_ARGUMENT( EVerticalAlignment, VAlign )
	/** Spawn a new window or reuse current window for this combo*/
	SLATE_ARGUMENT( TOptional<EPopupMethod>, Method )
	/** True if this combo's menu should be collapsed when our parent receives focus, false (default) otherwise */
	SLATE_ARGUMENT(bool, CollapseMenuOnParentFocus)
	//old stuff

	SLATE_END_ARGS()

	FStinky OnMenuClosed1;
	
	virtual void Construct(const FArguments& InArgs);

	TSharedPtr<class SComboButton> ComboButton;
	void CreateClassFilter();
	TSharedRef<SWidget> GenerateClassPicker();
	void OnClassPicked(UClass* InClass);
	FText GetDisplayValueAsString() const;


	/** Options used for creating the class viewer. */
	FClassViewerInitializationOptions ClassViewerOptions;

	/** The meta class that the selected class must be a child-of */
	const UClass* MetaClass;
	/** An interface that the selected class must implement */
	const UClass* RequiredInterface;
	/** Whether or not abstract classes are allowed */
	bool bAllowAbstract;
	/** Should only base blueprints be displayed? */
	bool bIsBlueprintBaseOnly;
	/** Should we be able to select "None" as a class? */
	bool bAllowNone;
	/** Should only placeable classes be displayed? */
	bool bAllowOnlyPlaceable;
	/** Should we show the view options button at the bottom of the class picker?*/
	bool bShowViewOptions;
	/** Should we show the class picker in tree mode or list mode?*/
	bool bShowTree;
	/** Should we prettify class names on the class picker? (ie show their display name) */
	bool bShowDisplayNames;
	/** Classes that can be picked with this property */
	TArray<const UClass*> AllowedClassFilters;
	/** Classes that can NOT be picked with this property */
	TArray<const UClass*> DisallowedClassFilters;
	
	/** Class filter that the class viewer is using. */
    TSharedPtr<IClassViewerFilter> ClassFilter;

	/** Filter functions for class viewer. */
	TSharedPtr<FClassViewerFilterFuncs> ClassFilterFuncs;

	/** Attribute used to get the currently selected class (required if PropertyEditor == null) */
	TAttribute<const UClass*> SelectedClass;
	/** Delegate used to set the currently selected class (required if PropertyEditor == null) */
	//FOnSetClass OnSetClass;
};
