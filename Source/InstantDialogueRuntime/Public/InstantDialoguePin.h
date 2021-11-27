#pragma once

#include "InstantDialoguePin.generated.h"

USTRUCT()
struct INSTANTDIALOGUERUNTIME_API FInstantPin
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "FlowPin")
	FName PinName;

	UPROPERTY(EditDefaultsOnly, Category = "FlowPin")
	FString PinToolTip;

	FInstantPin()
		: PinName(NAME_None)
	{
	}

	FInstantPin(const FName& InPinName)
		: PinName(InPinName)
	{
	}

	FInstantPin(const FString& InPinName)
		: PinName(*InPinName)
	{
	}

	FInstantPin(const FText& InPinName)
        : PinName(*InPinName.ToString())
	{
	}

	FInstantPin(const TCHAR* InPinName)
		: PinName(FName(InPinName))
	{
	}

	FInstantPin(const uint8& InPinName)
		: PinName(FName(*FString::FromInt(InPinName)))
	{
	}

	FInstantPin(const int32& InPinName)
		: PinName(FName(*FString::FromInt(InPinName)))
	{
	}

	FInstantPin(const FName& InPinName, const FString& InPinTooltip)
		: PinName(InPinName)
		, PinToolTip(InPinTooltip)
	{
	}

	FInstantPin(const FString& InPinName, const FString& InPinTooltip)
		: PinName(*InPinName)
		, PinToolTip(InPinTooltip)
	{
	}

	FInstantPin(const FText& InPinName, const FString& InPinTooltip)
        : PinName(*InPinName.ToString())
        , PinToolTip(InPinTooltip)
	{
	}

	FInstantPin(const TCHAR* InPinName, const FString& InPinTooltip)
		: PinName(FName(InPinName))
		, PinToolTip(InPinTooltip)
	{
	}
	
	FORCEINLINE bool IsValid() const
	{
		return !PinName.IsNone();
	}

	FORCEINLINE bool operator==(const FInstantPin& Other) const
	{
		return PinName == Other.PinName;
	}

	FORCEINLINE bool operator!=(const FInstantPin& Other) const
	{
		return PinName != Other.PinName;
	}

	FORCEINLINE bool operator==(const FName& Other) const
	{
		return PinName == Other;
	}

	FORCEINLINE bool operator!=(const FName& Other) const
	{
		return PinName != Other;
	}
};

// Processing Flow Nodes creates map of connected pins
USTRUCT()
struct INSTANTDIALOGUERUNTIME_API FInstantDialogueConnectedPin
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FGuid NodeGuid;

	UPROPERTY()
	FName PinName;

	FInstantDialogueConnectedPin()
		: NodeGuid(FGuid())
		, PinName(NAME_None)
	{
	}

	FInstantDialogueConnectedPin(const FGuid InNodeId, const FName& InPinName)
		: NodeGuid(InNodeId)
		, PinName(InPinName)
	{
	}
};

// Every time pin is activated, we record it and display this data while user hovers mouse over pin
#if !UE_BUILD_SHIPPING
struct INSTANTDIALOGUERUNTIME_API FInstantDialoguePinRecord
{	
	double Time;
	FString HumanReadableTime;

	static FString NoActivations;
	static FString PinActivations;

	FInstantDialoguePinRecord();
	FInstantDialoguePinRecord(const double InTime);

	private:
	FORCEINLINE static FString DoubleDigit(const int32 Number);
};
#endif