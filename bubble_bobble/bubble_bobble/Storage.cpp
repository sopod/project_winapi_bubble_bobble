#include "Storage.h"
#include <fstream>

Storage::Storage() { }

Storage::Storage(const Storage & ref) { }

Storage::~Storage()
{
	DestroyImageFile(player);
	DestroyImageFile(enemy);
	DestroyImageFile(tile);
	DestroyImageFile(character);
	DestroyImageFile(bubble);
	DestroyImageFile(item);

	DestroySoundFile(bgmOnce);
	DestroySoundFile(bgmIng);
	DestroySoundFile(bgmEffect);
	DestroySoundFile(playerSound);
}

void Storage::FileInit()
{
	LoadImageFile();
	LoadDataFile();
	LoadSoundFile();
}

void Storage::LoadImageFile()
{
	std::ifstream readFile;
	readFile.open("sprite.csv");

	char cur, c[50];
	int i = 0, row = 0;

	if (!readFile.is_open()) return;

	readFile.get(cur);
	while (!readFile.eof())
	{
		while (cur != ',' && cur != '\n')
		{
			c[i] = cur;
			i++;
			readFile.get(cur);
		}

		c[i] = '\0';
		i = 0;
		f[row].push_back(CharToWideChar(c));
		
		if (cur == '\n')
		{
			row++;
			readFile.get(cur);
			continue;
		}

		// 그 다음 것 미리 읽어보기... 
		// 만약 또 ,라면 ,가 아닐때까지 읽어버린다. 
		// \n라면 다음 행으로

		readFile.get(cur);

		while (cur == ',')
			readFile.get(cur);

		if (cur == '\n')
		{
			row++;
			readFile.get(cur);
		}
	}
	readFile.close();


	// 이미지 모두 로드
	int idx = 0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < f[idx].size(); j++)
		{
			player[i].push_back(Image::FromFile(f[idx][j]));
		}
		idx++;
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < f[idx].size(); j++)
		{
			enemy[i].push_back(Image::FromFile(f[idx][j]));
		}
		idx++;
	}

	for (int j = 0; j < f[17].size(); j++)
	{
		tile[0].push_back(Image::FromFile(f[17][j]));
	}
	idx++;

	for (int j = 0; j < f[18].size(); j++)
	{
		character[0].push_back(Image::FromFile(f[18][j]));
	}
	idx++;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < f[idx].size(); j++)
		{
			bubble[i].push_back(Image::FromFile(f[idx][j]));
		}
		idx++;
	}

	for (int j = 0; j < f[24].size(); j++)
	{
		item[0].push_back(Image::FromFile(f[24][j]));
	}
	idx++;
}

wchar_t * Storage::CharToWideChar(char * str)
{
	wchar_t * res;

	char c;
	int i = 0;
	while (str[i] != '\0') // \0까지의 카운트 i
	{
		i++;
	}

	int strSize = MultiByteToWideChar(CP_ACP, 0, str, i, NULL, NULL);

	res = new wchar_t[strSize];

	MultiByteToWideChar(CP_ACP, 0, str, i, res, strSize);

	res[strSize] = NULL;
	
	return res;	
}


void Storage::DestroyImageFile(std::vector<Image*> * img)
{
	for (int i = 0; i < (*img).size(); i++)
	{
		delete (*img)[i];
	}
}


std::vector<Image*>& Storage::GetPlayerImg(STATE state, bool isAttacking, bool isStartMotion, int & frameMax)
{
	if (isStartMotion == true)
	{
		frameMax = player[0].size();
		return player[0];
	}

	if (isAttacking == true)
	{
		frameMax = player[state + 6].size();
		return player[state + 6];
	}
	else
	{
		frameMax = player[state + 1].size();
		return player[state + 1];
	}

}

std::vector<Image*>& Storage::GetEnemyImg(STATE state, bool isAngry, int & frameMax)
{
	if (state == DEAD)
	{
		frameMax = enemy[1].size();
		return enemy[1];
	}

	if (isAngry == false)
	{
		frameMax = enemy[0].size();
		return enemy[0];
	}
	else
	{
		frameMax = enemy[2].size();
		return enemy[2];
	}
}

std::vector<Image*>& Storage::GetTileImg()
{
	return tile[0];
}

std::vector<Image*>& Storage::GetCharImg()
{
	return character[0];
}

