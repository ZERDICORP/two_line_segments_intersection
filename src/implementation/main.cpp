#include "config.h"
#include "tools.h"

#define toRadians zer::athm::toRadians

int loop(sf::RenderWindow& window, std::map<std::string, float>& cfg)
{
	bool bNeedToUpdateConsole = true;

	float fAngle = 0;
	float fFov = toRadians(cfg["FOV"]);
	float fFovHalf = fFov / 2;
	float fRayLength = mWW;;
	float fRaysCount = cfg["raysCount"];
	float fRayStep = fFov / fRaysCount;
	float fCameraY = mWH / 2;
	float fCameraX = mWW / 2;

	int iObjectsAngleOffset = 0;
	int iObjectsRadius = cfg["objectsRadius"];
	int iObjectsOrbitalRadius = cfg["objectsOrbitalRadius"];

	std::vector<std::vector<sf::Vector2f>> boundaries;

	sf::VertexArray boundary(sf::LineStrip, 2);
	boundary[0].color = sf::Color(255, 255, 255);
	boundary[1].color = sf::Color(255, 255, 255);

	sf::VertexArray ray(sf::LineStrip, 2);
	ray[0].color = sf::Color(255, 0, 0);
	ray[1].color = sf::Color(255, 0, 0);

	std::vector<sf::Vector2f> line(2);

	while (window.isOpen())
	{
		/*
			Clear boundaries vector.
		*/
		std::vector<std::vector<sf::Vector2f>>().swap(boundaries);

		/*
			Create objects.
		*/
		createCircle(boundaries, fCameraY + sin(toRadians(iObjectsAngleOffset) + toRadians(0)) * iObjectsOrbitalRadius,
			fCameraX + cos(toRadians(iObjectsAngleOffset) + toRadians(0)) * iObjectsOrbitalRadius, iObjectsRadius, 3);
		
		createCircle(boundaries, fCameraY + sin(toRadians(iObjectsAngleOffset) + toRadians(90)) * iObjectsOrbitalRadius,
			fCameraX + cos(toRadians(iObjectsAngleOffset) + toRadians(90)) * iObjectsOrbitalRadius, iObjectsRadius, 4);
		
		createCircle(boundaries, fCameraY + sin(toRadians(iObjectsAngleOffset) + toRadians(180)) * iObjectsOrbitalRadius,
			fCameraX + cos(toRadians(iObjectsAngleOffset) + toRadians(180)) * iObjectsOrbitalRadius, iObjectsRadius, 6);
		
		createCircle(boundaries, fCameraY + sin(toRadians(iObjectsAngleOffset) + toRadians(270)) * iObjectsOrbitalRadius,
			fCameraX + cos(toRadians(iObjectsAngleOffset) + toRadians(270)) * iObjectsOrbitalRadius, iObjectsRadius, 30);

		iObjectsAngleOffset %= 360;
		iObjectsAngleOffset -= cfg["objectsRotateSpeed"];

		window.clear();

		for (float f = -fFovHalf; f < fFovHalf; f += fRayStep)
		{
			float fSinus = sin(fAngle + f);
			float fCosinus = cos(fAngle + f);
			float fY = fCameraY + fSinus * fRayLength;
			float fX = fCameraX + fCosinus * fRayLength;
			float fDist = fRayLength;

			for (int i = 0; i < boundaries.size(); ++i)
			{
				line[0] = sf::Vector2f(fCameraX, fCameraY);
				line[1] = sf::Vector2f(fX, fY);

				/*
					Find intersection.
				*/
				IntersectionResult result = intersection(line, boundaries[i]);
				if (result.bHasIntersect)
				{
					float fCatet1 = fCameraY - result.fY;
					float fCatet2 = fCameraX - result.fX;
					float fHypot = sqrt(fCatet1 * fCatet1 + fCatet2 * fCatet2);

					if (fHypot < fDist)
						fDist = fHypot;
				}
			}

			fY = fCameraY + fSinus * fDist;
			fX = fCameraX + fCosinus * fDist;

			ray[0].position = sf::Vector2f(fCameraY, fCameraX);
			ray[1].position = sf::Vector2f(fY, fX);

			window.draw(ray);
		}

		for (int i = 0; i < boundaries.size(); ++i)
		{
			boundary[0].position = sf::Vector2f(boundaries[i][0].y, boundaries[i][0].x);
			boundary[1].position = sf::Vector2f(boundaries[i][1].y, boundaries[i][1].x);
			
			window.draw(boundary);
		}

		window.display();

		if (bNeedToUpdateConsole)
		{
			displayConsoleInformation(cfg);
			bNeedToUpdateConsole = false;
		}

		switch (eventListener(window))
		{
			case (CLOSE_EVENT_CODE):
				window.close();
				break;

			case RESTART_EVENT_CODE:
				init(window);
				break;
		}
	}
	return 0;
}

int init(sf::RenderWindow& window)
{
	std::map<std::string, float> cfg = readConfig(mConfigPath);
	return loop(window, cfg);
}