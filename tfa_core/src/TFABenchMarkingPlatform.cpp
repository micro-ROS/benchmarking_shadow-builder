#include <tfa/TFABenchMarkingPlatform.h>


TFABenchMarkingPlatform::TFABenchMarkingPlatform(
			const std::string& OSName, 
			const std::string& CPUArch,
			const std::string& CPUName,
			const std::string& boardName):
 			mOSName(OSName), mCPUArch(CPUArch),
			mCPUName(CPUName), mBoardName(boardName)
{
}


bool TFABenchMarkingPlatform::operator==(const TFABenchMarkingPlatform& A) const
{
	if ((mOSName != A.mOSName) && (A.mOSName != "*" && mOSName != "*") )
		return false;

	if (mCPUName != A.mCPUName && (A.mCPUName != "*" && mCPUName != "*"))
		return false;

	if (mCPUArch != A.mCPUArch && 
			(A.mCPUArch != "*" && mCPUArch != "*"))
		return false;

	if (mBoardName != A.mBoardName 
			&& (A.mBoardName != "*" && mBoardName != "*"))
		return false;

	return true;
}