std::vector<Image*>& Storage::GetBubbleImg(B_STATE state, bool isContainEnemy, int & frameMax)
{
	if (isContainEnemy == false)
	{
		frameMax = bubble[state].size();
		return bubble[state];
	}
	else
	{
		switch (state)
		{
		case SLIDING:
		case NORMAL:
			frameMax = enemy[3].size();
			return enemy[3];

		case CORROSION1:
			frameMax = enemy[4].size();
			return enemy[4];

		case CORROSION2:
			frameMax = enemy[5].size();
			return enemy[5];

		case POP:
			frameMax = bubble[state].size();
			return bubble[state];
		}
	}
}

std::vector<Image*>& Storage::GetItemImg()
{
	return item[0];
}

void Storage::SaveTopFiveDataFile(DATA & curUser)
{
	// 유저의 스코어를 포함해서 top5를 파일에 저장하기
	DATA prev;
	DATA next;
	bool isOut = false;

	for (int i = 0; i < 5; i++)
	{
		if (curUser.score > top[i].score)
		{
			prev = top[i];
			top[i] = curUser;

			if (prev.score == 0)
				break;

			for (int j = i + 1; j < 5; j += 2)
			{
				isOut = true;
				next = top[j];
				top[j] = prev;
				if (j + 1 >= 5 || next.score == 0)
					break;
				prev = top[j + 1];
				top[j + 1] = next;
			}

			if (isOut == true)
				break;
		}
	}	
	
	std::ofstream writeFile;
	writeFile.open("data.txt", std::ios_base::out | std::ios_base::binary);

	if (writeFile.is_open() == false)
		return;

	for(int i = 0; i < 5; i++)
		writeFile.write((char*)&top[i], sizeof(DATA));

	writeFile.close();
}

void Storage::LoadDataFile()
{
	// 파일에서 정보 불러오기
	std::ifstream readFile;
	readFile.open("data.txt", std::ios_base::in | std::ios_base::binary);

	if (readFile.is_open() == false)
		return;
	
	for (int i = 0; i < 5; i++)
		readFile.read((char*)&top[i], sizeof(DATA));

	readFile.close();
}

DATA * Storage::GetTop()
{
	return top;
}

int Storage::GetTopScore()
{
	return top[0].score;
}


void Storage::LoadSoundFile()
{
	std::ifstream readFile;
	readFile.open("bgm.csv");

	char cur;
	int i = 0, row = 0;

	if (readFile.is_open() == false)
		return;

	readFile.get(cur);
	while (readFile.eof() == false)
	{
		char * res = new char[50];
		while (cur != ',' && cur != '\n')
		{
			res[i] = cur;
			i++;
			readFile.get(cur);
		}
		res[i] = '\0';
		i = 0;

		b[row].push_back(res);

		if (cur == '\n')
		{
			row++;
			readFile.get(cur);
			continue;
		}

		readFile.get(cur);

		while (cur == ',')
			readFile.get(cur);

		if (cur == '\n')
		{
			row++;
			readFile.get(cur);
		}
	}

	readFile.close();

	int idx = 0;
	for (int i = 0; i < b[idx].size(); i++)
	{
		bgmOnce.push_back(new Sound(b[idx][i], false));
	}
	idx++;

	for (int i = 0; i < b[idx].size(); i++)
	{
		bgmIng.push_back(new Sound(b[idx][i], true)); // 배경 음악만 계속 반복
	}
	idx++;
	
	for (int i = 0; i < b[idx].size(); i++)
	{
		bgmEffect.push_back(new Sound(b[idx][i], false));
	}
	idx++;

	for (int i = 0; i < b[idx].size(); i++)
	{
		playerSound.push_back(new Sound(b[idx][i], false));
	}
	idx++;
}

Sound * & Storage::GetBgm(SCREEN screen, bool isHurryUp)
{
	switch (screen)
	{
	case LOBBY:
		return bgmOnce[0];

	case ROUNDCHANGE:
	case GAME:
		if (isHurryUp == false)
		{
			return bgmIng[0];
		}
		else
		{
			return bgmIng[1];
		}

	case SCORE:
		return bgmOnce[1];

	case GAMEOVER:
		return bgmOnce[2];
	}
}

Sound *& Storage::GetBgmEffect()
{
	return bgmEffect[0];
}

Sound *& Storage::GetPlayerSound(PLAYER_SOUND ps)
{
	switch (ps)
	{
	case JUMP:
		return playerSound[0];

	case ATTACK:
		return playerSound[1];

	case POPBUBBLE:
		return playerSound[2];

	case GETITEM:
		return playerSound[3];

	case GOODBYE:
		return playerSound[4];

	}
}

void Storage::DestroySoundFile(std::vector<Sound*> & sound)
{
	for (int i = 0; i < sound.size(); i++)
	{
		delete sound[i];
	}
}
