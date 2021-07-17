#include "tools.h"

void createCircle(std::vector<std::vector<sf::Vector2f>>& boundaries, float fY, float fX, float fRadius, int iVertexCount)
{
	float fAngleStep = mPi * 2 / iVertexCount;
	float fLastY = fY + sin(0) * fRadius;
	float fLastX = fX + cos(0) * fRadius;
	float fAngle = fAngleStep;

	for (; fAngle < mPi * 2; fAngle += fAngleStep)
	{
		float fTempY = fY + sin(fAngle) * fRadius;
		float fTempX = fX + cos(fAngle) * fRadius;

		boundaries.push_back(std::vector<sf::Vector2f>({
			sf::Vector2f(fLastX, fLastY),
			sf::Vector2f(fTempX, fTempY)
		}));

		fLastY = fTempY;
		fLastX = fTempX;
	}

	boundaries.push_back(std::vector<sf::Vector2f>({
		sf::Vector2f(fLastX, fLastY),
		sf::Vector2f(fX + cos(0) * fRadius, fY + sin(0) * fRadius)
	}));
}

IntersectionResult::IntersectionResult()
{
	this -> bHasIntersect = false;
}

IntersectionResult::IntersectionResult(float fY, float fX)
{
	this -> fY = fY;
	this -> fX = fX;
	this -> bHasIntersect = true;
}

float determinant(sf::Vector2f& p1, sf::Vector2f& p2, sf::Vector2f& p3)
{
	return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

IntersectionResult intersection(std::vector<sf::Vector2f> line1, std::vector<sf::Vector2f> line2)
{
	float fZ1 = determinant(line1[0], line1[1], line2[0]);
	float fZ2 = determinant(line1[0], line1[1], line2[1]);
	
	if (zer::athm::sign(fZ1) == zer::athm::sign(fZ2))
		return IntersectionResult();

	float iZ3 = determinant(line2[0], line2[1], line1[0]);
	float iZ4 = determinant(line2[0], line2[1], line1[1]);

	if (zer::athm::sign(iZ3) == zer::athm::sign(iZ4))
		return IntersectionResult();
	
	float fY = line2[0].y + (line2[1].y - line2[0].y) * fabs(fZ1) / fabs(fZ2 - fZ1);
	float fX = line2[0].x + (line2[1].x - line2[0].x) * fabs(fZ1) / fabs(fZ2 - fZ1);

	return IntersectionResult(fY, fX);
}

void displayConsoleInformation(std::map<std::string, float>& cfg)
{
	system("cls");

	std::cout << "# " << mTitle << " #" << std::endl;
	std::cout << "\n[!] keyboard buttons for control:" << std::endl;
	std::cout << "\t [ ESC ] - exit;" << std::endl;
	std::cout << "\t [ R ] - restart;" << std::endl;
	std::cout << "\n[!] note: visit a \"" << mConfigPath << "\" file to change configuration;" << std::endl;
	std::cout << "\n[!] current configuration:" << std::endl;
	
	for (std::map<std::string, float>::iterator p = cfg.begin(); p != cfg.end(); p++)
		std::cout << "\t" << p -> first << " = " << p -> second << ";" << std::endl;
}

std::map<std::string, float> readConfig(std::string sConfigPath)
{
	std::map<std::string, float> cfg;

	zer::File file(sConfigPath);
	file.read({zer::file::Modifier::lines});

	for (int i = 0; i < file.linesLen(); ++i)
	{
		std::string sLine = file.lineAt(i);
		if (sLine.find(" = ") != std::string::npos)
		{
			std::vector<std::string> lineParts = zer::athm::split(sLine, " = ");
			cfg[lineParts[0]] = stof(lineParts[1]);
		}
	}

	return cfg;
}

EVENT_CODE eventListener(sf::RenderWindow& window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			return CLOSE_EVENT_CODE;
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
				return CLOSE_EVENT_CODE;
			if (event.key.code == sf::Keyboard::R)
				return RESTART_EVENT_CODE;
		}
	}
	return NULL_EVENT_CODE;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(mWW, mWH), mTitle);
	return init(window);
}