// Simple command-line application that will detect and remove UTF-8 style Byte Order Mark (BOM)
//

#include <iostream>
#include <fstream>
#include <string>

// Check Visual Studio
#if _WIN32 || _WIN64
	#if _WIN64
		#define ENVBITS 64
	#else
		#define ENVBITS 32
	#endif
#endif

// Check GCC
#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define ENVBITS 64
	#else
		#define ENVBITS 32
	#endif
#endif

enum EFileType
{
	NO_BOM = 0,
	UTF_8,
	UTF_16_LE,
	UTF_16_BE,
	UTF_32_LE,
	UTF_32_BE,
};

void NormalizeFilePath(std::string& fileName);
const EFileType DetectBom(const std::string& inFile);
void CopyWithoutUTF8BOM(const std::string& inFile, const std::string& outFile);

int main(int argc, char* argv[])
{
	// Only accept a specific line format 
	if (argc != 3)
	{
		// Give the user a clue of this is all about
		std::cerr << "UTF-8 BOM Remover : v1.0 [" << ENVBITS << " bits] : " << __DATE__ << std::endl;
		std::cerr << "Copyright(c) 2017 Felipe \"Zoc\" Silveira : http://www.github.com/TheZoc/BOMRemover" << std::endl;
		std::cerr << std::endl;
		std::cerr << "Usage: " << argv[0] << " <original file> <output file>" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Lazy++
	std::string inFile = argv[1];
	std::string outFile = argv[2];

	// Normalize file path
	NormalizeFilePath(inFile);
	NormalizeFilePath(outFile);

	EFileType fileType = DetectBom(inFile);
	switch (fileType)
	{
	case NO_BOM:
		std::cerr << inFile << " - No supported BOM detected. This could be a UTF-8 file without BOM or some other format. Stopping." << std::endl;
		break;
	case UTF_8:
		std::cout << inFile << " - File type detected as UTF-8 with BOM. Saving it as " << outFile << std::endl;
		break;
	case UTF_16_LE:
		std::cerr << inFile << " - File type detected as UTF-16 Little Endian. Stopping." << std::endl;
		break;
	case UTF_16_BE:
		std::cerr << inFile << " - File type detected as UTF-16 Big Endian. Stopping." << std::endl;
		break;
	case UTF_32_LE:
		std::cerr << inFile << " - File type detected as UTF-32 Little Endian. Stopping." << std::endl;
		break;
	case UTF_32_BE:
		std::cerr << inFile << " - File type detected as UTF-32 Big Endian. Stopping." << std::endl;
		break;
	default:
		std::cerr << inFile << " - Something has gone wrong. Stopping." << std::endl;
		break;
	}

	CopyWithoutUTF8BOM(inFile, outFile);

	return 0;
}

// Convert supplied path to use forward slashes
// From https://github.com/TheZoc/ZPatcher/blob/master/ZPatcherLib/FileUtils.cpp
void NormalizeFilePath(std::string& fileName)
{
	for (char & i : fileName)
		if (i == '\\')
			i = '/';
}

const EFileType DetectBom(const std::string& inFile)
{
	using namespace std;
	ifstream file(inFile, ifstream::in | ifstream::binary);

	const unsigned char BOM_SIZE = 4;
	unsigned char BOM[BOM_SIZE];

	file.read((char*)BOM, BOM_SIZE); // read() only accepts char*, but we want unsigned char*, so, this quick trick will do it ;)

	if (BOM[0] == 0xFE && BOM[1] == 0xFF)
		return EFileType::UTF_16_BE;

	if (BOM[0] == 0xFF && BOM[1] == 0xFE)
	{
		if (BOM[2] == 0x00 && BOM[3] == 0x00)
			return EFileType::UTF_32_LE;
		return EFileType::UTF_16_LE;
	}

	if (BOM[0] == 0xEF && BOM[1] == 0xBB && BOM[2] == 0xBF)
		return UTF_8;

	if (BOM[0] == 0x00 && BOM[1] == 0x00 && BOM[2] == 0xFE && BOM[3] == 0xFF)
		return EFileType::UTF_32_BE;

	// Either has no BOM or is in some other format.
	return EFileType::NO_BOM;
}

void CopyWithoutUTF8BOM(const std::string& inFile, const std::string& outFile)
{
	using namespace std;
	ifstream in(inFile, ios::binary | ifstream::in);
	ofstream out(outFile, ios::binary | ifstream::out);

	in.seekg(3); // UTF-8 BOM Size in bytes

	out << in.rdbuf();

	in.close();
	out.close();
}
