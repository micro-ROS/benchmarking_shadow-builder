#ifndef __TFA_CORE_TFABENCHMARKINGPLATFORM_H__
#define __TFA_CORE_TFABENCHMARKINGPLATFORM_H__

#include <string>

class TFABenchMarkingPlatform {
public:
	TFABenchMarkingPlatform (const TFABenchMarkingPlatform& tfaconf)
		:
		mOSName(tfaconf.mOSName), mCPUArch(tfaconf.mCPUArch),
	       	mCPUName(tfaconf.mCPUName), mBoardName(tfaconf.mBoardName)
	{
	}

	TFABenchMarkingPlatform(
			const std::string& OSName,
			const std::string& CPUArch,
			const std::string& CPUName,
			const std::string& boardName);

	bool operator==(const TFABenchMarkingPlatform& A) const;

	bool operator!=(const TFABenchMarkingPlatform& A) const
	{
		return !operator==(A);
	}


	const TFABenchMarkingPlatform& operator=(const TFABenchMarkingPlatform& A)
	{
		mBoardName = A.mBoardName;
		mCPUName = A.mCPUName;
		mCPUArch = A.mCPUArch;
		mOSName = A.mOSName;

		return *this;
	}
	friend std::ostream& operator<<(std::ostream& out, 
			const TFABenchMarkingPlatform& tbp) {

		out << std::string("OS: ") << tbp.mOSName << std::string(" Arch: ")
			<< tbp.mCPUArch << std::string(" CPU: ") << tbp.mCPUName 
			<< std::string(" Board: ") << tbp.mBoardName;

		return out;
	}

private:
	std::string mOSName;
	std::string mCPUArch;
	std::string mCPUName;
	std::string mBoardName;
};

#endif /* __TFA_CORE_TFAPLUGININFO_H__ */
