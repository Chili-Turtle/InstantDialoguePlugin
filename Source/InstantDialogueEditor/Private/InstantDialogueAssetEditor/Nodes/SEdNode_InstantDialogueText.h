#pragma once

#include "CoreMinimal.h"

#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"
#include "ConditionObject.h"
#include "EdNode_InstantDialogueAnswer.h"
#include "PropertyCustomizationHelpers.h"
#include "SGraphNode.h"
#include "SEdNode_InstantDialogueNode.h"

class SEdNode_Text : public SEdNode_InstantDialogueNode
{
public:
	SLATE_BEGIN_ARGS(SEdNode_Text)
		//: _Font(FEditorStyle::GetFontStyle(PropertyEditorConstants::PropertyFontStyle)) 
		: _MetaClass(UObject::StaticClass())
		, _RequiredInterface(nullptr)
		, _AllowAbstract(false)
		, _IsBlueprintBaseOnly(false)
		, _AllowNone(true)
		, _ShowDisplayNames(false)
	{}
	SLATE_ARGUMENT(FSlateFontInfo, Font)

	/** Arguments used when constructing this outside of a PropertyEditor (PropertyEditor == null), ignored otherwise */
	/** The meta class that the selected class must be a child-of (required if PropertyEditor == null) */
	SLATE_ARGUMENT(const UClass*, MetaClass)
	/** An interface that the selected class must implement (optional) */
	SLATE_ARGUMENT(const UClass*, RequiredInterface)
	/** Whether or not abstract classes are allowed (optional) */
	SLATE_ARGUMENT(bool, AllowAbstract)
	/** Should only base blueprints be displayed? (optional) */
	SLATE_ARGUMENT(bool, IsBlueprintBaseOnly)
	/** Should we be able to select "None" as a class? (optional) */
	SLATE_ARGUMENT(bool, AllowNone)
	/** Attribute used to get the currently selected class (required if PropertyEditor == null) */
	SLATE_ATTRIBUTE(const UClass*, SelectedClass)
	/** Should we show the view options button at the bottom of the class picker?*/
	SLATE_ARGUMENT(bool, ShowViewOptions)
	/** Should we show the class picker in tree mode or list mode?*/
	SLATE_ARGUMENT(bool, ShowTree)
	/** Should we prettify class names on the class picker? (ie show their display name) */
	SLATE_ARGUMENT(bool, ShowDisplayNames)
	/** Delegate used to set the currently selected class (required if PropertyEditor == null) */
	SLATE_EVENT(FOnSetClass, OnSetClass)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode);

	virtual void UpdateGraphNode() override;
	
    	virtual void CreatePinWidgets() override;
    	//virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
    	//virtual bool IsNameReadOnly() const override;
    
    	virtual FSlateColor GetBorderBackgroundColor() const override;
    	virtual FSlateColor GetBackgroundColor() const override;
    
    	virtual EVisibility GetDragOverMarkerVisibility() const override;
    
    	virtual const FSlateBrush* GetNameIcon() const override;
    	
    	// Create the inner node content area, including the left/right pin boxes
    	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
    	// @param OwnerPanel  The GraphPanel that this node belongs to
		//virtual void SetOwner( const TSharedRef<SGraphPanel>& OwnerPanel ) override;
    	// Create a single pin widget
    	virtual void CreateStandardPinWidget(UEdGraphPin* Pin) override;
    	//virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

	virtual void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;
	virtual FReply OnAddPin() override;

	void OnSpeakerNameCommitted(const FText& InText, ETextCommit::Type CommitInfo);
	void OnDialogueCommitted(const FText& InText, ETextCommit::Type CommitInfo);


//UComboWidget Objects
	// TArray<UObject*> Items;
	// TSharedRef<SWidget> RebuildWidget();
	
	TSharedRef<SWidget>	GetDefaultValueWidget();
	TSharedRef<SWidget> GenerateAssetPicker();
	void OnAssetSelectedFromPicker(const struct FAssetData& AssetData);
	const FAssetData& GetAssetData(bool bRuntimePath) const;
	void OnAssetEnterPressedInPicker(const TArray<FAssetData>& InSelectedAssets);
	FString DefaultValue = "Default";

	/** Object manipulator buttons. */
	TSharedPtr<SButton> UseButton;
	TSharedPtr<SButton> BrowseButton;
	
	TSharedPtr<class SMenuAnchor> AssetPickerAnchor;
	
	/** Cached AssetData of object selected */
	mutable FAssetData CachedAssetData;

	FSlateColor OnGetWidgetBackground() const;
	FSlateColor OnGetWidgetForeground() const;
	FSlateColor OnGetComboForeground() const;
	
	FOnClicked GetOnBrowseButtonDelegate();
	FReply OnClickBrowse();
	FOnClicked GetOnUseButtonDelegate();
	FReply OnClickUse();
	FText OnGetComboTextValue() const;
	FText GetDefaultComboText() const;
	FText GetObjectToolTip() const;
	FText GetValue() const;



	/////////////// SPropertyEditorClass
	
	//Used when the property deals with Classes and will display a Class Picker. */
	TSharedPtr<class SComboButton> ComboButton;
	
	TSharedRef<SWidget> GenerateComboButton(); //const FArguments& InArgs, const TSharedPtr< class FPropertyEditor >& InPropertyEditor
	FText GetDisplayValueAsString() const;
	void CreateClassFilter();
	TSharedRef<SWidget> GenerateClassPicker();
	void OnClassPicked(UClass* InClass);

	void SendToObjects(const FString& NewValue);

	/** Options used for creating the class viewer. */
	FClassViewerInitializationOptions ClassViewerOptions;

	/** The property editor we were constructed for, or null if we're editing using the construction arguments */
	//TSharedPtr<class FPropertyEditor> PropertyEditor;
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
	FOnSetClass OnSetClass;
	

};


class FInstantConditionFilterViewer : public IClassViewerFilter
{
public:
	TSet<const UClass*> AllowedChildrenOfClasses;
	//uint32 DisallowedClassFlags;
	EClassFlags DisallowedClassFlags;
	
	//virtual ~FInstantConditionFilterViewer() override;
	 virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
	 	TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	 {
	 	//return !InClass->HasAnyClassFlags(DisallowedClassFlags)
		//	&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	 	return InClass->IsChildOf(UInstantDialogueConditionObject::StaticClass());
	 }
	
	 virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
	 	const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
	 	TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	 {
	 	return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	 	//return InClass->IsChildOf(UAnimationModifier::StaticClass());
	 }
};
