// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// //Widget Stuff
// #include "Widgets/SWidget.h"
// #include "Widgets/SBoxPanel.h"
// #include "Widgets/SCompoundWidget.h"
// #include "Widgets/Input/SComboBox.h"
// #include "Widgets/Layout/SWrapBox.h"
// #include "Widgets/Layout/SBox.h"
// //#include "Widgets/Input/SButton.h"
// #include "ClassViewerModule.h"
// #include "Widgets/Input/SComboButton.h"
// #include "Widgets/Input/SMenuAnchor.h"
// #include "PropertyCustomizationHelpers.h"


/**
 * 
 */
// class SEasyBoxWidget : public SCompoundWidget
// {
// public:
// 	SLATE_BEGIN_ARGS(SEasyBoxWidget) {}
// 	SLATE_END_ARGS()
//
// 	void Construct(const FArguments& InArgs);
//
// 	//Used when the property deals with Classes and will display a Class Picker. */
// 	TSharedPtr<class SComboButton> ComboButton;
// 	
// 	//TSharedRef<SWidget> GenerateComboButton(); //const FArguments& InArgs, const TSharedPtr< class FPropertyEditor >& InPropertyEditor
//
// 	void CreateClassFilter();
// 	TSharedRef<SWidget> GenerateClassPicker();
// 	void OnClassPicked(UClass* InClass);
// 	FText GetDisplayValueAsString() const;
//
// 	//void SendToObjects(const FString& NewValue);
//
// 	/** Options used for creating the class viewer. */
// 	FClassViewerInitializationOptions ClassViewerOptions;
//
// 	/** The property editor we were constructed for, or null if we're editing using the construction arguments */
// 	//TSharedPtr<class FPropertyEditor> PropertyEditor;
// 	/** The meta class that the selected class must be a child-of */
// 	const UClass* MetaClass;
// 	/** An interface that the selected class must implement */
// 	const UClass* RequiredInterface;
// 	/** Whether or not abstract classes are allowed */
// 	bool bAllowAbstract;
// 	/** Should only base blueprints be displayed? */
// 	bool bIsBlueprintBaseOnly;
// 	/** Should we be able to select "None" as a class? */
// 	bool bAllowNone;
// 	/** Should only placeable classes be displayed? */
// 	bool bAllowOnlyPlaceable;
// 	/** Should we show the view options button at the bottom of the class picker?*/
// 	bool bShowViewOptions;
// 	/** Should we show the class picker in tree mode or list mode?*/
// 	bool bShowTree;
// 	/** Should we prettify class names on the class picker? (ie show their display name) */
// 	bool bShowDisplayNames;
// 	/** Classes that can be picked with this property */
// 	TArray<const UClass*> AllowedClassFilters;
// 	/** Classes that can NOT be picked with this property */
// 	TArray<const UClass*> DisallowedClassFilters;
// 	
// 	/** Class filter that the class viewer is using. */
//     TSharedPtr<IClassViewerFilter> ClassFilter;
//
// 	/** Filter functions for class viewer. */
// 	TSharedPtr<FClassViewerFilterFuncs> ClassFilterFuncs;
//
// 	/** Attribute used to get the currently selected class (required if PropertyEditor == null) */
// 	TAttribute<const UClass*> SelectedClass;
// 	/** Delegate used to set the currently selected class (required if PropertyEditor == null) */
// 	FOnSetClass OnSetClass;
// };
