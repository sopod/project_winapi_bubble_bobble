#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct DATA
{
	int score = 0;
	int level = 0;
};

std::vector<DATA> data;

void SaveDataFile(DATA & curUser)
{
	std::ofstream writeFile;
	writeFile.open("data.txt", std::ios_base::out | std::ios_base::binary | std::ios::app);

	if (writeFile.is_open() == false)
		return;

	writeFile.write((char*)&curUser, sizeof(DATA));

	writeFile.close();
}

void LoadDataFile()
{
	std::ifstream readFile;
	readFile.open("data.txt", std::ios_base::in | std::ios_base::binary);

	if (readFile.is_open() == false)
		return;

	if (data.size() != 0)
		data.clear();

	DATA temp;
	while (readFile.read((char*)&temp, sizeof(DATA)))
	{
		data.push_back(temp);
	}

	readFile.close();
}


int main()
{
	LoadDataFile(); // 저장된 파일 불러오기	

	std::cout << "기존 정보" << std::endl;
	for (int i = 0; i < data.size(); i++)
	{
		std::cout << data[i].level << " " << data[i].score << " " << std::endl;
	}

	DATA curData;
	curData.level = 2;
	curData.score = 2;

	SaveDataFile(curData); // 새로운 정보 저장하기

	LoadDataFile(); // 새로운 정보를 추가한 파일 다시 로드하기
	
	std::cout << "기존 정보 + 추가된 정보" << std::endl;
	for (int i = 0; i < data.size(); i++)
	{
		std::cout << data[i].level << " " << data[i].score << " " << std::endl;
	}

	return 0;
}