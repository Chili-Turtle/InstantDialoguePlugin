#include "InstantDialoguePin.h"


#if !UE_BUILD_SHIPPING

FString FInstantDialoguePinRecord::NoActivations = TEXT("No activations");
FString FInstantDialoguePinRecord::PinActivations = TEXT("Pin activations");

FInstantDialoguePinRecord::FInstantDialoguePinRecord()
{
	Time = 0.0f;
	HumanReadableTime = FString();
}

FInstantDialoguePinRecord::FInstantDialoguePinRecord(const double InTime)
{
	Time = InTime;

	const FDateTime SystemTime(FDateTime::Now());
	HumanReadableTime = DoubleDigit(SystemTime.GetHour()) + TEXT(".")
		+ DoubleDigit(SystemTime.GetMinute()) + TEXT(".")
		+ DoubleDigit(SystemTime.GetSecond()) + TEXT(":")
		+ DoubleDigit(SystemTime.GetMillisecond()).Left(3);
}

FORCEINLINE FString FInstantDialoguePinRecord::DoubleDigit(const int32 Number)
{
	return Number > 9 ? FString::FromInt(Number) : TEXT("0") + FString::FromInt(Number);
}
#endif