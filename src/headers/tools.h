#include "config.h"

struct IntersectionResult
{
	bool bHasIntersect;

	float fY;
	float fX;

	IntersectionResult();
	IntersectionResult(float fY, float fX);
};

IntersectionResult intersection(std::vector<sf::Vector2f> line1, std::vector<sf::Vector2f> line2);

void createCircle(std::vector<std::vector<sf::Vector2f>>& boundaries, float fY, float fX, float fRadius, int iVertexCount);
void displayConsoleInformation(std::map<std::string, float>& cfg);

std::map<std::string, float> readConfig(std::string sConfigPath);

EVENT_CODE eventListener(sf::RenderWindow& window);

int init(sf::RenderWindow& window);
int main();