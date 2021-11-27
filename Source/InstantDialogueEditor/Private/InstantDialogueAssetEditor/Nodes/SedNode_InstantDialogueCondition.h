// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClassViewerFilter.h"
#include "ClassViewerModule.h"

#include "IAutomationControllerManager.h"
#include "MyPushyButton.h"
#include "PropertyCustomizationHelpers.h"
#include "SEdNode_InstantDialogueNode.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FCondtionCommitted, UClass* curClass, int curIndex); //,
DECLARE_DELEGATE(FUpateConditionComitted)
DECLARE_DELEGATE_TwoParams( FMyOnClassPicked, UClass*, int );

class UEdNode_InstantDialogueNode;
class SedNode_Condition : public SEdNode_InstantDialogueNode
{
public:
	SLATE_BEGIN_ARGS(SedNode_Condition) {}
	SLATE_END_ARGS()

	FUpateConditionComitted OnUpdateCondition;

	void Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode);
	
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const;
	virtual void CreateStandardPinWidget(UEdGraphPin* Pin) override;

	//TArray<TSharedPtr<SMultiBoxWidget>> MultiBoxArray;
	//TArray<TSharedPtr<SOverlay>> ConditionBoxArray;
	TArray<UObject*> ConditionArray;

	void OnConditionCommited(UClass* CondtionClass, int curIndex ); //, int OutputIndex
	void OnConditionCommited1();

	///// condition
	TSharedRef<SOverlay> ConditionBox(int index);
	TSharedPtr<SOverlay> ConditionBoxVar; // dont use TSharedRef it give you gibberish errors, and you lose your mind :) 
	
	TSharedPtr<class SComboButton> ComboButton;
	
	//TSharedRef<SWidget> GenerateComboButton(); //const FArguments& InArgs, const TSharedPtr< class FPropertyEditor >& InPropertyEditor

	void CreateClassFilter();
	TSharedRef<SWidget> GenerateClassPicker();
	void OnClassPicked(UClass* InClass);
	FText GetDisplayValueAsString() const;
	
	TArray<TSharedPtr<SMyPushyButton>> PushyButtons;

	//void SendToObjects(const FString& NewValue);

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
